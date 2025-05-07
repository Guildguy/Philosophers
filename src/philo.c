#include "../philo.h"

void	*philo_creation(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	printf("Philosopher [%d] created!\n", philo->ID);
	return (NULL);
}

int	init_data (t_data *data, char **v)
{
	int	i;

	i = 0;
	data->nbr_of_philos = atoi(v[1]);
	if (data->nbr_of_philos <= 0)
	{
		printf("Error: must exist at least 1 philosopher!\n");
		return (1);
	}
	data->philos = malloc(sizeof(t_philo) * data->nbr_of_philos);
	data->threads = main(sizeof(pthread_t) * data->nbr_of_philos);
	if (!data->philos || !data->threads)
	{
		printf("Error: failure to alocate memory!\n");
		free(data->philos);
		free(data->threads);
		return (1);
	}
	while (i <= data->nbr_of_philos)
	{
		data->philos[i].ID = i + 1;
		i++;
	}
	return (0);
}

int	main(int c, char **v)
{
	int			i;
	int			*IDs;
	int			nbr_of_philos;
	pthread_t	*philos;

	if (/*c < 5 || c > 6*/ c != 2)
	{
		printf(MSG);
		return (1);
		//num. filos = qnt de filos e garfo
		//tmp. morte = em milisegundos, o filo precisa comer dentro do tempo imposto em <tempoMorte>, se expirar, ele morre
		//tmp. comer = em milisegundo, o tempo que leva para o filo comer, ele precisa segurar os 2 garfos
		//tmp. mimir = em milisegundos, o tempo que o filo gasta dormindo
		// n x cada filo pracisa comer = (opcional), se for imposto, a simulação acaba 
		  //quando o filo comer as x vezes impostas, se não utilizado, acaba quando todos morrem
	}
	nbr_of_philos = atoi(v[1]);
	if (nbr_of_philos <= 0)
	{
		write(2, "Error: must exist at least 1 philosopher!\n", 42);
		return (1);
	}
	philos = malloc(sizeof(pthread_t) * nbr_of_philos);
	IDs = malloc(sizeof(int) * nbr_of_philos);
	if (!philos || !IDs)
	{
		write(2, "Error: failure to alocate memory!\n", 34);
		free(philos);
		free(IDs);
		return (1);
	}
	i = 0;
	while (i < nbr_of_philos)
	{
		IDs[i] = i + 1;
		if (pthread_create(&philos[i], NULL, philo_creation, &IDs[i]) != 0)
		{
			printf("Error: failure to create thread n°: [%d]!\n", i + 1);
			free(philos);
			free(IDs);
			return (1);
		}
		i++;
	}
	
	i = 0;
	while (i < nbr_of_philos)
	{
		if (pthread_join(philos[i], NULL) != 0)
			printf("Error: failure in wait the thread [%d]!\n", i + 1);
		i++;
	}
	free(philos);
	free(IDs);
	return (0);
}