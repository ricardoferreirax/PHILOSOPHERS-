/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:49:15 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/02 12:03:26 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

int	start_philo_threads(t_sim *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_create(&table->philos[i].thread_id, NULL, philo_routine,
				&table->philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_sim	table;

	if (ac != 5 && ac != 6)
		return (show_usage_error(), 1);
	if (!validate_args(&table, ac, av))
		return (show_usage_error(), 1);
	if (!init_philos(&table))
		return (show_usage_error(), 1);
	if (!start_philo_threads(&table))
		return (show_usage_error(), 1);
	end_simulation(&table);
	return (0);
}
