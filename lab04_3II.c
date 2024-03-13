#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define RESOURCES 3
#define PROCESSES 5

int availableR[RESOURCES] = {10, 5, 7};
int assignedR[PROCESSES][RESOURCES] = {0};
int requestedR[PROCESSES][RESOURCES] = {0};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* proceso(void* arg) {
    int id = *((int*)arg);
    int requestedRL[RESOURCES];
    for (int i = 0; i < RESOURCES; ++i) {
        requestedRL[i] = rand() % (availableR[i] + 1);
    }

    while (1) {
        //Simular trabajo
        sleep(rand() % 5 + 1);

        //Solicitar recursos
        pthread_mutex_lock(&mutex);
        printf("Proceso %d solicita recursos: ", id);
        for (int i = 0; i < RESOURCES; ++i) {
            requestedR[id][i] = rand() % (availableR[i] + 1);
            printf("%d ", requestedR[id][i]);
        }
        printf("\n");
        pthread_mutex_unlock(&mutex);

        //Comprobar asignación de recursos solicitados
        bool enoughR = true;
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < RESOURCES; ++i) {
            if (requestedR[id][i] > availableR[i]) {
                enoughR = false;
                break;
            }
        }
        if (enoughR) {
            // Asignar recursos
            for (int i = 0; i < RESOURCES; ++i) {
                availableR[i] -= requestedR[id][i];
                assignedR[id][i] += assignedR[id][i];
            }
            printf("Recursos asignados al proceso %d\n", id);
        } else {
            printf("No hay recursos suficientes para asignar al proceso %d\n", id);
        }
        pthread_mutex_unlock(&mutex);

        //Liberar recursos
        sleep(rand() % 5 + 1);
        pthread_mutex_lock(&mutex);
        printf("Proceso %d libera recursos: ", id);
        for (int i = 0; i < RESOURCES; ++i) {
            availableR[i] += assignedR[id][i];
            printf("%d ", assignedR[id][i]);
            assignedR[id][i] = 0;
        }
        printf("\n");
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t threads[PROCESSES];
    int ids[PROCESSES];

    //Crear procesos
    for (int i = 0; i < PROCESSES; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, proceso, &ids[i]);
    }

    //Esperar finalización de proceso (bucle infinito)
    for (int i = 0; i < PROCESSES; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
