#include "../philo.h"

int	create_fork(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_of_philos);
	if (!data->forks)
	{
		printf("Error:failure to allocate memory for fork!\n");
		return (1);
	}
	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i > 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			data->forks = NULL;
			printf("Error: failure to initialize forks!\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	take_fork(t_philo *philo, unsigned int *left_fork,
		unsigned int *right_fork)
{
	if (philo->id % 2 == 0) //filos pares
	{
		pthread_mutex_lock
				(&philo->data->forks[philo->id - 1]); //garfo esquerdo
		*left_fork = 1;
		if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
		philo_behavior(philo, "has taken left fork!");
		pthread_mutex_lock(&philo->data->forks[philo->id
			% philo->data->nbr_of_philos]); //garfo direito
		*right_fork = 1;
		if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
		philo_behavior(philo, "has taken right fork!");
	}
	else //filos impares
	{
		pthread_mutex_lock(&philo->data->forks[philo->id
			% philo->data->nbr_of_philos]); //garfo direito
		*right_fork = 1;
		if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
		philo_behavior(philo, "has taken right fork!");
		pthread_mutex_lock
				(&philo->data->forks[philo->id - 1]); //garfo esquerdo
		*left_fork = 1;
		if (behavior_prevention(philo, left_fork, right_fork))
		return (1);
		philo_behavior(philo, "has taken left fork!");
	}
	return (0);
}
