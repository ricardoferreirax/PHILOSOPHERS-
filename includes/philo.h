/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmedeiro <rmedeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:51:12 by rmedeiro          #+#    #+#             */
/*   Updated: 2026/01/01 21:57:11 by rmedeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	int					philo_id;
	int					meals_eaten;
	long long			last_meal_time;
	t_mutex				*r_fork;
	t_mutex				*l_fork;
	t_mutex				last_meal;
	t_mutex				meal_count;
	pthread_t			thread_id;
	t_sim				*table;
}						t_philo;

typedef struct s_sim
{
	int					philo_count;
	int					eat_count;
	int					someone_died;
	long				tt_sleep;
	long				tt_die;
	long				tt_eat;
	long long			start_sim;
	int					full_philos;
	t_mutex				*forks;
	t_mutex				death_lock;
	t_mutex				print_lock;
	t_mutex				full_lock;
	pthread_t			death_monitor;
	t_philo				*philos;
}						t_sim;

void					*philo_routine(void *arg);
void					philo_usleep(long duration, t_sim *table);
long					current_timestamp(void);
int						cleanup_and_error(t_sim *table, char *str, int code);
void					philo_print_state(t_philo *philo, char *s);
int						init_philos(t_sim *table);
int						init_mutexes(t_sim *table);
void					end_simulation(t_sim *table);
void					*monitor(void *arg);

int						pick_forks_even(t_philo *philo);
int						pick_forks_odd(t_philo *philo);
void					unlock_forks_odd(t_philo *philo);
void					unlock_forks_even(t_philo *philo);

int						validate_numbers(int ac, char **av);

int						ft_isdigit(int c);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
long					ft_atol(const char *str);
void					ft_putstr_fd(char *s, int fd);
void					ft_bzero(void *str, size_t n);
void					*ft_memset(void *s, int c, size_t n);
void					*ft_calloc(size_t nmemb, size_t size);

#endif