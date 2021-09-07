#include "philo.h"

int	init(int ac, char **av, t_philo *time)
{
	if (ac < 5 || ac > 6)
		return (0);
	time->nb_philosophers =  my_atoi(av[1]);
	time->to_die = my_atoi(av[2]);
	time->to_eat =  my_atoi(av[3]);
	time->to_sleep =  my_atoi(av[4]);
	time->each_must_eat = 0;
	if (ac == 6)
		time->each_must_eat =  my_atoi(av[5]);
	if (time->nb_philosophers < 0 || time->to_die < 0 ||
		time->to_eat < 0 || time->to_sleep < 0 || time->each_must_eat < 0)
		return (0);
	return (1);
}

t_table	*the_table(t_philo *time)
{
	t_table	*philo;
	int		i;

	philo = malloc(sizeof(t_table) * time->nb_philosophers);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < time->nb_philosophers)
	{
		philo[i].philosopher = i + 1;
		philo[i].fork = 0;
		i++;
	}
	return (philo);
}

void	*routine(void *arg)
{
	t_table *philo;

	philo = (t_table*)arg;
	if (philo->fork == 0)
	{
		printf("holaa\n");
		pthread_mutex_lock(philo->mutex);
		philo->fork = 1;
		usleep(philo->time->to_eat);
		printf(".. %d %s\n", philo->philosopher, TAKE_FORK);
		if (philo->philosopher > 1)
			(philo - 1)->fork = 1;			
		pthread_mutex_unlock(philo->mutex);
		if (philo->philosopher > 1)
			(philo - 1)->fork = 0;			
		philo->fork = 0;
	}
	return (NULL);
}

void	simulation(t_philo *time)
{
	pthread_t		*thread;
	int				i;

	thread = malloc(sizeof(pthread_t) * time->nb_philosophers);
	i = 0;
	while (i < time->nb_philosophers)
	{
		if (pthread_create(&thread[i], NULL, &routine, time->philo + i))
			printf("errrrrror\n");
		i++;
	}
	while (i > 0)
	{
		pthread_join(thread[i], NULL);
		i--;
	}
}

int main(int ac, char **av)
{
	t_philo	time;

	if (!init(ac, av, &time))
		return (0);
	time.philo = the_table(&time);
	printf("phi %d, die %d, eat %d, sleep %d , MUST eat %d\n", time.nb_philosophers,time.to_die , time.to_eat, time.to_sleep , time.each_must_eat);

	simulation(&time);
	return (clean_all(&time));
}
	//	printf("holaa\n");
	//	pthread_join(thread, result of the fct sent to pthread creat);