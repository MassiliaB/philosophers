#include "philo.h"

int	clean_all()
{
	int	i;

	i = 0;
	if (actions->mutex_fork)
	{
		while (i <= actions->nb_philosophers)
		{
			pthread_mutex_destroy(&(actions->mutex_fork[i]));
			i++;
		}
		free(actions->mutex_fork);
	}
	pthread_mutex_destroy(&(actions->mutex_die));
	pthread_mutex_destroy(&(actions->mutex_print));
	pthread_mutex_destroy(&(actions->mutex_meal));
	if (actions->philo)
		free(actions->philo);
	return (0);
}

int	init_mutex()
{
	int	i;

	actions->mutex_fork = malloc(sizeof(pthread_mutex_t) * (actions->nb_philosophers + 1));
	if (!actions->mutex_fork)
		return (0);
	i = 0;
	while (i <= actions->nb_philosophers)
	{
		if (pthread_mutex_init(&(actions->mutex_fork[i]), 0))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&(actions->mutex_die), 0))
		return (0);
	if (pthread_mutex_init(&(actions->mutex_print), 0))
		return (0);
	if (pthread_mutex_init(&(actions->mutex_meal), 0))
		return (0);
	return (1);
}

int	init_args(int args, char **av)
{
	if (args < 5 || args > 6)
		return (0);
	actions->nb_philosophers =	my_atoi(av[1]);
	actions->tto_die = my_atoi(av[2]);
	actions->tto_eat =	my_atoi(av[3]);
	actions->tto_sleep =  my_atoi(av[4]);
	actions->each_must_eat = -1;
	actions->are_alive = 1;
	if (args == 6)
	{
		actions->each_must_eat =  my_atoi(av[5]);
		if (actions->each_must_eat <= 0)
			return (0);
	}
	if (actions->nb_philosophers <= 0 || actions->tto_die <= 0 ||
		actions->tto_eat <= 0 || actions->tto_sleep <= 0)
		return (0);
	if (!init_mutex())
		return (0);
	return (1);
}

t_philo	*the_philo()
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * actions->nb_philosophers);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < actions->nb_philosophers)
	{
		philo[i].philosopher = i + 1;
		philo[i].has_eat = 0;
		philo[i].last_meal = 0;
		i++;
	}
	return (philo);
}

long	get_ms()
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (((long)current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000);
}

void	my_usleep(long time, long start)
{
	long int time_before;
	long int time_actual;

	time_before = get_ms() - start;
	time_actual = 0;
	while ((get_ms() - start) - time_before <= time)
		usleep(100);
}

int	all_alive()
{	
	pthread_mutex_lock(&(actions->mutex_die));
	if (!actions->are_alive)
	{
		pthread_mutex_unlock(&(actions->mutex_die));
		return (0);
	}
	pthread_mutex_unlock(&(actions->mutex_die));
	return (1);
}

void	print_this(long time, int philosopher, char *is_doing)
{
	pthread_mutex_lock(&actions->mutex_print);

	if (!all_alive())
	{
		pthread_mutex_unlock(&(actions->mutex_print));
		return ;
	}
	write_nbr(time);
	write(1, "ms ", 3);
	write_nbr(philosopher);
	write(1, " ", 1);
	write_str(is_doing);
	write(1, "\n", 1);
	pthread_mutex_unlock(&(actions->mutex_print));
}	


void	is_eating(t_philo *philo, long start)
{

	print_this(get_ms() - start, philo->philosopher, IS_EATING);

	pthread_mutex_lock(&(actions->mutex_meal));
	philo->last_meal = get_ms() - start;
	pthread_mutex_unlock(&(actions->mutex_meal));

	my_usleep(actions->tto_eat, start);
	philo->has_eat += 1;
}

int	takes_forks_and_eat(t_philo *philo, long start)
{
	int		left;
	int		right;

	
	left = philo->philosopher;
	right = philo->philosopher - 1;
	if (philo->philosopher % 2 == 0)
		right = philo->philosopher + 1;
	
	pthread_mutex_lock(&(actions->mutex_fork[right]));
	print_this(get_ms() - start, philo->philosopher, TAKE_FORK);

	pthread_mutex_lock(&(actions->mutex_fork[left]));
	print_this(get_ms() - start, philo->philosopher, TAKE_FORK);


	is_eating(philo, start);

	pthread_mutex_unlock(&(actions->mutex_fork[right]));
	pthread_mutex_unlock(&(actions->mutex_fork[left]));
	return (1);
}

int	is_it_dead(t_philo *philo, long start)
{
	long	actual_time;

	pthread_mutex_lock(&(actions->mutex_meal));
	actual_time = get_ms() - start - philo->last_meal;
	pthread_mutex_unlock(&(actions->mutex_meal));
	if (all_alive() && actual_time >= (long)actions->tto_die)
	{
		print_this(actual_time, philo->philosopher, DIED);
		pthread_mutex_lock(&(actions->mutex_die));
		actions->are_alive = 0;
		pthread_mutex_unlock(&(actions->mutex_die));
		return (1);
	}
	return (0);
}

int	time_has_eat(t_philo *philo)
{
	if (all_alive() && philo->has_eat == actions->each_must_eat)
	{
		pthread_mutex_lock(&(actions->mutex_die));
		actions->are_alive = 0;
		pthread_mutex_unlock(&(actions->mutex_die));
		write(1, "All philos ate", 14);
		write_nbr(philo->has_eat);
		write(1, "time\n", 5);
		return (0);
	}
	return (1);
}

void	*routine(void *arg)
{
	t_philo		*philo;
	long		start;

	philo = (t_philo*)arg;
	start = get_ms();
	while (all_alive())
	{
		print_this(get_ms() - start, philo->philosopher, IS_THINKING);
		takes_forks_and_eat(philo, start);
		if (!time_has_eat(philo))
			return (NULL);
		print_this(get_ms() - start, philo->philosopher, IS_SLEEPING);
		my_usleep(actions->tto_sleep, start);
		if (is_it_dead(philo, start))
		{
			return (NULL);}
	}
	return (NULL);
}

void	simulation()
{
	pthread_t		*thread;
	int				i;

	thread = malloc(sizeof(pthread_t) * actions->nb_philosophers);
	if (!thread)
		return ;
	i = 0;
	while (i < actions->nb_philosophers)
	{
		if (pthread_create(&thread[i], 0, &routine, (void*)&(actions->philo[i])))
			break ;
		i++;
	}
	i = 0;
	while (i < actions->nb_philosophers)
	{
		pthread_join(thread[i], 0);
		i++;
	}
	free(thread);
}

int	main(int ac, char **av)
{
	t_actions	act;

	actions = &act;
	if (!init_args(ac, av))
		return (0);
	actions->philo = the_philo();
	simulation();
	return (clean_all());
}
