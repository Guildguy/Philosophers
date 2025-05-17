#include "../philo.h"

int	behavior_prevention(t_philo *philo, unsigned int *l_fork,
		unsigned int *r_fork)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->is_dead || philo->data->ate_enough)
	{
		if (*l_fork)
			pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
		if (*r_fork)
			pthread_mutex_unlock(&philo->data->forks[philo->id
				% philo->data->nbr_of_philos]);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}
int	philo_behavior(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (behavior_prevention(philo, &(unsigned int){0}, &(unsigned int){0}))
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (1);
	}	
	printf("Philosopher [%d] %s!\n", philo->id, action);
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (0);
}
