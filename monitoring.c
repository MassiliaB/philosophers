#include "philo.h"

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

void	all_ate(t_philo *philo)
{
	philo->actions->each_one++;
	pthread_mutex_unlock(&(philo->actions->mutex_eat));
	pthread_mutex_lock(&(philo->actions->mutex_die));
	philo->actions->are_alive = 0;
	pthread_mutex_unlock(&(philo->actions->mutex_die));
	write(1, "All philos ate ", 15);
	write_nbr(philo->has_eat);
	write(1, " time\n", 6);
}

int	time_has_eat(t_philo *philo)
{
	if (philo->actions->each_must_eat == -1)
		return (-1);
	pthread_mutex_lock(&(philo->actions->mutex_eat));
	if (philo->has_eat == philo->actions->each_must_eat)
	{
		if (philo->actions->each_one != philo->actions->nb_philosophers)
		{
			philo->actions->each_one++;
			pthread_mutex_unlock(&(philo->actions->mutex_eat));
			return (1);
		}
		if (philo->actions->each_one == philo->actions->nb_philosophers)
		{
			all_ate(philo);
			return (1);
		}
	}
	pthread_mutex_unlock(&(philo->actions->mutex_eat));
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
		pthread_mutex_lock(&(philo->actions->mutex_meal));
		actual_time = get_ms() - start - philo->last_meal;
		pthread_mutex_unlock(&(philo->actions->mutex_meal));
		if (time_has_eat(philo) == 0)
			;
		else if (time_has_eat(philo) == 1 || !all_alive(philo))
			break ;
		else if (actual_time >= (long)philo->actions->tto_die)
		{
			print_this(philo, start, philo->philosopher, DIED);
			pthread_mutex_lock(&(philo->actions->mutex_die));
			philo->actions->are_alive = 0;
			pthread_mutex_unlock(&(philo->actions->mutex_die));
			break ;
		}
		usleep(100);
	}
	return (NULL);
}