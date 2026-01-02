/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:44:44 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/02 12:09:24 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

void	show_usage_error(void)
{
	ft_putstr_fd("====================================================\n", 2);
	ft_putstr_fd("                 PHILOSOPHERS ERROR              \n", 2);
	ft_putstr_fd("====================================================\n\n", 2);
	ft_putstr_fd("Incorrect number of arguments !!!\n", 2);
	ft_putstr_fd("The simulation requires the following format:\n\n", 2);
	ft_putstr_fd("./philo <nb_philos> <tt_die> <tt_eat> \n", 2);
	ft_putstr_fd("        <tt_sleep> [meals_count]\n\n ", 2);
	ft_putstr_fd("Example: ./philo 5 800 200 200 7\n\n", 2);
	ft_putstr_fd("Description:\n", 2);
	ft_putstr_fd(" - nb_philos: how many philos sit at the table\n", 2);
	ft_putstr_fd(" - tt_die (ms): max time a philo can go without eating\n", 2);
	ft_putstr_fd(" - tt_eat (ms): time spent eating\n", 2);
	ft_putstr_fd(" - tt_sleep (ms): time spent sleeping\n", 2);
	ft_putstr_fd(" - meals_count: stop when all philos ate this many times\n\n",
		2);
	ft_putstr_fd("====================================================\n", 2);
	ft_putstr_fd("            Simulation aborted. Try again.\n", 2);
	ft_putstr_fd("====================================================\n", 2);
}

long	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_usleep(long duration, t_sim *table)
{
	long long	start;

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

void	print_philo_death(t_sim *table, int id, long now)
{
	pthread_mutex_lock(&table->print_lock);
	printf("%lld %d died\n", now - table->start_sim, id);
	pthread_mutex_unlock(&table->print_lock);
}

void	philo_print_state(t_philo *philo, char *msg)
{
	long	time;
	char	*color;

	color = CLR_RESET;
	if (!ft_strncmp(msg, "has taken a fork", 17))
		color = CLR_FORK;
	else if (!ft_strncmp(msg, "is eating", 9))
		color = CLR_EAT;
	else if (!ft_strncmp(msg, "is sleeping", 11))
		color = CLR_SLEEP;
	else if (!ft_strncmp(msg, "is thinking", 11))
		color = CLR_THINK;
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
	printf("%s%ld %d %s%s\n", color, time, philo->philo_id, msg, CLR_RESET);
	pthread_mutex_unlock(&philo->table->print_lock);
}
