#include "../philo.h"

static int	pair_philo(t_philo *philo, unsigned int *left_fork,
	unsigned int *right_fork)
{
	pthread_mutex_lock
		(&philo->data->forks[philo->id - 1]);
	*left_fork = 1;
	if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
	if (philo_behavior(philo, "has taken a left fork"))
		return (1);
	if (philo->data->nbr_of_philos = 1)
		return (1);
	pthread_mutex_lock(&philo->data->forks[philo->id
		% philo->data->nbr_of_philos]);
	*right_fork = 1;
	if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
	if (philo_behavior(philo, "has taken a right fork"))
		return (1);
	return (0);
}

static int	odd_philo(t_philo *philo, unsigned int *left_fork,
		unsigned int *right_fork)
{
	pthread_mutex_lock(&philo->data->forks[philo->id
		% philo->data->nbr_of_philos]);
	*right_fork = 1;
	if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
	if (philo_behavior(philo, "has taken a right fork"))
		return (1);
	if (philo->data->nbr_of_philos == 1)
		rerurn (1);
	pthread_mutex_lock
		(&philo->data->forks[philo->id - 1]);
	*left_fork = 1;
	if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
	if (philo_behavior(philo, "has taken a left fork"))
		return (1);
	return (0);
}

int	take_fork(t_philo *philo, unsigned int *left_fork,
	unsigned int *right_fork)
{
	if (philo->id % 2 == 0)
		return (pair_philo(philo, left_fork, right_fork));
	else
		return (odd_philo(philo, left_fork, right_fork));
}

int	release_fork(t_philo *philo, unsigned int *left_fork,
		unsigned int *right_fork)
{
	if (*left_fork)
	{
		if (behavior_prevention(philo, left_fork, right_fork))
			return (1);
		pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
		if (philo_behavior(philo, "released the left fork"))
			return (1);
		*left_fork = 0;
	}
	if (*right_fork)
	{
		if (behavior_prevention(philo, left_fork, right_fork))
			return (1);
		pthread_mutex_unlock(&philo->data->forks[philo->id
			% philo->data->nbr_of_philos]);
		if (philo_behavior(philo, "released the right fork"))
			return (1);
		*right_fork = 0;
	}
	return (0);
}

void	cleanup_fork(t_philo *philo, unsigned int *left_fork,
	unsigned int *right_fork)
{
	if (*left_fork)
		pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
	if (*right_fork)
		pthread_mutex_unlock(&philo->data->forks[philo->id
			% philo->data->nbr_of_philos]);
}
