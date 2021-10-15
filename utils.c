
# include <unistd.h>

void	write_str(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	write(1, str, i);
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
