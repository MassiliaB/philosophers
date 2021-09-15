#include "philo.h"


/*void	my_usleep(long time)
{
	long	start_time;

	start_time = get_ms();
	while ((get_ms() - start_time) < time)
		usleep(time);
}
int	my_usleep(int time_needed, t_datas *data)
{
	long	starting_time;
	long	elapsed_time;
	int		ret;

	elapsed_time = 0;
	starting_time = get_time_elapsed(data);
	while (elapsed_time < time_needed)
	{
		usleep(300);
		ret = check_death_variable(data);
		if (ret)
			return (ret);
		elapsed_time = get_time_elapsed(data) - starting_time;
	}
	return (SUCCESS);
}*/


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
		free(actions->mutex_fork);
	}
	pthread_mutex_destroy(&(actions->mutex_die));
	pthread_mutex_destroy(&(actions->mutex_print));
	if (actions->philo != NULL)
		free(actions->philo);
	return (0);
}