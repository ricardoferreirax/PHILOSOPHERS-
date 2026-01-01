/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:59:25 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:52:42 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	end_simulation(t_sim *table)
{
	int	i;

	pthread_create(&table->death_monitor, NULL, monitor, table);
	pthread_join(table->death_monitor, NULL);
	i = 0;
	while (i < table->philo_count)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos[i].last_meal);
		pthread_mutex_destroy(&table->philos[i].meal_count);
		i++;
	}
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->death_lock);
	pthread_mutex_destroy(&table->full_lock);
	free(table->forks);
	free(table->philos);
}

int	init_mutexes(t_sim *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) < 0)
		{
			while (i > 0)
			{
				pthread_mutex_destroy(&table->forks[i - 1]);
				i--;
			}
			free(table->forks);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&table->print_lock, NULL) < 0)
		return (cleanup_and_error(table, "Init Failed\n", 1));
	if (pthread_mutex_init(&table->death_lock, NULL) < 0)
		return (cleanup_and_error(table, "Init Failed\n", 2));
	if (pthread_mutex_init(&table->full_lock, NULL) < 0)
		return (cleanup_and_error(table, "Init Failed\n", 3));
	return (1);
}

int	init_philos(t_sim *table)
{
	int	id;

	table->philos = ft_calloc(table->philo_count, sizeof(t_philo));
	if (!table->philos)
		return (cleanup_and_error(table, "Malloc Failed\n", 3));
	id = 0;
	while (id < table->philo_count)
	{
		table->philos[id].philo_id = id + 1;
		table->philos[id].table = table;
		table->philos[id].l_fork = &table->forks[id];
		table->philos[id].r_fork = &table->forks[(id + 1) % table->philo_count];
		table->philos[id].last_meal_time = table->start_sim;
		table->philos[id].meals_eaten = 0;
		if (pthread_mutex_init(&table->philos[id].last_meal, NULL) < 0)
			return (cleanup_and_error(table, "Init Failed\n", 4));
		if (pthread_mutex_init(&table->philos[id].meal_count, NULL) < 0)
			return (cleanup_and_error(table, "Init Failed\n", 5));
		id++;
	}
	return (1);
}
