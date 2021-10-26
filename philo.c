#include "philo.h"

int	all_alive(t_philo *philo)
{
	pthread_mutex_lock(&(philo->actions->mutex_die));
	if (!philo->actions->are_alive)
	{
		pthread_mutex_unlock(&(philo->actions->mutex_die));
		return (0);
	}
	pthread_mutex_unlock(&(philo->actions->mutex_die));
	return (1);
}

void	is_eating(t_philo *philo, long start)
{
	print_this(philo, start, philo->philosopher, IS_EATING);
	pthread_mutex_lock(&(philo->actions->mutex_meal));
	philo->last_meal = get_ms() - start;
	pthread_mutex_unlock(&(philo->actions->mutex_meal));
	my_usleep(philo->actions->tto_eat, start);
	pthread_mutex_lock(&(philo->actions->mutex_eat));
	philo->has_eat += 1;
	pthread_mutex_unlock(&(philo->actions->mutex_eat));
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
	pthread_mutex_lock(&(philo->actions->mutex_fork[right]));
	print_this(philo, start, philo->philosopher, TAKE_FORK);
	pthread_mutex_lock(&(philo->actions->mutex_fork[left]));
	print_this(philo, start, philo->philosopher, TAKE_FORK);
	is_eating(philo, start);
	pthread_mutex_unlock(&(philo->actions->mutex_fork[right]));
	pthread_mutex_unlock(&(philo->actions->mutex_fork[left]));
	return (1);
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
		if (!all_alive(philo))
			break ;
		print_this(philo, start, philo->philosopher, IS_THINKING);
		takes_forks_and_eat(philo, start);
		print_this(philo, start, philo->philosopher, IS_SLEEPING);
		my_usleep(philo->actions->tto_sleep, start);
	}
	pthread_join(thread, 0);
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
		if (pthread_create(&thread[i], 0, &routine, (void *)(actions->philo + i)))
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
	t_actions	actions;

	if (!init_args(&actions, ac, av))
		return (0);
	the_philo(&actions);
	simulation(&actions);
	return (clean_all(&actions));
}