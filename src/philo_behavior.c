#include "../philo.h"

int	*philo_behavior(t_philo *philo, char *action)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    printf("Philosopher [%d] %s!\n", philo->id, action);
    pthread_mutex_unlock(&philo->data->print_mutex);
    return (0);
}

int behavior_prevention(t_philo *philo, unsigned int *l_fork, unsigned int *r_fork)
{
    pthread_mutex_lock(&philo->data->dead_mutex);
    pthread_mutex_unlock(&philo->data->dead_mutex);
}