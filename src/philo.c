#include "../philo.h"

int	main(int c, char **v)
{
	t_data	data = {0};
	if (c != 5 && c != 6)
	{
		free_all(&data);
		printf(MSG);
		return (1);
	}
	if (init_data(&data, v, c) != 0)
		return (1);
	if (philo_creation(&data) != 0)
	{
		free_all(&data);
		return (1);
	}
	philo_wait(&data);
	free_all(&data);
	return (0);
}
