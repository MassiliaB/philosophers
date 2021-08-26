#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>

typedef struct s_table
{
    int fork;
    int philosopher;
    typedef struct *s_table;
}               t_table;

#endif