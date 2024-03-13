#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource2_mutex = PTHREAD_MUTEX_INITIALIZER;

void *process1(void *arg) {
    while (1) {
        pthread_mutex_lock(&resource1_mutex);
        printf("Proceso 1 con recurso 1\n");
        sleep(1);
        pthread_mutex_lock(&resource2_mutex);
        printf("Proceso 1 con recurso 2\n");

        sleep(2);

        pthread_mutex_unlock(&resource2_mutex);
        printf("Proceso 1 con recurso 2\n");
        pthread_mutex_unlock(&resource1_mutex);
        printf("Proceso 1 con recurso 1\n");
    }
}

void *process2(void *arg) {
    while (1) {
        pthread_mutex_lock(&resource2_mutex);
        printf("Proceso 2 con recurso 2\n");
        sleep(1);
        pthread_mutex_lock(&resource1_mutex);
        printf("Proceso 2 con recurso 1\n");

        sleep(2);

        pthread_mutex_unlock(&resource1_mutex);
        printf("Proceso 2 con recurso 1\n");
        pthread_mutex_unlock(&resource2_mutex);
        printf("Proceso 2 con recurso 2\n");
    }
}

int main() {
    pthread_t tid1, tid2;

    //Creación de procesos
    pthread_create(&tid1, NULL, process1, NULL);
    pthread_create(&tid2, NULL, process2, NULL);

    //Esperar a que terminen, a pesar de ser infinitos.
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
