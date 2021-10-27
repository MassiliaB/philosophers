/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:31:07 by masboula          #+#    #+#             */
/*   Updated: 2021/10/27 12:31:09 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	time_to_die(t_philo *philo, int actual_time)
{
	if (actual_time >= (long)philo->actions->tto_die)
	{
		if (philo->philosopher % 2 == 0
			< max(2 * philo->actions->tto_eat, philo->actions->tto_eat
				+ philo->actions->tto_sleep))
			return (1);
		if (philo->philosopher % 2 < philo->actions->tto_eat
			+ max(2 * philo->actions->tto_eat, philo->actions->tto_eat
				+ philo->actions->tto_sleep))
			return (1);
	}
	return (0);
}

void	*is_it_dead(void *arg)
{
	long	actual_time;
	t_philo	*philo;
	long	start;

	philo = (t_philo *)arg;
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
		else if (time_to_die(philo, actual_time))
		{
			print_this(philo, start, philo->philosopher, DIED);
			pthread_mutex_lock(&(philo->actions->mutex_die));
			philo->actions->are_alive = 0;
			pthread_mutex_unlock(&(philo->actions->mutex_die));
			break ;
		}
	}
	return (NULL);
}
