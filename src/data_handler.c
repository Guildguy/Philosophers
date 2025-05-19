#include "../philo.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	safe_usleep(unsigned long duration, t_philo *philo)
{
	long	start;

	start = get_time();
	while (6)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->is_dead || philo->data->ate_enough)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		if (get_time() - start >= duration)
			return ;
		usleep(500);
	}
}

void	free_all(t_data *data)
{
	int	i;

	if (data)
	{
		if (data->philos)
			free(data->philos);
		data->philos = NULL;
		if (data->threads)
			free(data->threads);
		data->threads = NULL;
		if (data->forks)
		{
			i = 0;
			while (i < data->nbr_of_philos)
			{
				pthread_mutex_destroy(&data->forks[i]);
				i++;
			}
			free(data->forks);
		}
		data->forks = NULL;
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->dead_mutex);
	}
}
