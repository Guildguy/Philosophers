#include "../philo.h"

int	main(int c, char **v)
{
	t_data	data = {0};
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
	if (init_data(&data, v) != 0)
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