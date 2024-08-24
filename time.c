/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 12:02:25 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 12:17:21 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	cur(t_thread *philo)
{
	struct timeval	tv;
	long			time;

	time = 0;
	gettimeofday(&tv, NULL);
	if (!philo)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	else
	{
		time = philo->data->start_time;
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - time);
	}
}

void	wrap_up(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr)
		pthread_mutex_destroy(&data->forks[i++]);
	free(data->forks);
	data->forks = NULL;
	i = 0;
	while (i < data->nbr)
	{
		free(data->philo[i]);
		data->philo[i] = NULL;
		i++;
	}
	free(data->philo);
	data->philo = NULL;
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->xie);
	printf("Simulation ended\n");
}

long	update_status(t_thread *philo, int option)
{
	long	res;

	res = 0;
	if (pthread_mutex_lock(&philo->data->xie) == 0)
	{
		if (option == 3 && philo->data->meals_left > 0)
			philo->data->meals_left -= 1;
		else if (option == 4)
		{
			pthread_mutex_unlock(&philo->data->xie);
			return (philo->data->full);
		}
		else if (option == 5)
			philo->data->full += 1;
		res = philo->data->meals_left;
		pthread_mutex_unlock(&philo->data->xie);
	}
	return (res);
}
