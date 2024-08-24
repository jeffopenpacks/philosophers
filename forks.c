/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:35:04 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 12:19:35 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	slumber(t_thread *ph)
{
	if (check_death(ph, 1) != 0 || check_full(ph->data) != 0)
		return ;
	if (announce(ph, 2) == 0)
		usleep(ph->data->slp * 1000);
	if (check_death(ph, 1) != 0 || check_full(ph->data) != 0)
		return ;
	if (announce(ph, 3) == 0)
	{
		usleep(1000);
		eat(ph);
	}
}

void	release_forks(t_thread *philo)
{
	if (philo->lock1 == 1)
	{
		pthread_mutex_unlock(philo->fork1);
		philo->lock1 = 0;
	}
	if (philo->lock2 == 1)
	{
		pthread_mutex_unlock(philo->fork2);
		philo->lock2 = 0;
	}
}

static void	eating(t_thread *philo)
{
	release_forks(philo);
	philo->meal -= 1;
	if (philo->meal == 0)
	{
		update_status(philo, 5);
		philo->meal -= 1;
	}
	if (check_full(philo->data) == 0)
		slumber(philo);
}

static void	can_i_eat(t_thread *philo)
{
	if (philo->lock1 == 1 && philo->lock2 == 1)
	{
		if (check_death(philo, 1) != 0 || check_full(philo->data) != 0)
			return ;
		if (announce(philo, 5) == 0)
		{
			if (pthread_mutex_lock(&philo->data->xie) == 0)
			{
				philo->eaten++;
				philo->start = cur(philo);
				pthread_mutex_unlock(&philo->data->xie);
			}
		}
		if (philo->data->eat < philo->data->hp)
			usleep(philo->data->eat * 1000);
		else
		{
			usleep(philo->data->hp * 1000);
			philo_dead(philo);
			return ;
		}
		eating(philo);
	}
}

void	grab_fork(t_thread *philo, int fork)
{
	pthread_mutex_t	*forks;
	int				flag;

	flag = 0;
	if (fork == 1)
		forks = philo->fork1;
	else
		forks = philo->fork2;
	while (flag == 0)
	{
		flag = check_death(philo, -1);
		if (pthread_mutex_lock(forks) == 0)
		{
			if (fork == 1)
				philo->lock1 = 1;
			else
				philo->lock2 = 1;
			if (announce(philo, 1) != 0)
				return ;
			else
				break ;
		}
	}
	can_i_eat(philo);
}
