/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:30:15 by masboula          #+#    #+#             */
/*   Updated: 2021/10/27 12:30:31 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	int	left;
	int	right;

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
