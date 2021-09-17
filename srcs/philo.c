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
	{
		actions->each_must_eat =  my_atoi(av[5]);
		if (actions->each_must_eat <= 0)
			return (0);
	}
	if (actions->nb_philosophers <= 0 || actions->tto_die <= 0 ||
		actions->tto_eat <= 0 || actions->tto_sleep <= 0)
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
	if (pthread_mutex_init(&(actions->mutex_print), NULL))
		return (0);
	if (pthread_mutex_init(&(actions->mutex_meal), NULL))
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
		philo[i].has_eat = 0;
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

void	print_this(long time, int philosopher, t_philo *philo,  char *is_doing)
{
	
	pthread_mutex_lock(&philo->actions->mutex_print);

	printf("%ld ms %d %s \n", time, philosopher, is_doing);

	pthread_mutex_unlock(&(philo->actions->mutex_print));
}


int	not_dead(t_philo *philo, long startt)
{
	long int			time;

	pthread_mutex_lock(&(philo->actions->mutex_die));
	if (!philo->actions->is_alive)
		return (0);
	time = get_ms() - startt;
	printf("dead time %ld, last meal %ld, time = %ld \n", time, philo->last_meal , time - philo->last_meal);
	if (time >= (long)philo->actions->tto_die)
	{
		philo->actions->is_alive = 0;
		print_this(get_ms() - startt, philo->philosopher, philo, DIED);
		return (0);
	}
	pthread_mutex_unlock(&(philo->actions->mutex_die));
	return (1);
}

void	is_sleeping(t_philo *philo, long startt)
{
	print_this(get_ms() - startt, philo->philosopher, philo, IS_SLEEPING);
	usleep(philo->actions->tto_sleep * 1000);
}

void is_eating(t_philo *philo, long startt)
{
	if (!philo->actions->is_alive)
		return ;
	print_this(get_ms() - startt, philo->philosopher, philo, IS_EATING);
	philo->last_meal = get_ms() - startt;
	philo->has_eat += 1;
	usleep(philo->actions->tto_eat * 1000);
}

void	takes_forks(t_philo *philo, long startt)
{
	int		left;
	int		right;

	if (philo->philosopher % 2 == 0)
		left = philo->philosopher + 1;
	else
		left = philo->philosopher;
	if (philo->philosopher % 2 == 0)
		right = philo->philosopher;
	else
		right = philo->philosopher - 1;

	print_this(get_ms() - startt, philo->philosopher, philo, IS_THINKING);
	
	pthread_mutex_lock(&(philo->actions->mutex_die));
	pthread_mutex_lock(&(philo->actions->mutex_fork[left]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);
	pthread_mutex_lock(&(philo->actions->mutex_fork[right]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	is_eating(philo, startt);

	pthread_mutex_unlock(&(philo->actions->mutex_fork[right]));
	pthread_mutex_unlock(&(philo->actions->mutex_fork[left]));
	pthread_mutex_unlock(&(philo->actions->mutex_die));

}

void	*routine(void *arg)
{
	t_philo *philo;
	long	 startt;

	philo = (t_philo*)arg;
 	startt = get_ms();
	while (philo->actions->is_alive)
	{
		takes_forks(philo, startt);
		if (philo->actions->each_must_eat && (philo->has_eat == philo->actions->each_must_eat))
		{
			not_dead(philo, startt);
			break ;
		}
		is_sleeping(philo, startt);
		not_dead(philo, startt);
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
			break ;
		i++;
	}
	while (--i > 0)
		pthread_join(thread[i], NULL);
	free(thread);
}

int main(int ac, char **av)
{
	t_actions		actions;

	if (!init(ac, av, &actions))
		return (clean_all(&actions));
	actions.philo = the_philo(&actions);
	printf("phi %d, die %d, eat %d, sleep %d , MUST eat %d\n", actions.nb_philosophers,actions.tto_die , actions.tto_eat, actions.tto_sleep , actions.each_must_eat);
	simulation(&actions);
	return (clean_all(&actions));
}