#include "../philo.h"

static long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

static void	*philo_create(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep(philo->id * 1000);
	while (6)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->is_dead)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("philosopher [%d] is waiting!\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock
					(&philo->data->forks[philo->id - 1]);
			pthread_mutex_lock
					(&philo->data->forks[philo->id
					% philo->data->nbr_of_philos]);
		}
		else
		{
			pthread_mutex_lock
					(&philo->data->forks[philo->id
					% philo->data->nbr_of_philos]);
			pthread_mutex_lock
					(&philo->data->forks[philo->id - 1]);
		}
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->is_dead)
		{
			pthread_mutex_unlock
					(&philo->data->forks[philo->id
					% philo->data->nbr_of_philos]);
			pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("philosopher [%d] is eating!\n", philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		philo->last_meal = get_time();
		usleep(philo->data->time_to_eat * 1000);
		printf("\n");
		pthread_mutex_unlock(&philo->data->forks[philo->id
			% philo->data->nbr_of_philos]);
		pthread_mutex_unlock(&philo->data->forks[philo->id -1]);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	while (6)
	{
		i = 0;
		while (i < data->nbr_of_philos)
		{
			pthread_mutex_lock(&data->dead_mutex);
			if (data->is_dead)
			{
				pthread_mutex_unlock(&data->dead_mutex);
				return (NULL);
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
		usleep(1000);
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
	if (pthread_join(data->monitor, NULL) != 0)
		printf("Error: failure in wait the monitor!\n");
}

int	init_data(t_data *data, char **v)
{
	int	i;

	data->nbr_of_philos = atoi(v[1]);
	data->time_to_die = atoi(v[2]);
	data->time_to_eat = atoi(v[3]);
	if (data->nbr_of_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0)
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
	data->start_time = get_time();
	data->is_dead = 0;
	i = 0;
	while (i < data->nbr_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].last_meal = data->start_time;
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	return (0);
}

int	philo_creation(t_data *data)
{
	int	i;

	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
	{
		printf("Error: failure to create the monitor thread!\n");
		return (1);
	}
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
