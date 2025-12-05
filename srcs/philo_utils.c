/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:44:44 by rmedeiro          #+#    #+#             */
/*   Updated: 2025/12/05 05:05:53 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	current_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(long long duration, t_philo *philo)
{
	long long	start;

	start = current_time_ms();
	while (current_time_ms() < start + duration)
	{
		pthread_mutex_lock(&philo->table->death_lock);
		if (philo->table->someone_died == 1)
		{
			pthread_mutex_unlock(&philo->table->death_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->table->death_lock);
		usleep(500);
	}
}

int	print_status(t_philo *philo, const char *msg)
{
	long long	now;

	pthread_mutex_lock(&philo->table->death_lock);
	if (philo->table->someone_died)
	{
		pthread_mutex_unlock(&philo->table->death_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->table->death_lock);
	pthread_mutex_lock(&philo->table->print_lock);
	now = current_time_ms();
	if (!philo->table->someone_died)
		printf("%lld %d %s\n", now - philo->start_sim, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->table->print_lock);
	return (1);
}
