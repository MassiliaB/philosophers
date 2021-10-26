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
	pthread_mutex_destroy(&(actions->mutex_eat));
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
	if (pthread_mutex_init(&(actions->mutex_eat), 0))
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
	actions->each_one = 0;
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
	return ((current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000);
}

void	my_usleep(long time, long start)
{
	long int time_before;
	long int time_actual;

	time_before = get_ms() - start;
	time_actual = 0;
	while ((get_ms() - start) - time_before < time)
		usleep(10);
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

int	nb_size(int nb)
{
	int size;

	size = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= 10;
		size++;
	}
	return (size);
}

void	buff_digit(int size, long nb, int index, char *buff)
{
	int i;

	i = 0;
	if (nb == 0)
		buff[i++] = '0';
	while (i < size)
	{
		buff[index + (size - i)] = nb % 10 + '0';
		nb /= 10;
		i++;
	}
	buff[index + size + i] = '\0';
}

void	buff_this(char *buff, long start, int philosopher, char *is_doing)
{
	int len;
	int i;
	long time;

	time = 0;
	time = get_ms() - start;
	buff_digit(nb_size(time), time, -1, buff);
	len = nb_size(time);
	buff[len++] = 'm';
	buff[len++] = 's';
	buff[len] = ' ';
	buff_digit(nb_size(philosopher), philosopher, len, buff);
	len += nb_size(philosopher) + 1;
	i = 0;
	while (is_doing[i])
	{
		buff[len + i] = is_doing[i];
		i++;
	}
	buff[len + i] = '\0';
}
void	print_this(long start, int philosopher, char *is_doing)
{
	char to_print[10000];
	pthread_mutex_lock(&actions->mutex_print);
	if (!all_alive())
	{
		pthread_mutex_unlock(&(actions->mutex_print));
		return;
	}
	buff_this(to_print, start, philosopher, is_doing);
	write(1, &to_print, ft_strlen(to_print));
	pthread_mutex_unlock(&(actions->mutex_print));
	return ;
}	


void	is_eating(t_philo *philo, long start)
{
	print_this(start, philo->philosopher, IS_EATING);
	pthread_mutex_lock(&(actions->mutex_meal));
	philo->last_meal = get_ms() - start;
	pthread_mutex_unlock(&(actions->mutex_meal));
	my_usleep(actions->tto_eat, start);

	pthread_mutex_lock(&(actions->mutex_eat));
	philo->has_eat += 1;
	pthread_mutex_unlock(&(actions->mutex_eat));
}

int	takes_forks_and_eat(t_philo *philo, long start)
{
	int		left;
	int		right;

	left = philo->philosopher + 1;
	right = philo->philosopher;
	if (philo->philosopher % 2 == 0)
	{
		left = philo->philosopher;
		right = philo->philosopher - 1;
	}
	pthread_mutex_lock(&(actions->mutex_fork[right]));
	print_this(start, philo->philosopher, TAKE_FORK);
	pthread_mutex_lock(&(actions->mutex_fork[left]));
	print_this(start, philo->philosopher, TAKE_FORK);
	is_eating(philo, start);
	pthread_mutex_unlock(&(actions->mutex_fork[right]));
	pthread_mutex_unlock(&(actions->mutex_fork[left]));
	return (1);
}

int	time_has_eat(t_philo *philo)
{
	if (actions->each_must_eat == -1)
		return (-1);
	pthread_mutex_lock(&(actions->mutex_eat));
	if (philo->has_eat == actions->each_must_eat)
	{
		if (actions->each_one != actions->nb_philosophers)
		{
			actions->each_one++;
			pthread_mutex_unlock(&(actions->mutex_eat));

			return (1);
		}
		if (actions->each_one == actions->nb_philosophers)
		{
			actions->each_one++;
			pthread_mutex_unlock(&(actions->mutex_eat));

			pthread_mutex_lock(&(actions->mutex_die));
			actions->are_alive = 0;
			pthread_mutex_unlock(&(actions->mutex_die));
			write(1, "All philos ate ", 15);
			write_nbr(philo->has_eat);
			write(1, " time\n", 6);
			return (1);
		}
	}
	pthread_mutex_unlock(&(actions->mutex_eat));
	return (0);
}

void	*is_it_dead(void *arg)
{
	long	actual_time;
	t_philo		*philo;
	long		start;

	philo = (t_philo*)arg;
	start = get_ms();
	while (1)
	{
		pthread_mutex_lock(&(actions->mutex_meal));
		actual_time = get_ms() - start - philo->last_meal;
		pthread_mutex_unlock(&(actions->mutex_meal));
		if (time_has_eat(philo) == 0)
			;
		else if (time_has_eat(philo) == 1 || !all_alive())
		{
			break ;
		}
		else if (actual_time >= (long)actions->tto_die)
		{
			print_this(start, philo->philosopher, DIED);
			pthread_mutex_lock(&(actions->mutex_die));
			actions->are_alive = 0;
			pthread_mutex_unlock(&(actions->mutex_die));
			break ;
		}
		usleep(100);
	}
	return (NULL);

}


void	*routine(void *arg)
{
	t_philo		*philo;
	pthread_t		thread;
	long		start;

	philo = (t_philo*)arg;
	pthread_create(&thread, 0, &is_it_dead, philo);
	start = get_ms();
	while (1)
	{
		if (!all_alive())
			break ;
		print_this(start, philo->philosopher, IS_THINKING);
		takes_forks_and_eat(philo, start);
		print_this(start, philo->philosopher, IS_SLEEPING);
		my_usleep(actions->tto_sleep, start);
	}
	pthread_join(thread, 0);
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