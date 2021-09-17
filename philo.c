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
	actions->each_must_eat = -1;
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
		philo[i].last_meal = 0;
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

void	print_this(long time, int philosopher, t_philo *philo,  char *is_doing)
{
	
	pthread_mutex_lock(&philo->actions->mutex_print);

	write_nbr(time);
	write(1, "ms ", 3);
	write_nbr(philosopher);
	write(1, " ", 1);
	write_str(is_doing);
	write(1, "\n", 1);

	pthread_mutex_unlock(&(philo->actions->mutex_print));
}


void	*is_alive(void *arg)
{
	t_philo		*philo;
	long		time_b;
	long		actual_time;

	philo = (t_philo*)arg;

	time_b = get_ms();
	while (philo->alive)
	{
		pthread_mutex_lock(&(philo->actions->mutex_die));
		actual_time = get_ms() - time_b;
		if (philo->actions->is_alive && actual_time - philo->last_meal >= (long)philo->actions->tto_die)
		{
		//printf("dead time and actual %ld , last meal %ld\n", actual_time, philo->last_meal);
			print_this(actual_time, philo->philosopher, philo, DIED);
			philo->actions->is_alive = 0;
			philo->alive = 0;
			pthread_mutex_unlock(&(philo->actions->mutex_die));
			break ;
		}
		pthread_mutex_unlock(&(philo->actions->mutex_die));
	}
	return (NULL);
}

void	is_sleeping(t_philo *philo, long startt)
{
	pthread_mutex_lock(&(philo->actions->mutex_die));
	print_this(get_ms() - startt, philo->philosopher, philo, IS_SLEEPING);
	pthread_mutex_unlock(&(philo->actions->mutex_die));

	usleep(philo->actions->tto_sleep * 1000);
}

void	is_eating(t_philo *philo, long startt)
{
	pthread_mutex_lock(&(philo->actions->mutex_die));
	print_this(get_ms() - startt, philo->philosopher, philo, IS_EATING);
	philo->last_meal = get_ms() - startt;
	pthread_mutex_unlock(&(philo->actions->mutex_die));

	usleep(philo->actions->tto_eat * 1000);
	philo->has_eat += 1;
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

	pthread_mutex_lock(&(philo->actions->mutex_fork[left]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	pthread_mutex_lock(&(philo->actions->mutex_fork[right]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	is_eating(philo, startt);

	pthread_mutex_unlock(&(philo->actions->mutex_fork[right]));
	pthread_mutex_unlock(&(philo->actions->mutex_fork[left]));

}

void	*routine(void *arg)
{
	t_philo		*philo;
	pthread_t	thread;
	long		startt;

	philo = (t_philo*)arg;
	philo->startt = get_ms();
	thread = malloc(sizeof(pthread_t) * 1);
	if (pthread_create(&thread, NULL, &is_alive, (void *)philo))
		return (NULL);
 	startt = get_ms();
	while (philo->alive)
	{
		if (philo->actions->is_alive && philo->actions->each_must_eat && (philo->has_eat == philo->actions->each_must_eat))
		{
			pthread_mutex_lock(&(philo->actions->mutex_die));
			philo->actions->is_alive = 0;
			philo->alive = 0;
			pthread_mutex_unlock(&(philo->actions->mutex_die));
			printf("All philos ate %d time\n", philo->actions->each_must_eat);
			break ;
		}
		takes_forks(philo, startt);
		is_sleeping(philo, startt);
	}
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
	simulation(&actions);
	return (clean_all(&actions));
}
