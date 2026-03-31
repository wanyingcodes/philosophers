/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 18:20:00 by wtang             #+#    #+#             */
/*   Updated: 2026/01/14 13:29:40 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&data->pasta_mutex, NULL) != 0)
		return (0);
	data->pasta_mutex_initialized = 1;
	if (pthread_mutex_init(&data->finish_mutex, NULL) != 0)
		return (0);
	data->finish_mutex_initialized = 1;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	data->print_mutex_initialized = 1;
	return (1);
}

static void	ft_assign_philos(t_data *data, t_philo *main_philo)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i] = *main_philo;
		data->philos[i].p_id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philo_count];
		data->philos[i].data = data;
		i++;
	}
}

int	ft_init_simulation(t_data *data, t_philo *main_philo)
{
	memset(data, 0, sizeof(t_data));
	data->philo_count = main_philo->number_of_philosophers;
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	data->pastas_count = malloc(sizeof(int) * data->philo_count);
	data->threads = malloc(sizeof(pthread_t) * data->philo_count);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->philos || !data->pastas_count || !data->threads || !data->forks)
	{
		free(data->philos);
		free(data->pastas_count);
		free(data->threads);
		free(data->forks);
		return (0);
	}
	memset(data->pastas_count, 0, sizeof(int) * data->philo_count);
	if (!ft_init_mutexes(data))
		return (ft_clean_up(data), 0);
	ft_assign_philos(data, main_philo);
	return (1);
}

int	ft_init_philo(int ac, char **av, t_philo *main_philo)
{
	memset(main_philo, 0, sizeof(t_philo));
	main_philo->number_of_philosophers = ft_atoi(av[1]);
	main_philo->time_to_die = ft_atoi(av[2]);
	main_philo->time_to_eat = ft_atoi(av[3]);
	main_philo->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		main_philo->max_pastas = ft_atoi(av[5]);
	else
		main_philo->max_pastas = -1;
	if (main_philo->time_to_die <= 0 || main_philo->time_to_eat <= 0
		|| main_philo->time_to_sleep <= 0)
		return (0);
	if (main_philo->number_of_philosophers < 1
		|| main_philo->number_of_philosophers > 200)
		return (0);
	if (main_philo->time_to_die < main_philo->time_to_eat
		|| main_philo->time_to_die < main_philo->time_to_sleep)
		return (0);
	return (1);
}

int	ft_check_args(int ac, char *av[])
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6)
		return (0);
	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			if (ft_isdigit(av[i][j]) == 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
