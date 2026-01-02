/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:04:07 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 22:07:05 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	check_philo_death(t_sim *table, int i)
{
	long		now;
	long long	last_meal;

	pthread_mutex_lock(&table->philos[i].last_meal);
	last_meal = table->philos[i].last_meal_time;
	pthread_mutex_unlock(&table->philos[i].last_meal);
	now = current_timestamp();
	if ((long long)now - last_meal >= (long long)table->tt_die)
	{
		pthread_mutex_lock(&table->death_lock);
		if (!table->someone_died)
		{
			table->someone_died = 1;
			pthread_mutex_unlock(&table->death_lock);
			print_philo_death(table, table->philos[i].philo_id, now);
		}
		else
			pthread_mutex_unlock(&table->death_lock);
		return (0);
	}
	return (1);
}

static int	check_all_philos_full(t_sim *table)
{
	if (table->eat_count > 0)
	{
		pthread_mutex_lock(&table->full_lock);
		if (table->full_philos == table->philo_count)
		{
			pthread_mutex_lock(&table->death_lock);
			if (!table->someone_died)
				table->someone_died = 1;
			pthread_mutex_unlock(&table->death_lock);
			pthread_mutex_lock(&table->print_lock);
			printf("All philos have eaten the required number of meals\n");
			pthread_mutex_unlock(&table->print_lock);
			pthread_mutex_unlock(&table->full_lock);
			return (0);
		}
		pthread_mutex_unlock(&table->full_lock);
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	int		i;
	t_sim	*table;

	table = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < table->philo_count)
		{
			if (!check_philo_death(table, i))
				return (NULL);
			i++;
		}
		if (!check_all_philos_full(table))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
