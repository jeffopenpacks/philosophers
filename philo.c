/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joolim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:57:05 by joolim            #+#    #+#             */
/*   Updated: 2024/04/04 11:34:59 by joolim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_data(int argc, char **argv, t_data *data)
{
	data->nbr = ft_atol(argv[1]);
	if (data->nbr == 0)
		error_exit(-2);
	data->hp = ft_atol(argv[2]);
	data->eat = ft_atol(argv[3]);
	data->slp = ft_atol(argv[4]);
	data->dead = 0;
	data->full = 0;
	data->cut = 0;
	data->meal = -1;
	if (argc == 6)
		data->meal = ft_atol(argv[5]);
}

static void	check_input(int argc, char **argv, t_data *data)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (argc < 5 || argc > 6)
		error_exit(-1);
	while (i < argc)
	{
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				error_exit(-2);
			else
				j++;
		}
		i++;
		j = 0;
	}
	init_data(argc, argv, data);
}

static void	init_philo(t_thread **philo, t_data *data)
{
	long	i;
	long	j;

	i = 0;
	j = 0;
	while (i < data->nbr)
	{
		philo[i] = (t_thread *)malloc(sizeof(t_thread));
		if (!(philo[i]))
			free_thread(philo, data, i, 0);
		philo[i]->id = i + 1;
		j = philo[i]->id % data->nbr;
		philo[i]->fork1 = &data->forks[i];
		philo[i]->fork2 = &data->forks[j];
		philo[i]->data = data;
		philo[i]->meal = data->meal;
		philo[i]->lock1 = 0;
		philo[i]->lock2 = 0;
		philo[i]->start = 0;
		philo[i]->eaten = 0;
		i++;
	}
	data->philo = philo;
}

static void	init_misc(t_data *data, pthread_mutex_t *forks)
{
	long	i;

	i = 0;
	if (pthread_mutex_init(&data->death, NULL) != 0)
		free_thread(data->philo, data, 0, 6);
	if (pthread_mutex_init(&data->xie, NULL) != 0)
	{
		pthread_mutex_destroy(&data->death);
		free_thread(data->philo, data, 0, 6);
	}
	while (i < data->nbr)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	data->forks = forks;
}

int	main(int argc, char **argv)
{
	t_data				data;
	t_thread			**philo;
	pthread_mutex_t		*forks;

	check_input(argc, argv, &data);
	philo = (t_thread **)malloc(data.nbr * sizeof(t_thread *));
	if (!philo)
		error_exit(0);
	forks = (pthread_mutex_t *)malloc(data.nbr * sizeof(pthread_mutex_t));
	if (!forks)
	{
		free(philo);
		error_exit(0);
	}
	init_misc(&data, forks);
	init_philo(philo, &data);
	set_table(&data);
	wrap_up(&data);
	return (0);
}
