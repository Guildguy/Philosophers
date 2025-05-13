#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <string.h> //memset
# include <pthread.h> //threads// processor threads
# include <stdlib.h> //exit
# include <unistd.h> //fork
# include <sys/time.h> //gettimeofday

# define MSG "ERROR: ./philo <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> Optional: \
<number_of_times_each_philosopher_must_eat>\n"

//struct para armazenar os dados do filos
typedef struct s_philo
{
	int				id;
	struct s_data	*data;
	long			last_meal;
}				t_philo;

//struct para criação da thread onde os comportamentos dos filos serão executados
typedef struct s_data
{
	int				nbr_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				is_dead;
	long			start_time;
	t_philo			*philos;
	pthread_t		*threads;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
}				t_data;

//src
////data_handler
void	free_all(t_data *data);
////philosopher_creation
int		init_data(t_data *data, char **v);
int		philo_creation(t_data *data);
void	philo_wait(t_data *data);
void	*monitor_routine(void *arg);

#endif