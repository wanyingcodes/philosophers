/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:04:36 by wtang             #+#    #+#             */
/*   Updated: 2026/01/16 16:07:04 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_assign_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

void	ft_apply_initial_delay(t_philo *philo)
{
	int	delay_us;

	delay_us = 0;
	if (philo->p_id % 2 == 0)
		delay_us = 1000;
	delay_us += (philo->p_id * 20) % 300;
	if (delay_us > 0)
		usleep(delay_us);
}
