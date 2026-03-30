/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 18:03:54 by wtang             #+#    #+#             */
/*   Updated: 2026/01/16 17:02:08 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_take_forks(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	ft_assign_fork_order(philo, first, second);
	if (must_stop(philo->data))
		return (0);
	pthread_mutex_lock(*first);
	if (must_stop(philo->data))
		return (pthread_mutex_unlock(*first), 0);
	print_status(philo, "has taken a fork");
	if (philo->data->philo_count == 1)
	{
		while (!must_stop(philo->data))
			usleep(1000);
		return (pthread_mutex_unlock(*first), 0);
	}
	pthread_mutex_lock(*second);
	if (must_stop(philo->data))
		return (pthread_mutex_unlock(*second),
			pthread_mutex_unlock(*first), 0);
	print_status(philo, "has taken a fork");
	return (1);
}

static void	ft_eat(t_philo *philo)
{
	long long	start_to_do;

	if (must_stop(philo->data))
		return ;
	start_to_do = ft_get_time();
	pthread_mutex_lock(&philo->data->pasta_mutex);
	philo->last_pasta_time = start_to_do;
	pthread_mutex_unlock(&philo->data->pasta_mutex);
	print_status(philo, "is eating");
	while ((ft_get_time() - start_to_do) < philo->time_to_eat)
		usleep(1000);
	pthread_mutex_lock(&philo->data->pasta_mutex);
	philo->pastas_eaten++;
	pthread_mutex_unlock(&philo->data->pasta_mutex);
}

static void	ft_sleep(t_philo *philo)
{
	long long	start_to_do;

	print_status(philo, "is sleeping");
	start_to_do = ft_get_time();
	while (ft_get_time() - start_to_do < philo->time_to_sleep)
	{
		if (must_stop(philo->data))
			break ;
		usleep(1000);
	}
}

static void	ft_think(t_philo *philo)
{
	int			safe_ms;
	long long	wait_time;
	float		ratio;
	int			think_ms;

	print_status(philo, "is thinking");
	safe_ms = philo->time_to_die - philo->time_to_eat;
	safe_ms = safe_ms - philo->time_to_sleep - 15;
	if (safe_ms < 1)
		safe_ms = 1;
	wait_time = ft_get_time() - philo->last_pasta_time;
	if (wait_time > philo->time_to_die * 0.75)
		ratio = 0.15;
	else if (philo->p_id == 1 || philo->p_id == philo->data->philo_count)
		ratio = 0.25;
	else
		ratio = 0.45;
	think_ms = safe_ms * ratio;
	if (think_ms < 1)
		think_ms = 1;
	usleep(think_ms * 1000);
}

void	*ft_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	philo = (t_philo *)arg;
	if (philo == NULL)
		return (NULL);
	ft_apply_initial_delay(philo);
	while (!must_stop(philo->data))
	{
		if (!ft_take_forks(philo, &first_fork, &second_fork))
			break ;
		ft_eat(philo);
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		if (philo->max_pastas > 0
			&& philo->pastas_eaten >= philo->max_pastas)
		{
			mark_philo_finished(philo->data, philo->p_id);
			break ;
		}
		ft_sleep(philo);
		ft_think(philo);
	}
	return (NULL);
}
