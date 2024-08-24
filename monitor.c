/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:49:03 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 10:29:25 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	check_death(t_thread *philo, int option)
{
	long	time;

	if (pthread_mutex_lock(&philo->data->death) == 0)
	{
		if (option == -1 && philo->data->dead != 0)
		{
			pthread_mutex_unlock(&philo->data->death);
			return (-1);
		}
		if (option == 1 && philo->data->dead != 0)
		{
			release_forks(philo);
			if (philo->id == philo->data->whodied + 1 && philo->data->cut == 0)
			{
				time = philo->start + philo->data->hp;
				printf("%ld %ld has died\n", time, philo->id);
				printf("Simulation ending...\n");
				philo->data->cut = 1;
			}
			pthread_mutex_unlock(&philo->data->death);
			return (-2);
		}
		pthread_mutex_unlock(&philo->data->death);
	}
	return (0);
}

static long	check_hp(t_data *data)
{
	long	i;

	i = -1;
	while (++i < data->nbr)
	{
		if (check_death(data->philo[i], -1) != 0)
			return (-1);
		if (pthread_mutex_lock(&data->xie) == 0)
		{
			if ((cur(data->philo[i]) - data->philo[i]->start > data->hp))
			{
				pthread_mutex_unlock(&data->xie);
				if (pthread_mutex_lock(&data->death) == 0)
				{
					data->whodied = i;
					data->dead += 1;
					pthread_mutex_unlock(&data->death);
					return (-1);
				}
			}
			else
				pthread_mutex_unlock(&data->xie);
		}
	}
	return (0);
}

long	check_full(t_data *data)
{
	int	res;

	res = 0;
	if (pthread_mutex_lock(&data->xie) == 0)
	{
		if (data->full == data->nbr)
			res = -1;
		pthread_mutex_unlock(&data->xie);
	}
	return (res);
}

void	*monitor(void *info)
{
	t_data	*data;

	data = (t_data *) info;
	while (1)
	{
		if (check_hp(data) != 0)
			break ;
		if (check_full(data) != 0)
			break ;
	}
	return (NULL);
}
