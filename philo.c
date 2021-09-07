#include "philo.h"

t_global	*init(int ac, char **av, t_global *time)
{
	if (ac < 5 || ac > 6)
		return (NULL);
	time->nb_philosophers =  my_atoi(av[1]);
	time->to_die = my_atoi(av[2]);
	time->to_eat =  my_atoi(av[3]);
	time->to_sleep =  my_atoi(av[4]);
	time->each_must_eat = 0;
	if (ac == 6)
		time->each_must_eat =  my_atoi(av[5]);
	if (time->nb_philosophers < 0 || time->to_die < 0 ||
		time->to_eat < 0 || time->to_sleep < 0 || time->each_must_eat < 0)
		return (NULL);
	return (time);
}

t_table	*the_table(t_global *time)
{
	t_table	*philo;
	int		i;

	philo = malloc(sizeof(t_table) * time->nb_philosophers);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < time->nb_philosophers)
	{
		philo[i].philosopher = i;
		philo[i].fork = 1;
		i++;
	}
	return (philo);
}

void	*routine()
{

	printf("Takes the fork\n");
	printf("%s", IS_EATING);
	printf("%s", IS_THINKING);
	printf("%s", IS_SLEEPING);
	printf("%s", IS_DEAD);
	printf("Puts down the fork\n");
	return (0);
}

int main(int ac, char **av)
{
	t_global	time;

	init(ac, av, &time);
	time.philo = the_table(&time);
printf("phi %d, die %d, eat %d, sleep %d , MUST eat %d\n", time.nb_philosophers,time.to_die , time.to_eat, time.to_sleep , time.each_must_eat);

	return (clean_all(&time));
}
	//	printf("holaa\n");