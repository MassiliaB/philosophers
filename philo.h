#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# define TAKE_FORK "has taken a fork\n"
# define IS_EATING "is eating\n"
# define IS_THINKING "is thinking\n"
# define IS_SLEEPING "is sleeping\n"
# define DIED "died\n"

typedef struct s_philo
{
	int					philosopher;
	int					alive;
	long				last_meal;
	struct s_actions	*actions;
}				t_philo;

typedef struct s_actions
{
	int				tto_die;
	int				tto_eat;
	int				tto_sleep;
	int				each_must_eat;
	int				nb_philosophers;
	long			beginning;
	int				is_alive;
	pthread_mutex_t	mutex_die;
	pthread_mutex_t	*mutex_fork;
	t_philo			*philo;
	struct timeval	start;
}				t_actions;

int		my_atoi(char *number);
int		clean_all(t_actions *time);

#endif