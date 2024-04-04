#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>

#define CORE 4
#define MAX_SIZE 1000

pthread_t thread[CORE];
float mat_A[MAX_SIZE][MAX_SIZE], mat_B[MAX_SIZE][MAX_SIZE], sum[MAX_SIZE][MAX_SIZE], sumSequential[MAX_SIZE][MAX_SIZE];

void *addition(void *arg)
{
    int i, j;
    int core = (intptr_t)arg;
    for (i = core * MAX_SIZE / CORE; i < (core + 1) * MAX_SIZE / CORE; i++)
    {
        for (j = 0; j < MAX_SIZE; j++)
        {
            sum[i][j] = mat_A[i][j] + mat_B[i][j];
        }
    }
    return NULL;
}

void addSequential()
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            sumSequential[i][j] = mat_A[i][j] + mat_B[i][j];
        }
    }
}

void gen_matrix()
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            mat_A[i][j] = (float)rand() / RAND_MAX;
            mat_B[i][j] = (float)rand() / RAND_MAX;
        }
    }
}

// Main function
int main(int Argc, char *Args[])
{
    int i, j;
    int error = 0;

    // generate 1000 size matrix for A and B
    gen_matrix();

    // Sequential Computation Timing
    struct timespec beginSeq, endSeq;
    clock_gettime(CLOCK_MONOTONIC, &beginSeq);
    addSequential(); // Sequential Computation
    clock_gettime(CLOCK_MONOTONIC, &endSeq);
    double elapsedSeq = endSeq.tv_sec - beginSeq.tv_sec;
    elapsedSeq += (endSeq.tv_nsec - beginSeq.tv_nsec) / 1000000000.0;
    printf("Sequential Execution Time: %f seconds\n", elapsedSeq);

    // Parallel Computation Timing
    struct timespec beginT, endT;
    clock_gettime(CLOCK_MONOTONIC, &beginT);

    for (i = 0; i < CORE; i++)
    {
        // pthread_create(&thread[i],NULL,&addition,(void *)(unsigned long long)i);
        pthread_create(&thread[i], NULL, &addition, (void *)(intptr_t)i);
    }
    for (i = 0; i < CORE; i++)
    {
        pthread_join(thread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &endT);
    double elapsedT = endT.tv_sec - beginT.tv_sec;
    elapsedT += (endT.tv_nsec - beginT.tv_nsec) / 1000000000.0;
    printf("Threads (Parallel) Execution Time: %f seconds\n", elapsedT);

    // check parellel implementation produces same results as the sequential one ALWAYS
    for (i = 0; i < MAX_SIZE; i++)
    {
        for (j = 0; j < MAX_SIZE; j++)
        {
            if (sum[i][j] != sumSequential[i][j])
            {
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