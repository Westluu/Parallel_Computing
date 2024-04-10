#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>

#define CORE 4
#define MAX_SIZE 1000
pthread_t thread[CORE];
float mat_A[MAX_SIZE][MAX_SIZE], mat_B[MAX_SIZE][MAX_SIZE], result[MAX_SIZE][MAX_SIZE], resultSequential[MAX_SIZE][MAX_SIZE];

// Sequential Matrix Multiplication Implementation
// resulting matrix size is A * B (row of A x col of B)
void MMSeq()
{
    int row, col, i;
    for (row = 0; row < MAX_SIZE; row++)
    {
        for (col = 0; col < MAX_SIZE; col++)
        {
            float sum = 0;
            for (i = 0; i < MAX_SIZE; i++)
            {
                sum += mat_A[row][i] * mat_B[i][col];
            }
            resultSequential[row][col] = sum;
        }
    }
}

// 1D Array Representation
void *MM(void *id)
{
    int core = (intptr_t)id;
    for (int target = core * MAX_SIZE * MAX_SIZE / CORE; target < (core + 1) * MAX_SIZE * MAX_SIZE / CORE; target++)
    {
        float sum = 0;
        int row = target / MAX_SIZE;
        int col = target % MAX_SIZE;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            float a = *((float *)mat_A + row * MAX_SIZE + i);
            float b = *((float *)mat_B + i * MAX_SIZE + col);
            sum += a * b;
        }
        *((float *)result + row * MAX_SIZE + col) = sum;
    }
    return NULL;
}

// 2D array representation
void *MM2(void *id)
{
    intptr_t core = (intptr_t)id;
    int start = core * MAX_SIZE * MAX_SIZE / CORE;
    int end = (core + 1) * MAX_SIZE * MAX_SIZE / CORE;

    for (int target = start; target < end; target++)
    {
        int row = target / MAX_SIZE;
        int col = target % MAX_SIZE;
        float sum = 0;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            sum += mat_A[row][i] * mat_B[i][col];
        }
        result[row][col] = sum;
    }
    return NULL;
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

    gen_matrix();
    // initialize_matrices();

    printf("NUMBER OF CORES: %d\n", CORE);

    // Sequential Computation Timing
    struct timespec beginSeq, endSeq;
    clock_gettime(CLOCK_MONOTONIC, &beginSeq);
    MMSeq(); // Sequential Computation
    clock_gettime(CLOCK_MONOTONIC, &endSeq);
    double elapsedSeq = endSeq.tv_sec - beginSeq.tv_sec;
    elapsedSeq += (endSeq.tv_nsec - beginSeq.tv_nsec) / 1000000000.0;
    printf("Sequential Execution Time: %f seconds\n", elapsedSeq);

    // Parallel Computing Timing
    struct timespec beginT, endT;
    clock_gettime(CLOCK_MONOTONIC, &beginT);

    for (i = 0; i < CORE; i++)
    {
        // pthread_create(&thread[i],NULL,&addition,(void *)(unsigned long long)i);
        pthread_create(&thread[i], NULL, &MM, (void *)(intptr_t)i);
    }
    for (i = 0; i < CORE; i++)
    {
        pthread_join(thread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &endT);
    double elapsedT = endT.tv_sec - beginT.tv_sec;
    elapsedT += (endT.tv_nsec - beginT.tv_nsec) / 1000000000.0;
    printf("Threads (Parallel) Execution Time: %f seconds\n", elapsedT);

    // checking
    for (i = 0; i < MAX_SIZE; i++)
    {
        for (j = 0; j < MAX_SIZE; j++)
        {
            if (result[i][j] != resultSequential[i][j])
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