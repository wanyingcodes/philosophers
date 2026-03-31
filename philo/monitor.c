/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 18:05:19 by wtang             #+#    #+#             */
/*   Updated: 2026/01/16 16:01:38 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	must_stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->finish_mutex);
	stop = data->stop_simulation;
	pthread_mutex_unlock(&data->finish_mutex);
	return (stop);
}

void	mark_philo_finished(t_data *data, int id)
{
	long long	current_time;
	
	if (must_stop(data))
		return ;
	pthread_mutex_lock(&data->finish_mutex);
	if (data->pastas_count[id - 1] == 0)
	{
		data->pastas_count[id - 1] = 1;
		data->finished_count++;
		if (data->finished_count >= data->philo_count)
		{
			data->stop_simulation = 1;
			pthread_mutex_lock(&data->print_mutex);
			current_time = ft_get_time() - data->start_time;
			printf("%lld They all ate %d pastas\n", current_time,
				data->philos->pastas_eaten);
			pthread_mutex_unlock(&data->print_mutex);
		}
	}
	pthread_mutex_unlock(&data->finish_mutex);
}

static int	check_philosopher_death(t_data *data, int i)
{
	long long	time_since_last_meal;
	long long	current_time;

	pthread_mutex_lock(&data->pasta_mutex);
	time_since_last_meal = ft_get_time() - data->philos[i].last_pasta_time;
	pthread_mutex_unlock(&data->pasta_mutex);
	if (time_since_last_meal > data->philos[i].time_to_die)
	{
		pthread_mutex_lock(&data->finish_mutex);
		if (!data->stop_simulation)
		{
			data->stop_simulation = 1;
			pthread_mutex_lock(&data->print_mutex);
			current_time = ft_get_time() - data->start_time;
			printf("%lld %d died\n", current_time, data->philos[i].p_id);
			pthread_mutex_unlock(&data->print_mutex);
		}
		pthread_mutex_unlock(&data->finish_mutex);
		return (1);
	}
	return (0);
}

void	*death_checker(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		if (must_stop(data))
            break ;
		i = 0;
		while (i < data->philo_count)
		{
			if (check_philosopher_death(data, i))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
