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

int	parse_args(t_data *data, int c, char *v[])
{
	int i;
    
	i = 1;
    while (i < c)
    {
        if (!v[i][0] || v[i][0] == '0' || atoi(v[i]) <= 0)
        {
            printf("Error: invalid arguments!\n");
            return (1);
        }
        i++;
    }
    data->nbr_of_philos = atoi(v[1]);
    data->time_to_die = atoi(v[2]);
    data->time_to_eat = atoi(v[3]);
    data->time_to_sleep = atoi(v[4]);
    if (c == 6)
        data->nbr_of_meals = atoi(v[5]);
    else
        data->nbr_of_meals = 0;
    if (data->nbr_of_philos < 1 || data->time_to_die < 0 ||
        data->time_to_eat < 0 || data->time_to_sleep < 0 ||
        (c == 6 && data->nbr_of_meals <= 0))
    {
        printf("Error: invalid arguments!\n");
        return (1);
    }
    return (0);
}

int	create_resources(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nbr_of_philos);
	data->threads = malloc(sizeof(pthread_t) * data->nbr_of_philos);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_of_philos);
	if (!data->philos || !data->threads || !data->forks)
	{
		printf("Error: failure to allocate memory!\n");
		free_all(data);
		return (1);
	}
	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free_all(data);
			return (1);
		}
		i++;
	}
	return (0);
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
