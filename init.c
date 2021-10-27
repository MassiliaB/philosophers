/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:31:30 by masboula          #+#    #+#             */
/*   Updated: 2021/10/27 12:31:31 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	clean_all(t_actions *actions)
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

int	init_mutex(t_actions *actions)
{
	int	i;

	actions->mutex_fork = malloc(sizeof(pthread_mutex_t)
			* (actions->nb_philosophers + 1));
	if (!actions->mutex_fork)
		return (0);
	i = 0;
	while (i <= actions->nb_philosophers)
	{
		if (pthread_mutex_init(&(actions->mutex_fork[i]), 0))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&actions->mutex_die, 0))
		return (0);
	if (pthread_mutex_init(&(actions->mutex_print), 0))
		return (0);
	if (pthread_mutex_init(&(actions->mutex_meal), 0))
		return (0);
	if (pthread_mutex_init(&(actions->mutex_eat), 0))
		return (0);
	return (1);
}

int	init_args(t_actions *actions, int args, char **av)
{
	if (args < 5 || args > 6)
		return (0);
	actions->nb_philosophers = my_atoi(av[1]);
	actions->tto_die = my_atoi(av[2]);
	actions->tto_eat = my_atoi(av[3]);
	actions->tto_sleep = my_atoi(av[4]);
	actions->each_must_eat = -1;
	actions->each_one = 0;
	actions->are_alive = 1;
	if (args == 6)
	{
		actions->each_must_eat = my_atoi(av[5]);
		if (actions->each_must_eat <= 0)
			return (0);
	}
	if (actions->nb_philosophers <= 0 || actions->tto_die <= 0
		|| actions->tto_eat <= 0 || actions->tto_sleep <= 0)
		return (0);
	if (!init_mutex(actions))
		return (0);
	return (1);
}

void	the_philo(t_actions *actions)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * actions->nb_philosophers);
	if (!philo)
		return ;
	i = 0;
	while (i < actions->nb_philosophers)
	{
		philo[i].philosopher = i + 1;
		philo[i].has_eat = 0;
		philo[i].last_meal = 0;
		philo[i].actions = actions;
		i++;
	}
	actions->philo = philo;
}
