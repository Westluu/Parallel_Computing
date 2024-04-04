#include <unistd.h>
#include <stdio.h>

int main()
{
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN); // Get the number of logical cores
    if (nprocs < 0)
    {
        perror("Failed to get number of processors");
        return 1;
    }
    printf("Number of processor cores: %ld\n", nprocs);
    return 0;
}
