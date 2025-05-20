#include "../philo.h"

static int	ft_atoi(const char *str)
{
	long	result;

	result = 0;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + (*str++ - '0');
	return (result);
}

static int	check_args(char *arg)
{
	if (!arg[0] || arg[0] == '0')
		return (0);
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (0);
		arg++;
	}
	return (1);
}

static int	set_args(t_data *data, int c, char *v[])
{
	data->nbr_of_philos = ft_atoi(v[1]);
	data->time_to_die = ft_atoi(v[2]);
	data->time_to_eat = ft_atoi(v[3]);
	data->time_to_sleep = ft_atoi(v[4]);
	if (c == 6)
		data->nbr_of_meals = ft_atoi(v[5]);
	else
		data->nbr_of_meals = 0;
	if (data->nbr_of_philos < 1 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (c == 6 && data->nbr_of_meals <= 0))
		return (0);
	return (1);
}

int	parse_args(t_data *data, int c, char *v[])
{
	int	i;

	i = 1;
	while (i < c)
	{
		if (!check_args(v[i]))
		{
			printf("Error: invalid arguments!\n");
			return (1);
		}
		i++;
	}
	if (!set_args(data, c, v))
	{
		printf("Error: invalid arguments!\n");
		return (1);
	}
	return (0);
}
