#include "../philo.h"

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
	}
}
