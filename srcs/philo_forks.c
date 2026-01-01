/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:42:28 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:43:04 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	unlock_forks_odd(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

int	pick_forks_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	philo_print_state(philo, "has taken a fork");
	pthread_mutex_lock(philo->r_fork);
	philo_print_state(philo, "has taken a fork");
	return (1);
}

void	unlock_forks_even(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

int	pick_forks_even(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	philo_print_state(philo, "has taken a fork");
	pthread_mutex_lock(philo->l_fork);
	philo_print_state(philo, "has taken a fork");
	return (1);
}
