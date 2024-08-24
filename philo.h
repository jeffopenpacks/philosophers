/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 15:00:04 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 08:04:28 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>

struct	s_thread;

typedef struct s_data{
	long				nbr;
	int					dead;
	long				hp;
	long				eat;
	long				slp;
	long				meal;
	long				meals_left;
	long				full;
	pthread_mutex_t		death;
	pthread_mutex_t		xie;
	pthread_mutex_t		*forks;
	pthread_t			*threads;
	int					whodied;
	int					cut;
	struct s_thread		**philo;
	long				start_time;
}	t_data;

typedef struct s_thread{
	long			id;
	pthread_mutex_t	*fork1;
	int				lock1;
	pthread_mutex_t	*fork2;
	int				lock2;
	long			start;
	long			meal;
	long			eaten;
	t_data			*data;
}	t_thread;

int			main(int argc, char **argv);
long		cur(t_thread *philo);
long		check_death(t_thread *philo, int option);
long		check_full(t_data *data);
void		*monitor(void *info);
long int	ft_atol(const char *str);
void		free_thread(t_thread **philo, t_data *data, long i, int option);
int			announce(t_thread *philo, int option);
long		update_status(t_thread *philo, int option);
void		set_table(t_data *data);
void		*hunger_games(void *info);
void		grab_fork(t_thread *philo, int fork);
void		slumber(t_thread *philo);
void		eat(t_thread *philo);
void		philo_dead(t_thread *philo);
void		error_exit(int code);
void		release_forks(t_thread *philo);
void		wrap_up(t_data *data);

#endif
