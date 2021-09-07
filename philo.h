#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# define TAKE_FORK "has taken a fork\n"
# define IS_EATING "is eating\n"
# define IS_THINKING "is thinking\n"
# define IS_SLEEPING "is sleeping\n"
# define DIED "died\n"

typedef struct s_table
{
	int				fork;
	int				philosopher;
	pthread_mutex_t	*mutex;
	struct s_philo			*time;
}				t_table;

typedef struct s_philo
{
	int		to_die;
	int		to_eat;
	int		to_sleep;
	int		each_must_eat;
	int		nb_philosophers;
	t_table	*philo;
}				t_philo;

int		my_atoi(char *number);
int		clean_all(t_philo *time);

#endif