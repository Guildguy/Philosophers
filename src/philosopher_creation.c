#include "../philo.h"

static long	get_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}


static void	*philo_create(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	while (6)
	{
		/*pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->is_dead)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->dead_unlock);*/
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("philospher [%d] is waiting!\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		if (philo->id % 2 == 0) //filos pares comem 1°
		{
			pthread_mutex_lock
					(&philo->data->forks[philo->id - 1]); //pega garfo direito
			pthread_mutex_lock
					(&philo->data->forks[philo->id
					% philo->data->nbr_of_philos]); //pega garfo esquerdo
		}
		else //filos impares comem 1°
		{
			pthread_mutex_lock
					(&philo->data->forks[philo->id
					% philo->data->nbr_of_philos]); //pega garfo esquerdo
			pthread_mutex_lock
					(&philo->data->forks[philo->id - 1]); //pega garfo direito
		}
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("philosopher [%d] is eating!\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		philo->last_meal = get_time();
		usleep(philo->data->time_to_eat * 100000);
		printf("\n");
		pthread_mutex_unlock(&philo->data->forks[philo->id
			% philo->data->nbr_of_philos]);
		pthread_mutex_unlock(&philo->data->forks[philo->id -1]);
	}
	return (NULL);
}

void	philo_wait(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_join(data->threads[i], NULL) != 0)
			printf("Error: failure in wait the thread [%d]!\n", i + 1);
		i++;
	}
}

int	init_data(t_data *data, char **v)
{
	int	i;

	data->nbr_of_philos = atoi(v[1]);
	if (data->nbr_of_philos <= 0)
	{
		printf("Error: must exist at least 1 philosopher!\n");
		return (1);
	}
	data->philos = malloc(sizeof(t_philo) * data->nbr_of_philos);
	data->threads = malloc(sizeof(pthread_t) * data->nbr_of_philos);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_of_philos);
	if (!data->philos || !data->threads || !data->forks)
	{
		printf("Error: failure to alocate memory!\n");
		free_all(data);
		return (1);
	}
	i = 0;
	while (i < data->nbr_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (0);
}

int	philo_creation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL,
				philo_create, &data->philos[i]) != 0)
		{
			printf("Error: failure to create thread n°: [%d]!\n", i + 1);
			philo_wait(data);
			return (1);
		}
		i++;
	}
	return (0);
}
