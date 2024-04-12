// The rules of Conway’s Game of Life are as follows:

// If a cell has fewer than 2 ALIVE neighbors, it will be DEAD in the next time step.
// If an ALIVE cell has 2 or 3 ALIVE neighbors, it will be ALIVE in the next time step.
// If a cell has more than 3 ALIVE neighbors, it will be DEAD in the next time step.
// If a DEAD cell has 3 ALIVE neighbors, it will be ALIVE in the next time step.

// 0 dead, 1 alive, for displaying uint8_t

// #include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define GRID_SIZE 20
#define NUM_OF_ITERATIONS 100

#define TRUE 1
#define FALSE 0

// initalizes grid with random dead or alive cells
void gen_grid(uint8_t (*p_grid)[GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            p_grid[i][j] = (uint8_t)(rand() % 2);
        }
    }
}

void glider_gen(uint8_t (*p_grid)[GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            p_grid[i][j] = FALSE;
        }
    }
    p_grid[0][0] = TRUE;
    p_grid[0][2] = TRUE;
    p_grid[1][1] = TRUE;
    p_grid[1][2] = TRUE;
    p_grid[2][1] = TRUE;
}


uint8_t countNeighbors(int i, int j, uint8_t (*p_grid)[GRID_SIZE])
{
    int sum = 0;
    sum += p_grid[(i - 1 + GRID_SIZE) % GRID_SIZE][(j - 1 + GRID_SIZE) % GRID_SIZE];
    sum += p_grid[(i - 1 + GRID_SIZE) % GRID_SIZE][j];
    sum += p_grid[(i - 1 + GRID_SIZE) % GRID_SIZE][(j + 1) % GRID_SIZE];
    sum += p_grid[i][(j - 1 + GRID_SIZE) % GRID_SIZE];
    sum += p_grid[i][(j + 1) % GRID_SIZE];
    sum += p_grid[(i + 1) % GRID_SIZE][(j - 1 + GRID_SIZE) % GRID_SIZE];
    sum += p_grid[(i + 1) % GRID_SIZE][j];
    sum += p_grid[(i + 1) % GRID_SIZE][(j + 1) % GRID_SIZE];
    return sum;
}

void calculateGrid(uint8_t (*c_grid)[GRID_SIZE], uint8_t (*p_grid)[GRID_SIZE])
// Takes in previous grid and populates entire current grid off of it.
{
#pragma omp parallel for num_threads(8) //private(i,j)
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            switch (countNeighbors(i, j, p_grid))
            {
            // case 0:
            // case 1:
            //     current_grid[i][j] = FALSE;
            //     break;
            case 2:
                c_grid[i][j] = p_grid[i][j];
                break;
            case 3:
                c_grid[i][j] = TRUE;
                break;
            default:
                c_grid[i][j] = FALSE;
            }
        }
    }
}

void printGrid(uint8_t (*c_grid)[GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        putchar('|');
        for (int j = 0; j < GRID_SIZE; j++)
        {
            // printf("%d", c_grid[i][j]);
            // char value = (char) (current_grid[i][j] * 64);
            putchar((char)((c_grid[i][j] * 32) + 32));
        }
        putchar('|');
        putchar('\n');
    }
    putchar('\n');
}

int main()
{
    // uint8_t temp[GRID_SIZE][GRID_SIZE], c_grid[GRID_SIZE][GRID_SIZE], p_grid[GRID_SIZE][GRID_SIZE];
    uint8_t current_grid[GRID_SIZE][GRID_SIZE], previous_grid[GRID_SIZE][GRID_SIZE];

    uint8_t(*c_grid)[GRID_SIZE][GRID_SIZE] = &current_grid;
    uint8_t(*p_grid)[GRID_SIZE][GRID_SIZE] = &previous_grid;

    uint8_t(*temp)[GRID_SIZE][GRID_SIZE] = NULL;

    // uint8_t(*current_grid)[GRID_SIZE] = c_grid;
    // uint8_t(*previous_grid)[GRID_SIZE] = p_grid;

    // initalizes the grids
    gen_grid(*p_grid);
    // glider_gen(*p_grid);

    double start;
    double end;
    start = omp_get_wtime();   

    for (int i = 0; i < NUM_OF_ITERATIONS; i++)
    {
        calculateGrid(*c_grid, *p_grid);
        // printGrid(*c_grid);
        // printf("---------------------------------\n");
        // printf("%d\n", (int)*c_grid);
        // printf("%d\n", (int)*p_grid);
        // swapGrids(c_grid, p_grid);
        temp = p_grid;
        p_grid = c_grid;
        c_grid = temp;
    }
    // printGrid(*c_grid);
    
    end = omp_get_wtime();
    printf("Work took %f seconds\n", end - start);
}