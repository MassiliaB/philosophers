/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:31:07 by masboula          #+#    #+#             */
/*   Updated: 2021/10/28 17:33:24 by masboula         ###   ########.fr       */
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

void	someone_is_dead_stop(t_philo *philo, int i, long start)
{
	if (i)
		print_this(philo, start, philo->philosopher, DIED);
//	printf("--bye phi = %d \n", philo->philosopher + 1);
	pthread_mutex_lock(&(philo->actions->mutex_die));
	philo->actions->are_alive = 0;
	pthread_mutex_unlock(&(philo->actions->mutex_die));
}

int	everyone_done_eat(t_philo *philo, int i, long start)
{
	pthread_mutex_lock(&(philo->actions->mutex_each));
	if (i == 1)
	{
		philo->actions->each_one++;
	}
	else if (all_alive(philo) && philo->actions->each_one == philo->actions->nb_philosophers)
	{
		someone_is_dead_stop(philo, 0, start);
		pthread_mutex_unlock(&(philo->actions->mutex_each));
		return (1);
	}
	pthread_mutex_unlock(&(philo->actions->mutex_each));
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

		if (philo->actions->each_must_eat > 0)
		{
			if (everyone_done_eat(philo, 0, start))
			{
				printf("All philo ate %d time\n", philo->has_eat);
				//if (all_alive(philo))
				break ;
			}
		}	
		else if (!all_alive(philo))
			break ;
		else if (all_alive(philo) && actual_time >= (long)philo->actions->tto_die)
		{
			someone_is_dead_stop(philo, 1, start);
			break ;
		}
	}
	return (NULL);
}
