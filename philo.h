/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:12:59 by wtang             #+#    #+#             */
/*   Updated: 2026/01/16 16:10:15 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data t_data;

typedef struct s_philo
{
	int	p_id;
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	max_pastas;
	int	pastas_eaten;
	long long	last_pasta_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data	*data;
}	t_philo;

typedef struct s_data
{
	int	philo_count;
	int	*pastas_count;
	int	finished_count;
	int	stop_simulation;
	int	pasta_mutex_initialized;
	int	finish_mutex_initialized;
	int	print_mutex_initialized;
	long long	start_time;
	pthread_t	*threads;
	pthread_t	death_checker_thread;
	pthread_mutex_t	*forks;
	pthread_mutex_t	pasta_mutex;
	pthread_mutex_t	finish_mutex;
	pthread_mutex_t	print_mutex;
	t_philo	*philos;
}	t_data;

/* philo.c */
void		print_status(t_philo *philo, const char *status);
int			ft_create_threads(t_data *data);

/* init.c */
int			ft_init_simulation(t_data *data, t_philo *main_philo);
int			ft_init_philo(int ac, char **av, t_philo *main_philo);
int			ft_check_args(int ac, char *av[]);

/* routine.c */
void		*ft_routine(void *arg);

/*routine_utils.c*/
void		ft_assign_fork_order(t_philo *philo, pthread_mutex_t **first,
			pthread_mutex_t **second);
void		ft_apply_initial_delay(t_philo *philo);

/* monitor.c */
int			must_stop(t_data *data);
void		mark_philo_finished(t_data *data, int id);
void		*death_checker(void *arg);

/* utils.c */
void		ft_clean_up(t_data *data);
long long	ft_get_time(void);
int			ft_atoi(const char *nptr);
int			ft_isdigit(int c);

#endif