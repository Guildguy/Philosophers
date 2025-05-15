int	*philo_behavior(t_philo *philo, char *action)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    printf("Philosopher [%d] %s!\n", philo->id, action);
    pthread_mutex_unlock(&philo->data->print_mutex);
    return (0);
}

