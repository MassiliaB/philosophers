/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:30:15 by masboula          #+#    #+#             */
/*   Updated: 2021/10/28 17:30:21 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*void	all_ate(t_philo *philo)
{
	philo->actions->each_one++;
//	pthread_mutex_unlock(&(philo->actions->mutex_eat));
	

	pthread_mutex_lock(&(philo->actions->mutex_meal));	

	pthread_mutex_unlock(&(philo->actions->mutex_meal));
}*/

void	is_eating(t_philo *philo, long start)
{
	print_this(philo, start, philo->philosopher, IS_EATING);

	pthread_mutex_lock(&(philo->actions->mutex_meal));
	philo->last_meal = get_ms() - start;
	pthread_mutex_unlock(&(philo->actions->mutex_meal));

	my_usleep(philo->actions->tto_eat, start);

	philo->has_eat += 1;
//	pthread_mutex_lock(&(philo->actions->mutex_eat));
//	pthread_mutex_unlock(&(philo->actions->mutex_eat));
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
/*	if ( philo->philosopher == 5)
	{
		printf("phi = %d , ri %d, lef %d\n", philo->philosopher, right , left);
	}*/
	pthread_mutex_lock(&(philo->actions->mutex_fork[right]));
	print_this(philo, start, philo->philosopher, TAKE_FORK);
	pthread_mutex_lock(&(philo->actions->mutex_fork[left]));
	print_this(philo, start, philo->philosopher, TAKE_FORK);
	is_eating(philo, start);
	pthread_mutex_unlock(&(philo->actions->mutex_fork[right]));
	pthread_mutex_unlock(&(philo->actions->mutex_fork[left]));
	return (1);
}