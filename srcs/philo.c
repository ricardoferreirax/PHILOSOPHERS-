/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:49:15 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:23:38 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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

int	validate_args(t_sim *table, int ac, char **av)
{
	if (!validate_numbers(ac, av))
		return (0);
	table->philo_count = ft_atol(av[1]);
	table->tt_die = ft_atol(av[2]);
	table->tt_eat = ft_atol(av[3]);
	table->tt_sleep = ft_atol(av[4]);
	table->someone_died = 0;
	table->full_philos = 0;
	if (ac == 6)
		table->eat_count = ft_atol(av[5]);
	else
		table->eat_count = -1;
	table->forks = ft_calloc(table->philo_count, sizeof(t_mutex));
	if (!table->forks)
		return (cleanup_and_error(table, "Malloc Failed\n", 0));
	if (!setup_aux(table))
		return (0);
	table->start_sim = current_timestamp();
	return (1);
}

int	main(int ac, char **av)
{
	t_sim	table;
	int		i;

	i = -1;
	if (ac != 5 && ac != 6)
	{
		show_usage_error();
		return (1);
	}
	if (!validate_args(&table, ac, av))
	{
		ft_putstr_fd("Error: invalid arguments\n", 2);
		return (1);
	}
	if (!character_creation(&table))
		return (1);
	while (++i < table.philo_count)
	{
		pthread_create(&table.philos[i].thread_id, NULL, playthrough,
			&table.philos[i]);
	}
	pthread_life(&table);
	return (0);
}
