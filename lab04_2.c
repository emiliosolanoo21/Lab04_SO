#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PHILOSOPHERS 5

pthread_mutex_t forks[PHILOSOPHERS];
sem_t waiter;

void *philosopher(void *arg) {
    int philosopher_id = *((int *)arg);
    int right_fork = philosopher_id;
    int left_fork = (philosopher_id + 1) % PHILOSOPHERS;

    while (1) {
        //Pensar
        printf("El filósofo %d piensa\n", philosopher_id);
        sleep(1);

        //Esperar espacio en la mesa
        sem_wait(&waiter);

        //Tomar tenedores
        pthread_mutex_lock(&forks[right_fork]);
        pthread_mutex_lock(&forks[left_fork]);

        //Comer
        printf("El filósofo %d come\n", philosopher_id);
        sleep(2);

        // Liberar los tenedores
        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);

        // Dejar espacio en la mesa
        sem_post(&waiter);
    }
}

int main() {
    pthread_t philosophers[PHILOSOPHERS];
    int philosopher_ids[PHILOSOPHERS];

    sem_init(&waiter, 0, PHILOSOPHERS - 1); // Inicializar el semáforo del camarero

    //Inicializar mutex para tenedores
    for (int i = 0; i < PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    //Creación de hilos
    for (int i = 0; i < PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    //Esperar a que los filósofos terminen (bucle infinito)
    for (int i = 0; i < PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destruir los mutex y semáforos
    for (int i = 0; i < PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    sem_destroy(&waiter);

    return 0;
}
