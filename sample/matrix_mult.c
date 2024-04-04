% % writefile matvec.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>

#define CORE 4
#define MAX_SIZE 2048
        pthread_t thread[CORE];
int mat_A[MAX_SIZE][MAX_SIZE], vector[MAX_SIZE], result[MAX_SIZE], resultSequential[MAX_SIZE];

void *p_matvec(void *id)
{
    int threadID = (intptr_t)id;
    int i, j;
    int local_m = MAX_SIZE / CORE;
    int first_row = threadID * local_m;
    int last_row = first_row + local_m;
    for (i = first_row; i < last_row; i++)
    {
        result[i] = 0;
        for (j = 0; j < MAX_SIZE; j++)
            result[i] += mat_A[i][j] * vector[j];
    }
    return NULL;
}

// Main function
int main(int Argc, char *Args[])
{
    int i, j;
    int error = 0;

    // init array and vector
    for (i = 0; i < MAX_SIZE; i++)
    {
        for (j = 0; j < MAX_SIZE; j++)
        {
            mat_A[i][j] = rand() % 10;
        }
    }
    for (i = 0; i < MAX_SIZE; i++)
        vector[i] = rand() % 10;

    // for comparation
    for (i = 0; i < MAX_SIZE; i++)
    {
        for (j = 0; j < MAX_SIZE; j++)
        {
            resultSequential[i] += mat_A[i][j] * vector[j];
        }
    }

    for (i = 0; i < CORE; i++)
    {
        // pthread_create(&thread[i],NULL,&p_matvec,(void *)(unsigned long long)i);
        pthread_create(&thread[i], NULL, &p_matvec, (void *)(intptr_t)i);
    }
    for (i = 0; i < CORE; i++)
    {
        pthread_join(thread[i], NULL);
    }

    for (i = 0; i < MAX_SIZE; i++)
    {
        if (result[i] != resultSequential[i])
        {
            error++;
        }
    }
    if (error > 0)
        printf("Error");
    else
        printf("Done!");
    return 0;
}