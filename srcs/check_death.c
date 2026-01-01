/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:04:07 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:02:41 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void print_death(t_sim *table, int id, long now)
{
    pthread_mutex_lock(&table->print_lock);
    printf("%lld %d died\n", now - table->start_sim, id);
    pthread_mutex_unlock(&table->print_lock);
}

int	check_dead(t_sim *table, int i)
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
			print_death(table, table->philos[i].philo_id, now);
		}
		else
			pthread_mutex_unlock(&table->death_lock);
		return (0);
	}
	return (1);
}

int	check_full(t_sim *table)
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
			printf("All philos are Full\n");
			pthread_mutex_unlock(&table->print_lock);
			pthread_mutex_unlock(&table->full_lock);
			return (0);
		}
		pthread_mutex_unlock(&table->full_lock);
	}
	return (1);
}

void	*monitor(void *arg)
{
	int		i;
	t_sim	*table;

	table = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < table->philo_count)
		{
			if (!check_dead(table, i))
				return (NULL);
			i++;
		}
		if (!check_full(table))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
