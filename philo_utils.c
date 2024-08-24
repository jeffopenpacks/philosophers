/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:48:38 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 08:48:56 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long int	negcheck(char c)
{
	if (c == '+')
		return (1);
	else
		return (-1);
}

long int	ft_atol(const char *str)
{
	long int	i;
	long int	negcounter;
	long int	result;

	i = 0;
	result = 0;
	negcounter = 1;
	while (str[i])
	{
		while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
			i++;
		if (str[i] == 45 || str[i] == 43)
		{
			negcounter *= negcheck(str[i]);
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9')
		{
			result = (result * 10) + (str[i] - '0');
			i++;
		}
		return (result *= negcounter);
	}
	return (0);
}

void	free_thread(t_thread **philo, t_data *data, long i, int option)
{
	long	j;

	j = 0;
	if (option == 0)
	{
		while (j < i)
			free(philo[j++]);
	}
	if (option > 0 && option <= 5)
	{
		while (j < data->nbr)
			free(philo[j++]);
	}
	if (data->threads != NULL)
	{
		free(data->threads);
		data->threads = NULL;
	}
	free(data->forks);
	free(philo);
	error_exit(option);
}

void	error_exit(int code)
{
	if (code == -1)
		perror("Please provide 4 or 5 arguments");
	if (code == -2)
		perror("Please provide numbers above 0 only");
	if (code == 0)
		perror("Malloc failure");
	if (code == 3)
		perror("Failure to create pthread");
	if (code == 4)
		perror("Failure to join pthread");
	if (code == 6)
		perror("Failure to init mutex");
	exit(EXIT_FAILURE);
}

int	announce(t_thread *philo, int option)
{
	if (check_death(philo, 1) != 0)
	{
		release_forks(philo);
		return (-1);
	}
	if (check_full(philo->data) != 0)
		return (-1);
	if (pthread_mutex_lock(&philo->data->death) == 0)
	{
		if (option == 1 && philo->data->dead == 0)
			printf("%ld %ld has taken a fork\n", cur(philo), philo->id);
		if (option == 2 && philo->data->dead == 0)
			printf("%ld %ld is sleeping\n", cur(philo), philo->id);
		if (option == 3 && philo->data->dead == 0)
			printf("%ld %ld is thinking\n", cur(philo), philo->id);
		if (option == 5 && philo->data->dead == 0)
			printf("%ld %ld is eating\n", cur(philo), philo->id);
		pthread_mutex_unlock(&philo->data->death);
	}
	return (0);
}
