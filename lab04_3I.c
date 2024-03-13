#include <stdio.h>
#include <stdbool.h>

#define PROCESSES 5
#define RESOURCES 3

int max_demands[PROCESSES][RESOURCES] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

int assigned[PROCESSES][RESOURCES] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

int available[RESOURCES] = {3, 3, 2};
bool done[PROCESSES] = {false};

bool safety(int process) {
    int needings[RESOURCES];
    for (int i = 0; i < RESOURCES; ++i) {
        needings[i] = max_demands[process][i] - assigned[process][i];
    }

    bool assignResources = true;
    for (int i = 0; i < RESOURCES; ++i) {
        if (needings[i] > available[i]) {
            assignResources = false;
            break;
        }
    }

    if (!assignResources)
        return false;

    for (int i = 0; i < RESOURCES; ++i) {
        available[i] += assigned[process][i];
        assigned[process][i] = 0;
    }
    done[process] = true;

    for (int i = 0; i < PROCESSES; ++i) {
        if (!done[i] && safety(i))
            return true;
    }

    for (int i = 0; i < RESOURCES; ++i) {
        assigned[process][i] = max_demands[process][i];
        available[i] -= max_demands[process][i];
    }
    
    done[process] = false;

    return false;
}

int main() {
    printf("Recursos disponibles: ");
    for (int i = 0; i < RESOURCES; ++i) {
        printf("%d ", available[i]);
    }
    
    printf("\n");

    for (int i = 0; i < PROCESSES; ++i) {
        if (!done[i]) {
            if (safety(i)) {
                printf("Proceso %d puede ejecutarse de manera segura\n", i);
            } else {
                printf("Proceso %d no puede ejecutarse de manera segura\n", i);
            }
        }
    }

    return 0;
}
