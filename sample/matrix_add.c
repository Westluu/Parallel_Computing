#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>

#define CORE 4
#define MAX_SIZE 1000

pthread_t thread[CORE];
int mat_A[MAX_SIZE][MAX_SIZE], mat_B[MAX_SIZE][MAX_SIZE], sum[MAX_SIZE][MAX_SIZE], sumSequential[MAX_SIZE][MAX_SIZE];

void *addition(void *arg) {
    int i, j;
    // int core = (int)arg;
    // int core = *((int*)(&arg));
    int core = (intptr_t)arg;
    // Each thread computes 1/4th of matrix addition
    for (i = core * MAX_SIZE / 4; i < (core + 1) * MAX_SIZE / 4; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            sum[i][j] = mat_A[i][j] + mat_B[i][j];
        }
    }
    return NULL;
}


void addSequential() {
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            sumSequential[i][j] = mat_A[i][j] + mat_B[i][j];
        }
    }
}

// Main function
int main(int Argc, char *Args[]) {
    int i, j;
    int error = 0;


    //generate 1000 size matrix for A and B
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            mat_A[i][j] = rand() % 10;
            mat_B[i][j] = rand() % 10;
        }
    }

    // for comparation
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            sumSequential[i][j] = mat_A[i][j] + mat_B[i][j];
        }
    }

    for (i = 0; i < CORE; i++) {
        // pthread_create(&thread[i],NULL,&addition,(void *)(unsigned long long)i);
        pthread_create(&thread[i], NULL, &addition, (void *)(intptr_t)i);
    }
    for (i = 0; i < CORE; i++) {
        pthread_join(thread[i], NULL);
    }
    // check parellel implementation produces same results as the sequential one ALWAYS
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            if (sum[i][j] != sumSequential[i][j]) {
                error++;
            }
        }
    }
    if (error > 0)
        printf("Error");
    else
        printf("Done!");
    return 0;
}