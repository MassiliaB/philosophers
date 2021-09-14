#include "philo.h"

int	my_atoi(char *number)
{
	int			i;
	long int	res;

	i = 0;
	res = 0;
	if (number[i] == '-' || number[i] == '+')
		return (-1);
	while (number[i] >= '0' && number[i] <= '9')
	{
		res = res * 10 + (number[i] - 48);
		i++;
	}
	if (res > 2147483647)
		return (-1);
	return ((int)res);
}

int	clean_all(t_actions *actions)
{
	int	i;

	i = 0;
	if (actions->mutex_fork)
	{
		while (i < actions->nb_philosophers)
			pthread_mutex_destroy(&(actions->mutex_fork[i++]));
	}
	pthread_mutex_destroy(&(actions->mutex_die));
	pthread_mutex_destroy(&(actions->mutex_print));
//	pthread_mutex_destroy(&(actions->mutex_alive));
	if (actions->philo != NULL)
	{
		free(actions->philo);
	}
	return (0);
}