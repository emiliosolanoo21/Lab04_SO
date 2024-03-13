#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define RESOURCES 3
#define PROCESSES 5

int availableR[RESOURCES] = {10, 5, 7};
int assignedR[PROCESSES][RESOURCES] = {0};
int requestedR[PROCESSES][RESOURCES] = {0};
bool done[PROCESSES] = {false};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool sure(int proceso) {
    int availableTemp[RESOURCES];
    for (int i = 0; i < RESOURCES; ++i) {
        availableTemp[i] = availableR[i] - requestedR[proceso][i];
    }

    bool doneTemp[PROCESSES];
    for (int i = 0; i < PROCESSES; ++i) {
        doneTemp[i] = done[i];
    }

    for (int i = 0; i < PROCESSES; ++i) {
        if (!doneTemp[i]) {
            bool assigningR = true;
            for (int j = 0; j < RESOURCES; ++j) {
                if (requestedR[i][j] > availableTemp[j]) {
                    assigningR = false;
                    break;
                }
            }

            if (assigningR) {
                for (int j = 0; j < RESOURCES; ++j) {
                    availableTemp[j] += assignedR[i][j];
                }
                doneTemp[i] = true;
                i = -1; //Reiniciar el ciclo para verificar si otros procesos ahora pueden ejecutarse de manera segura
            }
        }
    }

    for (int i = 0; i < PROCESSES; ++i) {
        if (!doneTemp[i]) {
            return false; //Procesos no finalizados que no pueden ejecutarse de manera segura
        }
    }

    return true; //Todos los procesos pueden ejecutarse de manera segura
}

void* proceso(void* arg) {
    int id = *((int*)arg);
    int requestedRL[RESOURCES];

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

        //Comprobar aignación segura
        pthread_mutex_lock(&mutex);
        if (sure(id)) {
            //Asignar recursos
            for (int i = 0; i < RESOURCES; ++i) {
                availableR[i] -= requestedR[id][i];
                assignedR[id][i] += requestedR[id][i];
            }
            printf("Recursos asignados al proceso %d\n", id);
        } else {
            printf("No hay suficientes recursos disponibles para asignar al proceso %d\n", id);
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

    //Esperar terminación de ciclos (bucle infinito)
    for (int i = 0; i < PROCESSES; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
