/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:13:04 by wtang             #+#    #+#             */
/*   Updated: 2026/01/14 13:30:19 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_join_and_free_threads(t_data *data)
{
	int	i;

	if (data->death_checker_thread != 0)
		pthread_join(data->death_checker_thread, NULL);
	if (data->threads)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (data->threads[i] != 0)
				pthread_join(data->threads[i], NULL);
			i++;
		}
		free(data->threads);
	}
}

void	ft_clean_up(t_data *data)
{
	int	i;
	
	if (!data)
		return ;
	ft_join_and_free_threads(data);
	if (data->forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->finish_mutex_initialized)
		pthread_mutex_destroy(&data->finish_mutex);
	if (data->pasta_mutex_initialized)
		pthread_mutex_destroy(&data->pasta_mutex);
	if (data->print_mutex_initialized)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->philos)
		free(data->philos);
	if (data->pastas_count)
		free(data->pastas_count);
}

long long	ft_get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		write(2, "Error: gettimeofday() failed\n", 29);
		return (0);
	}
	return (time.tv_sec * 1000LL + time.tv_usec / 1000);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	while (*nptr == ' ' || *nptr == '\t' || *nptr == '\n' || *nptr == '\r'
		|| *nptr == '\v' || *nptr == '\f')
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
	{
		i = i * 10 + (*nptr - '0');
		nptr++;
	}
	return (sign * i);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}
