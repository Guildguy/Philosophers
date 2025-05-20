#include "../philo.h"

static int	philo_status(t_data *data, unsigned int i,
	unsigned int *meals_done)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (data->is_dead || data->ate_enough)
	{
		pthread_mutex_unlock(&data->dead_mutex);
		return (1);
	}
	if (get_time() - data->philos[i].last_meal >= data->time_to_die)
	{
		data->is_dead = 1;
		pthread_mutex_unlock(&data->dead_mutex);
		philo_behavior(&data->philos[i], "died");
		return (1);
	}
	if (*meals_done && data->philos[i].meals < data->nbr_of_meals)
		*meals_done = 0;
	pthread_mutex_unlock(&data->dead_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data			*data;
	unsigned int	i;
	unsigned int	meals_done;

	data = (t_data *)arg;
    while (6)
    {
        i = 0;
        meals_done = data->nbr_of_meals > 0;
        while (i < data->nbr_of_philos)
        {
            if (philo_status(data, i, &meals_done))
                return (NULL);
            i++;
        }
        if (meals_done)
        {
            data->ate_enough = 1;
            philo_behavior(&data->philos[0], "ate_enough");
            return (NULL);
        }
        usleep(500);
    }
    return (NULL);
}
