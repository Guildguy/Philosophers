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
//changes
typedef struct s_philo
{
	unsigned int	id;
	struct s_data	*data;
	unsigned long	last_meal;
	unsigned int	meals;
}				t_philo;

typedef struct s_data
{
	unsigned int	nbr_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	is_dead;
	unsigned long	start_time;
	unsigned int	nbr_of_meals;
	unsigned int	ate_enough;
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
int		init_data(t_data *data, int c, char **v);
int		philo_creation(t_data *data);
void	philo_wait(t_data *data);
void	*monitor_routine(void *arg);
/////philo_behavior
int		behavior_prevention(t_philo *philo, unsigned int *l_fork,
			unsigned int *r_fork);
void	philo_behavior(t_philo *philo, char *action);
#endif
