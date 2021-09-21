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

int	all_alive(t_philo *philo)
{
	pthread_mutex_lock(&(philo->actions->mutex_die));
	if (philo->actions->are_alive)
	{
		pthread_mutex_unlock(&(philo->actions->mutex_die));
		return (1);
	}
	pthread_mutex_unlock(&(philo->actions->mutex_die));
	return (0);
}

void	print_this(long time, int philosopher, t_philo *philo,  char *is_doing)
{
//	if (!all_alive(philo))
//		return ;
	pthread_mutex_lock(&philo->actions->mutex_print);

	// write_nbr(time);
	// write(1, "ms ", 3);
	// write_nbr(philosopher);
	// write(1, " ", 1);
	// write_str(is_doing);
	// write(1, "\n", 1);

	printf("%ldms %d %s\n", time, philosopher, is_doing);
	pthread_mutex_unlock(&(philo->actions->mutex_print));
}


void	*is_alive(void *arg)
{
	t_philo		*philo;
	long		time_b;
	long		actual_time;
	int			last_meal2;

	philo = (t_philo*)arg;
	time_b = get_ms();
	while (1)
	{
		// if (!)
		// 	break ;
		pthread_mutex_lock(&(philo->actions->mutex_meal));
		last_meal2 = philo->last_meal ;
		pthread_mutex_unlock(&(philo->actions->mutex_meal));
		
		actual_time = get_ms() - philo->startt;
		if (actual_time - philo->last_meal > (long)philo->actions->tto_die)
		{				
			pthread_mutex_lock(&(philo->actions->mutex_die));
			philo->actions->are_alive = 0;
			print_this(actual_time, philo->philosopher, philo, DIED);
			pthread_mutex_unlock(&(philo->actions->mutex_die));
			break ;
		}
	}
	return (NULL);
}

void	my_usleep(long time, t_philo* philo)
{
	long time_before = get_ms() - philo->startt;
	long time_actual = 0;

	pthread_mutex_lock(&philo->actions->mutex_print);
	printf("before philo %d\t%ldms\n", philo->philosopher, time_before);
	pthread_mutex_unlock(&philo->actions->mutex_print);
	

	/*if (!all_alive(philo))
	{
		// pthread_mutex_lock(&philo->actions->mutex_print);
		// printf("dead philo %d\t%ldms / %ldms in usleep\n", philo->philosopher, time_actual, time);
		// pthread_mutex_unlock(&philo->actions->mutex_print);
		return ;
	}*/
	while (time_actual < time)
	{
		usleep(10);
		time_actual = get_ms() - philo->startt ;
		// pthread_mutex_lock(&philo->actions->mutex_print);
		// printf("%ldms philo %d\t%ldms / %ldms in usleep\n", get_ms() - philo->startt, philo->philosopher, time_actual, time);
		// pthread_mutex_unlock(&philo->actions->mutex_print);
	}
	// pthread_mutex_lock(&philo->actions->mutex_print);
	// printf("out philo %d\t%ldms / %ldms in usleep\n", philo->philosopher, time_actual, time);
	// pthread_mutex_unlock(&philo->actions->mutex_print);


}

int	is_sleeping(t_philo *philo, long startt, int alive)
{
	(void)alive;
	print_this(get_ms() - startt, philo->philosopher, philo, IS_SLEEPING);
	my_usleep(philo->actions->tto_sleep, philo);
	return (1);
}

void	is_eating(t_philo *philo, long startt)
{
	if (!all_alive(philo))
		return ;
	
	print_this(get_ms() - startt, philo->philosopher, philo, IS_EATING);

	pthread_mutex_lock(&(philo->actions->mutex_meal));
	philo->last_meal = get_ms() - startt;
	pthread_mutex_unlock(&(philo->actions->mutex_meal));

	my_usleep(philo->actions->tto_eat, philo);

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

	print_this(get_ms() - startt, philo->philosopher, philo, IS_THINKING);

	pthread_mutex_lock(&(philo->actions->mutex_fork[left]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	pthread_mutex_lock(&(philo->actions->mutex_fork[right]));
	print_this(get_ms() - startt, philo->philosopher, philo, TAKE_FORK);

	is_eating(philo, startt);

	pthread_mutex_unlock(&(philo->actions->mutex_fork[right]));
	pthread_mutex_unlock(&(philo->actions->mutex_fork[left]));
	return (1);
}

void	*routine(void *arg)
{
	t_philo		*philo;
	pthread_t	thread;
	long		startt;
//	int			philo_alive;

	philo = (t_philo*)arg;
	philo->startt = get_ms();
	if (pthread_create(&thread, NULL, &is_alive, (void *)philo))
		return (NULL);
 	startt = get_ms();
	while (all_alive(philo))
	{
		takes_forks(philo, startt, 0);
		if (philo->actions->each_must_eat && (philo->has_eat == philo->actions->each_must_eat))
		{
			pthread_mutex_lock(&(philo->actions->mutex_die));
			philo->actions->are_alive = 0;
			pthread_mutex_unlock(&(philo->actions->mutex_die));
			printf("All philos ate %d time\n", philo->actions->each_must_eat);
			break ;
		}
		is_sleeping(philo, startt, 0);
	}
	pthread_join(thread, NULL);
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
		if (pthread_create(&thread[i], NULL, &routine, (actions->philo + i)))
			break ;
		i++;
	}
	while (--i >= 0)
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
