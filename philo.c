/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masboula <masboula@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:31:01 by masboula          #+#    #+#             */
/*   Updated: 2021/10/28 17:30:12 by masboula         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000);
}

void	my_usleep(long time, long start)
{
	long int	time_before;
	long int	time_actual;

	time_before = get_ms() - start;
	time_actual = 0;
	while ((get_ms() - start) - time_before < time)
		usleep(10);
}

void	*routine(void *arg)
{
	t_philo		*philo;
	pthread_t	thread;
	long		start;

	philo = (t_philo *)arg;
	pthread_create(&thread, 0, &is_it_dead, philo);
	//		printf("phi = %d \n", philo->philosopher);
	start = get_ms();
	while (1)
	{
		if (philo->actions->each_must_eat < 0 && !all_alive(philo))
			break ;
		print_this(philo, start, philo->philosopher, IS_THINKING);
		takes_forks_and_eat(philo, start);
		if (philo->has_eat == philo->actions->each_must_eat)
		{
		//	printf("hello\n");
			everyone_done_eat(philo, 1);
			break ;
		}
		print_this(philo, start, philo->philosopher, IS_SLEEPING);
		my_usleep(philo->actions->tto_sleep, start);
	}
//	printf("bye phi = %d \n", philo->philosopher);
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
		if (pthread_create(&thread[i], 0, &routine, actions->philo + i))
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
/*				write(1, "All philos ate ", 15);
				write_nbr(philo->has_eat);
				write(1, " time\n", 6);
*/