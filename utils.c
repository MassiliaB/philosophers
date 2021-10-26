# include "philo.h"

int	nb_size(int nb)
{
	int size;

	size = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= 10;
		size++;
	}
	return (size);
}

int	ft_strlen(char *str)
{
	int l;

	l = 0;
	while(str[l])
		l++;
	return (l);
}

void	write_nbr(int nb)
{
	long	n;
	char	c;

	n = nb;
	if (n < 0)
	{
		write(1, "-", 1);
		n = n * -1;
	}
	if (n > 9)
	{
		write_nbr(n / 10);
		write_nbr(n % 10);
	}
	else
	{
		c = n + '0';
		write(1, &c, 1);
	}
}

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
