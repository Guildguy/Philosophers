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
		i++;
	}
	return (0);
}