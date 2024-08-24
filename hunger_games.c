/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hunger_games.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 14:41:46 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 10:58:55 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_table(t_data *data)
{
	pthread_t	*th;
	long		i;

	i = -1;
	th = (pthread_t *)malloc(data->nbr * sizeof(pthread_t));
	if (!th)
		free_thread(data->philo, data, data->nbr, 0);
	data->threads = th;
	data->start_time = cur(NULL);
	while (++i < data->nbr)
	{
		if (pthread_create(&th[i], NULL, hunger_games, \
					(void *) data->philo[i]) != 0)
			free_thread(data->philo, data, 0, 3);
	}
	monitor(data);
	i = 0;
	while (i < data->nbr)
	{
		if (pthread_join(th[i++], NULL) != 0)
			free_thread(data->philo, data, 0, 4);
	}
	if (th != NULL)
		free(th);
	th = NULL;
}

void	philo_dead(t_thread *philo)
{
	if (pthread_mutex_lock(&philo->data->death) == 0)
	{
		if (philo->data->dead == 0)
		{
			philo->data->dead += 1;
			philo->data->whodied = philo->id - 1;
		}
		pthread_mutex_unlock(&philo->data->death);
	}
}

void	eat(t_thread *philo)
{
	long	flag1;
	long	flag2;

	flag1 = 0;
	flag2 = 0;
	while (flag1 == 0 && flag2 == 0)
	{
		flag1 = check_death(philo, 1);
		flag2 = check_full(philo->data);
		if (philo->id % 2 == 0)
		{
			if (philo->lock1 == 0)
				grab_fork(philo, 1);
			if (philo->lock2 == 0)
				grab_fork(philo, 2);
		}
		else
		{
			if (philo->lock2 == 0)
				grab_fork(philo, 2);
			if (philo->lock1 == 0)
				grab_fork(philo, 1);
		}
	}
	return ;
}

void	*hunger_games(void *info)
{
	t_data		*data;
	t_thread	*philo;

	philo = (t_thread *)info;
	data = philo->data;
	if (pthread_mutex_lock(&data->xie) == 0)
	{
		philo->start = cur(philo);
		pthread_mutex_unlock(&data->xie);
	}
	if (data->nbr == 1)
	{
		grab_fork(philo, 1);
		usleep(philo->data->hp * 1000);
		philo_dead(philo);
		pthread_mutex_lock(&philo->data->death);
		printf("%ld %ld has died\n", philo->start + philo->data->hp, philo->id);
		pthread_mutex_unlock(&philo->data->death);
	}
	else
		eat(philo);
	release_forks(philo);
	return (NULL);
}
