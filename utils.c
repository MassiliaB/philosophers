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

int	clean_all(t_philo *time)
{
	int	i;

	if (time->philo != NULL)
	{
		i = 0;
	//	while (time->p)
			free(time->philo);
	}
	return (0);
}