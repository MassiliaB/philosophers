#include "philo.h"

int	init(int ac, char **av, t_actions *actions)
{
	int i;

	if (ac < 5 || ac > 6)
		return (0);
	actions->nb_philosophers =  my_atoi(av[1]);
	actions->tto_die = my_atoi(av[2]);
	actions->tto_eat =  my_atoi(av[3]);
	actions->tto_sleep =  my_atoi(av[4]);
	actions->each_must_eat = 0;
	actions->is_alive = 1;
	if (ac == 6)
		actions->each_must_eat =  my_atoi(av[5]);
	if (actions->nb_philosophers <= 0 || actions->tto_die <= 0 ||
		actions->tto_eat <= 0 || actions->tto_sleep <= 0 || actions->each_must_eat < 0)
		return (0);
	actions->mutex_fork = malloc(sizeof(pthread_mutex_t) * actions->nb_philosophers);
	if (!actions->mutex_fork)
		return (0);
	i = 0;
	while (i < actions->nb_philosophers)
		if (pthread_mutex_init(&(actions->mutex_fork[i++]), NULL))
			return (0);
	if (pthread_mutex_init(&(actions->mutex_die), NULL))
		return (0);
	return (1);
}

t_philo	*the_philo(t_actions *actions)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * actions->nb_philosophers);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < actions->nb_philosophers)
	{
		philo[i].actions = actions;
		philo[i].philosopher = i + 1;
		philo[i].alive = 1;
		i++;
	}
	return (philo);
}

long	get_ms()
{
	struct timeval	current_time;
	long			time;

	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000;
	return (time);
}

long	current_time(struct timeval *start)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000000 + current_time.tv_usec) - (start->tv_sec * 1000000 + start->tv_usec));
}

int	not_dead(t_philo *philo)
{
	long int			time;

	time = current_time(&philo->actions->start);
	if (time >= philo->actions->tto_die && philo->actions->is_alive)
	{
		pthread_mutex_lock(&(philo->actions->mutex_die));
		printf("%ldms [%d] %s\n", time, philo->philosopher, DIED);
		return (0);
	}
	return (1);
}

// si le temps pour manger et dormir > temps pour mourir = meurs 
// si temps pour mourir > temps pour manger et dormir = meurt pas 
void	is_sleeping(t_philo *philo)
{
	long	time;

	time = current_time(&philo->actions->start);
	if (!philo->actions->is_alive)
		return ;
	printf("%ldms [%d] %s\n", time, philo->philosopher, IS_SLEEPING);
	usleep(philo->actions->tto_sleep);
}

void is_eating(t_philo *philo)
{
	long		time;

	time = current_time(&philo->actions->start);
	printf("%ldms [%d] %s\n", time, philo->philosopher, IS_EATING);
	usleep(philo->actions->tto_eat);
}

int	takes_forks(t_philo *philo)
{
	int		left;
	int		right;
	long	time;

	if (philo->philosopher % 2 == 0)
		left = philo->philosopher + 1;
	else
		left = philo->philosopher;
	if (philo->philosopher % 2 == 0)
		right = philo->philosopher;
	else
		right = philo->philosopher - 1;

	pthread_mutex_lock(&(philo->actions->mutex_fork[left]));
	time = current_time(&philo->actions->start);
	printf("%ldms [%d] %s\n", time, philo->philosopher, TAKE_FORK);

	pthread_mutex_lock(&(philo->actions->mutex_fork[right]));
	time = current_time(&philo->actions->start);
	printf("%ldms [%d] %s\n", time, philo->philosopher, TAKE_FORK);

	is_eating(philo);
	pthread_mutex_unlock(&(philo->actions->mutex_fork[right]));
	pthread_mutex_unlock(&(philo->actions->mutex_fork[left]));
	return (1);
}

void	*routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo->actions->is_alive)
	{
		current_time(&philo->actions->start);
		printf("%ldms [%d] %s\n", current_time(&philo->actions->start), philo->philosopher, IS_THINKING);
		takes_forks(philo);
		is_sleeping(philo);
		philo->actions->is_alive = not_dead(philo);
	}
	philo->actions->is_alive = 0;
	pthread_mutex_unlock(&(philo->actions->mutex_die));
	return (NULL);
}

void	simulation(t_actions *actions)
{
	pthread_t		*thread;
	int				i;

	thread = malloc(sizeof(pthread_t) * actions->nb_philosophers);
	if (!thread)
		return ;
	i = 0;
	while (i < actions->nb_philosophers)
	{
		if (pthread_create(&thread[i], NULL, &routine, actions->philo + i))
			return ;
		i++;
	}
	while (--i > 0)
		pthread_join(thread[i], NULL);
}

int main(int ac, char **av)
{
	t_actions		actions;
	struct timeval	start;

	gettimeofday(&start, NULL);
	actions.start = start;
	if (!init(ac, av, &actions))
		return (0);
	actions.philo = the_philo(&actions);
	printf("phi %d, die %d, eat %d, sleep %d , MUST eat %d\n", actions.nb_philosophers,actions.tto_die , actions.tto_eat, actions.tto_sleep , actions.each_must_eat);
	simulation(&actions);
	return (clean_all(&actions));
}
	//	printf("holaa\n");
	//	pthread_join(thread, result of the fct sent tto pthread creat);