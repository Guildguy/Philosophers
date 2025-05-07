#include "../philo.h"

void	free_all(t_data *data)
{
	if (data)
	{
		free(data->philos);
		data->philos = NULL;
		free(data->threads);
		data->threads = NULL;
	}
}