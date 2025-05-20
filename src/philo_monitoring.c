#include "../philo.h"

void	*monitor_routine(void *arg)
{
	unsigned int	i;
	unsigned int	j;
	t_data			*data;

	data = (t_data *)arg;
	while (6)
	{
		i = 0;
		while (i < data->nbr_of_philos)
		{
			pthread_mutex_lock(&data->dead_mutex);
			if (data->is_dead || data->ate_enough)
			{
				pthread_mutex_unlock(&data->dead_mutex);
				return (NULL);
			}		
			if (data->nbr_of_meals > 0)
			{
				j = 0;
				while (j < data->nbr_of_philos)
				{
					if (data->philos[j].meals < data->nbr_of_meals)
						break ;
					j++;
				}
				if (j == data->nbr_of_philos)
				{
					data->ate_enough = 1;
					pthread_mutex_lock(&data->print_mutex);
					printf("philosophers ate %u times!\n",
						data->nbr_of_meals);
					pthread_mutex_unlock(&data->print_mutex);
					pthread_mutex_unlock(&data->dead_mutex);
					return (NULL);
				}
			}
			if (get_time() - data->philos[i].last_meal >= data->time_to_die)
			{
				data->is_dead = 1;
				pthread_mutex_lock(&data->print_mutex);
				printf("philosopher [%d] is dead!\n", data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				pthread_mutex_unlock(&data->dead_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->dead_mutex);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data			*data;
	unsigned int	i;
	unsigned int	meals_done;

	i = 0;
	meals_done = data->nbr_of_meals > 0;
	data = 	(t_data *)arg;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(&data->dead_mutex);
		pthread_mutex_unlock(&data->dead_mutex);
		i++;
	}
}
