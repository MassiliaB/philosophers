#include "philo.h"

void	buff_digit(int size, long nb, int index, char *buff)
{
	int	i;

	i = 0;
	if (nb == 0)
		buff[i++] = '0';
	while (i < size)
	{
		buff[index + (size - i)] = nb % 10 + '0';
		nb /= 10;
		i++;
	}
	buff[index + size + i] = '\0';
}

void	buff_this(char *buff, long start, int philosopher, char *is_doing)
{
	int		len;
	int		i;
	long	time;

	time = get_ms() - start;
	buff_digit(nb_size(time), time, -1, buff);
	len = nb_size(time);
	buff[len++] = 'm';
	buff[len++] = 's';
	buff[len] = ' ';
	buff_digit(nb_size(philosopher), philosopher, len, buff);
	len += nb_size(philosopher) + 1;
	i = -1;
	while (is_doing[++i])
		buff[len + i] = is_doing[i];
	buff[len + i] = '\0';
}

void	print_this(t_philo *philo, long start, int philosopher, char *is_doing)
{
	char	to_print[10000];

	pthread_mutex_lock(&philo->actions->mutex_print);
	if (!all_alive(philo))
	{
		pthread_mutex_unlock(&(philo->actions->mutex_print));
		return;
	}
	buff_this(to_print, start, philosopher, is_doing);
	write(1, &to_print, ft_strlen(to_print));
	pthread_mutex_unlock(&(philo->actions->mutex_print));
	return ;
}