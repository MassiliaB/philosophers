#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# define IS_EATING "Is eating\n"
# define IS_THINKING "Is thinking\n"
# define IS_SLEEPING "Is sleeping\n"
# define IS_DEAD "Is dead\n"

typedef struct s_table
{
	int	fork;
	int philosopher;
}				t_table;

typedef struct s_global
{
	int	to_die;
	int	to_eat;
	int	to_sleep;
	int	each_must_eat;
	int	nb_philosophers;
	t_table	*philo;
}				t_global;


int	my_atoi(char *number);
int	clean_all(t_global *time);

#endif