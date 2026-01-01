/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:48:25 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:44:45 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_print_state(t_philo *philo, char *s)
{
	long	time;

	pthread_mutex_lock(&philo->table->death_lock);
	pthread_mutex_lock(&philo->table->print_lock);
	if (philo->table->someone_died)
	{
		pthread_mutex_unlock(&philo->table->death_lock);
		pthread_mutex_unlock(&philo->table->print_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->table->death_lock);
	time = current_timestamp() - philo->table->start_sim;
	printf("%ld %d %s\n", time, philo->philo_id, s);
	pthread_mutex_unlock(&philo->table->print_lock);
}

int	philo_pick_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->death_lock);
	if (philo->table->someone_died)
	{
		pthread_mutex_unlock(&philo->table->death_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->table->death_lock);
	if (philo->table->philo_count == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		philo_print_state(philo, "has taken a fork");
		philo_usleep(philo->table->tt_die * 2, philo->table);
		pthread_mutex_unlock(philo->l_fork);
		return (0);
	}
	if (philo->philo_id % 2 == 0)
		return (pick_forks_even(philo));
	return (pick_forks_odd(philo));
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal);
	philo->last_meal_time = current_timestamp();
	pthread_mutex_unlock(&philo->last_meal);
	pthread_mutex_lock(&philo->meal_count);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_count);
	philo_print_state(philo, "is eating");
	if (philo->table->eat_count > 0)
	{
		pthread_mutex_lock(&philo->meal_count);
		if (philo->meals_eaten == philo->table->eat_count)
		{
			pthread_mutex_unlock(&philo->meal_count);
			pthread_mutex_lock(&philo->table->full_lock);
			philo->table->full_philos++;
			pthread_mutex_unlock(&philo->table->full_lock);
		}
		else
			pthread_mutex_unlock(&philo->meal_count);
	}
	philo_usleep(philo->table->tt_eat, philo->table);
	if (philo->philo_id % 2 == 0)
		unlock_forks_even(philo);
	else
		unlock_forks_odd(philo);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 != 0 && philo->table->philo_count != 1)
		philo_usleep(philo->table->tt_eat / 2, philo->table);
	while (1)
	{
		if (!philo_pick_forks(philo))
			return (NULL);
		philo_eat(philo);
		pthread_mutex_lock(&philo->table->death_lock);
		if (philo->table->someone_died)
			return (pthread_mutex_unlock(&philo->table->death_lock), NULL);
		pthread_mutex_unlock(&philo->table->death_lock);
		philo_print_state(philo, "is sleeping");
		philo_usleep(philo->table->tt_sleep, philo->table);
		philo_print_state(philo, "is thinking");
		if (philo->table->philo_count % 2 != 0)
			philo_usleep((philo->table->tt_eat * 2) - philo->table->tt_sleep,
				philo->table);
		else
			philo_usleep(philo->table->tt_eat - philo->table->tt_sleep,
				philo->table);
	}
	return (NULL);
}
