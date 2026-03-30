/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:12:56 by wtang             #+#    #+#             */
/*   Updated: 2026/01/16 13:04:14 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, const char *status)
{
	long long	current_time;
	
	if (must_stop(philo->data))
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	if (must_stop(philo->data))
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;		
	}
	current_time = ft_get_time() - philo->data->start_time;
	printf("%lld %d %s\n", current_time, philo->p_id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	ft_create_threads(t_data *data)
{
	int	i;

	data->start_time = ft_get_time();
	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].last_pasta_time = data->start_time;
		i++;
	}
	if (pthread_create(&data->death_checker_thread, NULL, &death_checker,
		data) != 0)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->threads[i], NULL, &ft_routine,
				&data->philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_philo	main_philo;
	t_data	data;

	if (ft_check_args(ac, av) == 0)
		return (1);
	if (ft_init_philo(ac, av, &main_philo) == 0)
		return (1);
	if (!ft_init_simulation(&data, &main_philo))
	{
		ft_clean_up(&data);
		return (1);
	}
	if (!ft_create_threads(&data))
	{
		ft_clean_up(&data);
		return (1);
	}
	ft_clean_up(&data);
	return (0);
}
