/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:59:25 by rmedeiro          #+#    #+#             */
/*   Updated: 2025/12/11 15:55:14 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	init_philos(t_sim *sim, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < sim->philo_count)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (0);
		philos[i].philo_id = i + 1;
		philos[i].tt_eat = sim->tt_eat;
		philos[i].tt_sleep = sim->tt_sleep;
		philos[i].l_fork = &sim->forks[i];
		philos[i].r_fork = &sim->forks[(i + 1) % sim->philo_count];
		philos[i].start_sim = sim->start_sim;
		philos[i].last_meal_time = sim->start_sim;
		philos[i].meals_eaten = 0;
		philos[i].table = sim;
		if (pthread_mutex_init(&philos[i].last_meal, NULL) != 0)
			return (0);
		if (pthread_mutex_init(&philos[i].meal_count, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int allocate_and_init_structs(t_sim *sim)
{
    t_philo *philo_arr;

    if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&sim->death_lock, NULL) != 0)
        return (pthread_mutex_destroy(&sim->print_lock), 0);
    sim->forks = malloc(sim->philo_count * sizeof(t_mutex));
    if (!sim->forks)
        return (pthread_mutex_destroy(&sim->print_lock),
		pthread_mutex_destroy(&sim->death_lock), 0);
    philo_arr = malloc(sim->philo_count * sizeof(t_philo));
    if (!philo_arr)
        return (free(sim->forks), pthread_mutex_destroy(&sim->print_lock),
                pthread_mutex_destroy(&sim->death_lock), 0);
    if (!init_philos(sim, philo_arr))
    {
        free(philo_arr);
        free(sim->forks);
        pthread_mutex_destroy(&sim->print_lock);
        pthread_mutex_destroy(&sim->death_lock);
        return (0);
    }
    sim->philos = philo_arr;
    return (1);
}

static int	start_threads(t_sim *sim)
{
	int i;
	int created;

	i = 0;
	created = 0;
	while (i < sim->philo_count)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL,
				philo_routine, &sim->philos[i]) != 0)
			return (cleanup_simulation(sim, created), 0);
		created++;
		usleep(100);
		i++;
	}
	if (pthread_create(&sim->death_monitor, NULL, death_monitor, sim) != 0)
		return (cleanup_simulation(sim, created), 0);
	return (1);
}

int	init_simulation(t_sim *sim)
{
	if (!allocate_and_init_structs(sim))
		return (0);
	if (!start_threads(sim))
		return (0);
	return (1);
}
