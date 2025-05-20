#include "../philo.h"

static void	*philo_create(void *arg)
{
	t_philo			*philo;
	unsigned int	right_fork;
	unsigned int	left_fork;

	right_fork = 0;
	left_fork = 0;
	philo = (t_philo *)arg;
	usleep(philo->id * 500);
	while (6)
	{
		if (take_fork(philo, &left_fork, &right_fork))
			break ;
		if (eat_time(philo, &left_fork, &right_fork))
			break ;
		if (release_fork(philo, &left_fork, &right_fork))
			break ;
		if (sleep_time(philo, &left_fork, &right_fork))
			break ;
		if (think_time(philo, &left_fork, &right_fork))
			break ;
	}
	cleanup_fork(philo, &left_fork, &right_fork);
	return (NULL);
}

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
	philo_wait(data);
	return (0);
}

static void	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].meals = 0;
		i++;
	}
}

int	init_data(t_data *data, int c, char **v)
{
	if (parse_args(data, c, v))
		return (1);
	if (create_resources(data))
		return (1);
	data->start_time = get_time();
	data->is_dead = 0;
	data->ate_enough = 0;
	init_philo(data);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		printf("Error: failure to initialize print_mutex!\n");
		free_all(data);
		return (1);
	}
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
	{
		printf("Error: failure to initialize dead_mutex!\n");
		free_all(data);
		return (1);
	}
	return (0);
}
