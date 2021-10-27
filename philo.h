#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# define TAKE_FORK " has taken a fork\n"
# define IS_EATING " is eating\n"
# define IS_THINKING " is thinking\n"
# define IS_SLEEPING  " is sleeping\n"
# define DIED " died\n"
typedef struct s_philo
{
	int			philosopher;
	long		last_meal;
	int			has_eat;
	long		start;
	struct s_actions	*actions;	
}				t_philo;

typedef struct s_actions
{
	int				tto_die;
	int				tto_eat;
	int				tto_sleep;
	int				each_must_eat;
	int				nb_philosophers;
	int				are_alive;
	int				each_one;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_eat;
	pthread_mutex_t	mutex_meal;
	pthread_mutex_t	mutex_die;
	pthread_mutex_t	*mutex_fork;
	t_philo			*philo;	
}				t_actions;


int		my_atoi(char *number);
int		clean_all();
void	write_nbr(int nb);
int		ft_strlen(char *str);
int		nb_size(int nb);
void	the_philo(t_actions *actions);
int		init_args(t_actions *actions, int args, char **av);
int		init_mutex(t_actions *actions);
int		clean_all(t_actions *actions);
void	*is_it_dead(void *arg);
int		time_has_eat(t_philo *philo);
long	get_ms();
void	my_usleep(long time, long start);
void	buff_digit(int size, long nb, int index, char *buff);
void	buff_this(char *buff, long start, int philosopher, char *is_doing);
void	print_this(t_philo *philo, long start, int philosopher, char *is_doing);
int		all_alive(t_philo *philo);
int		takes_forks_and_eat(t_philo *philo, long start);
int		max(int a, int b);

#endif
