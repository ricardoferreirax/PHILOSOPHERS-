/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:44:44 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:19:11 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long current_timestamp(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void philo_usleep(long duration, t_sim *table)
{
    long long start;

    if (duration <= 0)
        return ;
    start = current_timestamp();
    while ((current_timestamp() - start) < duration)
    {
        pthread_mutex_lock(&table->death_lock);
        if (table->someone_died)
        {
            pthread_mutex_unlock(&table->death_lock);
            break ;
        }
        pthread_mutex_unlock(&table->death_lock);
        usleep(100);
    }
}
