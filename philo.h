#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# define TAKE_FORK "has taken a fork"
# define IS_EATING "is eating"
# define IS_THINKING "is thinking"
# define IS_SLEEPING "is sleeping"
# define DIED "died"

struct s_actions *actions;

typedef struct s_actions
{
	int				tto_die;
	int				tto_eat;
	int				tto_sleep;
	int				each_must_eat;
	int				nb_philosophers;
	int				are_alive;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_meal;
	pthread_mutex_t	mutex_die;
	pthread_mutex_t	*mutex_fork;
	struct s_philo	*philo;	
}				t_actions;

typedef struct s_philo
{
	int		philosopher;
	long	last_meal;
	int		has_eat;
	long	start;
}				t_philo;

int		my_atoi(char *number);
int		clean_all();
void	write_str(char *str);
void	write_nbr(int nb);

#endif
