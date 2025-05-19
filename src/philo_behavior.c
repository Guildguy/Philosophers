#include "../philo.h"

int	behavior_prevention(t_philo *philo, unsigned int *left_fork,
		unsigned int *right_fork)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->is_dead || philo->data->ate_enough)
	{
		if (*left_fork)
			pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
		if (*right_fork)
			pthread_mutex_unlock(&philo->data->forks[philo->id
				% philo->data->nbr_of_philos]);
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

void	philo_behavior(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("Philosopher [%d] %s!\n", philo->id, action);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	eat_time(t_philo *philo, unsigned int *left_fork,
		unsigned int *right_fork)
{
	if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
	philo_behavior(philo, "is eating!");
	philo->last_meal = get_time();
	philo->meals++;
	safe_usleep(philo->data->time_to_eat, philo);
	return (0);
}

int	sleep_time(t_philo *philo, unsigned int *left_fork,
		unsigned int *right_fork)
{
	if (behavior_prevention(philo, left_fork, right_fork))
		return (1) ;
	philo_behavior(philo, "is sleeping!");
	safe_usleep(philo->data->time_to_sleep, philo);
	return (0);
}
