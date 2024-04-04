% % writefile pi.c
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 8

    typedef struct
{
    int rank;
    double sum;
} thread_data;

thread_data td[NUM_THREADS];

void *help_pi(void *rank)
{
    int thread_rank = *(int *)rank;
    double incr = 0;
    int sign;
    int k = thread_rank;

    if (k % 2)
    {
        sign = -1;
    }
    else
    {
        sign = 1;
    }

    td[thread_rank].sum = 4 * ((double)sign / (2 * thread_rank + 1));

    do
    {
        k += NUM_THREADS;

        if (k % 2)
        {
            sign = -1;
        }
        else
        {
            sign = 1;
        }

        incr = (double)sign / (2 * k + 1);
        td[thread_rank].sum += 4 * incr;
    } while (fabs(incr) > 1e-6);

    return NULL;
}

int main()
{
    int rank = 0;
    int err;
    double pi = 0;
    pthread_t thread_ids[NUM_THREADS];

    while (rank < NUM_THREADS)
    {
        td[rank].rank = rank;

        err = pthread_create(&(thread_ids[rank]), NULL, help_pi, (void *)&td[rank].rank);
        if (err != 0)
        {
            printf("Can't create thread error =%d\n", err);
            return 1;
        }
        rank++;
    }

    rank = 0;
    while (rank < NUM_THREADS)
    {
        pthread_join(thread_ids[rank], NULL);
        rank++;
    }

    rank = 0;
    while (rank < NUM_THREADS)
    {
        pi += td[rank].sum;
        rank++;
    }

    printf("%f\n", pi);

    return 0;
}