#include "philo.h"

int	init(int ac, char **av)
{
	int i;

	if (ac < 5 || ac > 6)
		return (0);
	actions->nb_philosophers =  my_atoi(av[1]);
	actions->tto_die = my_atoi(av[2]);
	actions->tto_eat =  my_atoi(av[3]);
	actions->tto_sleep =  my_atoi(av[4]);
	actions->each_must_eat = -1;
	actions->are_alive = 1;
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
	//	philo[i].actions = actions;
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
	long			time;

	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000;
	return (time);
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

void	print_this(long time, int philosopher, t_philo *philo,  char *is_doing)
{
	
	(void)philo;
	pthread_mutex_lock(&actions->mutex_print);

	write_nbr(time);
	write(1, "ms ", 3);
	write_nbr(philosopher);
	write(1, " ", 1);
	write_str(is_doing);
	write(1, "\n", 1);

	pthread_mutex_unlock(&(actions->mutex_print));
}


void	*is_alive(void *arg)
{
	t_philo		*philo;
	long		time_b;
	long		actual_time;

	philo = (t_philo*)arg;
	time_b = get_ms();
	while (1)
	{
	pthread_mutex_lock(&(actions->mutex_meal));
		actual_time = get_ms() - time_b - philo->last_meal;
	pthread_mutex_unlock(&(actions->mutex_meal));

		if (all_alive() && actual_time  >= (long)actions->tto_die)
		{				
	//	printf("dead time and actual %ld , last meal %ld\n", actual_time, philo->last_meal);
			pthread_mutex_lock(&(actions->mutex_die));
			print_this(actual_time, philo->philosopher, philo, DIED);
			actions->are_alive = 0;
			pthread_mutex_unlock(&(actions->mutex_die));
			break ;
		}
	}
	return (NULL);
}

int	is_sleeping(t_philo *philo, long startt, int alive)
{
(void)alive;
	print_this(get_ms() - startt, philo->philosopher, philo, IS_SLEEPING);
	usleep(actions->tto_sleep * 1000);
	return (1);
}

void	is_eating(t_philo *philo, long startt)
{
	
	pthread_mutex_lock(&(actions->mutex_meal));
	print_this(get_ms() - startt, philo->philosopher, philo, IS_EATING);
	philo->last_meal = get_ms() - startt;
	pthread_mutex_unlock(&(actions->mutex_meal));

	usleep(actions->tto_eat * 1000);
	philo->has_eat += 1;
}

int	takes_forks(t_philo *philo, long startt, int alive)
{
	int		left;
	int		right;

(void)alive;
	if (philo->philosopher % 2 == 0)
		left = philo->philosopher + 1;
	else
		left = philo->philosopher;
	if (philo->philosopher % 2 == 0)
		right = philo->philosopher;
	else
		right = philo->philosopher - 1;
printf("right = %d\n", right);
printf("left = %d\n", left);
	print_this(get_ms() - startt, philo->philosopher, philo, IS_THINKING);

	pthread_mutex_lock(&(actions->mutex_fork[left]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	pthread_mutex_lock(&(actions->mutex_fork[right]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	is_eating(philo, startt);

	pthread_mutex_unlock(&(actions->mutex_fork[right]));
	pthread_mutex_unlock(&(actions->mutex_fork[left]));
	return (1);
}

void	*routine(void *arg)
{
	t_philo		*philo;
	pthread_t	thread;
	long		startt;

	philo = (t_philo*)arg;
	while (!actions->here)
		;
	if (pthread_create(&thread, NULL, &is_alive, (void *)philo))
		return (NULL);
 	startt = get_ms();
	while (all_alive())
	{
		takes_forks(philo, startt,0);
		if (actions->each_must_eat && (philo->has_eat == actions->each_must_eat))
		{
			pthread_mutex_lock(&(actions->mutex_die));
			actions->are_alive = 0;
			pthread_mutex_unlock(&(actions->mutex_die));
			printf("All philos ate %d time\n", actions->each_must_eat);
			break ;
		}
		is_sleeping(philo, startt,0);
	}
	pthread_join(thread, NULL);
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
	actions->here = 0;
	while (i < actions->nb_philosophers)
	{
		if (pthread_create(&thread[i], NULL, &routine, (actions->philo + i)))
			break ;
		i++;
	}
	actions->here = 1;
	while (--i >= 0)
		pthread_join(thread[i], NULL);
	free(thread);
}

int	clean_all()
{
	int	i;

	i = 0;
	if (actions->mutex_fork)
	{
		while (i < actions->nb_philosophers)
			pthread_mutex_destroy(&(actions->mutex_fork[i++]));
		free(actions->mutex_fork);
	}
	pthread_mutex_destroy(&(actions->mutex_die));
	pthread_mutex_destroy(&(actions->mutex_print));
	if (actions->philo != NULL)
		free(actions->philo);
	return (0);
}

int main(int ac, char **av)
{
	t_actions		act;

	actions = &act;
	if (!init(ac, av))
		return (clean_all());
	actions->philo = the_philo();
	simulation();
	return (clean_all());
}
