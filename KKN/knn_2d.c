#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>

#define NUM_THREADS 4
// #define N 10000000
#define N 256

struct Point
{
	int val;	 // Group of point
	int x, y;	 // Co-ordinate of point
	double distance; // Distance from test point
} typedef Point;


Point A[N][N];


void gen_array() {
	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j].x = i;
			A[i][j].y = j;
			A[i][j].val = rand() % 256;
		}
	}
}


void mergeSortHelp(Point arr[], int size) { //ADD A BASE CASE STUPID
	// printf("Size: %d\n", size);
	// Point arr_result[size];
	Point *arr_result = malloc(size * sizeof(Point));
	

	if (size == 1) {
		return;
	} else {
		mergeSortHelp(arr, size / 2);
		mergeSortHelp(&arr[size / 2], size - (size / 2));
	}

	int left_top = 0;
	int right_top = size / 2;
	
	for (int i = 0; i < size; i++) {
		if (left_top >= size / 2) { //check if left has values to go for
			memcpy(&arr_result[i], &arr[right_top], sizeof(Point) * (size - right_top));
			break;
		} 
		else if (right_top >= size) { //same check for right
			memcpy(&arr_result[i], &arr[left_top], sizeof(Point) * (size / 2 - left_top));
			break;
		} 
		else if (arr[left_top].distance < arr[right_top].distance) { //in the cases for both chunks still have things
			arr_result[i] = arr[left_top];
			left_top++;
		} 
		else {
			arr_result[i] = arr[right_top];
			right_top++;
		}
	}

	memcpy(arr, arr_result, sizeof(Point) * size);

	// for (int i=0; i < size; i++) {
	// 	printf("%.2f ", arr[i].distance);
	// }
	// putchar('\n');
	
	free(arr_result);
}


void mergeSortHelpPrl(Point arr[], int size, int depth) {
	// printf("Size: %d\n", size);
	// Point arr_result[size];
	Point *arr_result = malloc(size * sizeof(Point));

	if (depth-- > 0) {
		// #pragma omp parallel
		// #pragma omp single nowait
		#pragma omp parallel sections
		{
			// #pragma omp task
			#pragma omp section
			mergeSortHelpPrl(arr, size / 2, depth);
			// #pragma omp task
			#pragma omp section
			mergeSortHelpPrl(&arr[size / 2], size - (size / 2), depth);
			// #pragma omp barrier
		}
		// #pragma omp taskwait
	} else {
		mergeSortHelp(arr, size / 2);
		mergeSortHelp(&arr[size / 2], size - (size / 2));
	}

	int left_top = 0;
	int right_top = size / 2;
	
	for (int i = 0; i < size; i++) {
		if (left_top >= size / 2) { //check if left has values to go for
			memcpy(&arr_result[i], &arr[right_top], sizeof(Point) * (size - right_top));
			break;
		} 
		else if (right_top >= size) { //same check for right
			memcpy(&arr_result[i], &arr[left_top], sizeof(Point) * (size / 2 - left_top));
			break;
		} 
		else if (arr[left_top].distance < arr[right_top].distance) { //in the cases for both chunks still have things
			arr_result[i] = arr[left_top];
			left_top++;
		} 
		else {
			arr_result[i] = arr[right_top];
			right_top++;
		}
		// printf("Put %.2f into %d\n", arr_result[i].distance, i);
	}

	memcpy(arr, arr_result, sizeof(Point) * size);

	// for (int i=0; i < size; i++) {
	// 	printf("%.2f ", arr[i].distance);
	// }
	// putchar('\n');
	free(arr_result);
}


void mergeSort(Point *arr, int size) {
	//Main merge sort call, call this and will take care of everything
	int depth = 0;
	uint8_t x = NUM_THREADS;
	while (x >>= 1) depth++; //Log base two to find, basically just number of right shift (/2s) is a log
	mergeSortHelpPrl(arr, size, depth);
}


void calc_distance(Point p) {
	// Fill distances of all points from p
	#pragma omp parallel for num_threads(NUM_THREADS) 
	for (int i = 0; i < N; i++) {
		A[i]->distance =
			sqrt((double)((A[i]->x - p.x) * (A[i]->x - p.x) +
				(A[i]->y - p.y) * (A[i]->y - p.y)));
	}
}

int get_max(uint8_t a[]){
	uint8_t max = 0;
	for (int i=0; i < N; i++) {
		if (a[i] > max){
			max = a[i];
		}
	}
	return max;
}

// This function finds classification of point p using
// k nearest neighbour algorithm. It assumes only two
// groups and returns 0 if p belongs to group 0, else
// 1 (belongs to group 1).
int classifyAPoint(Point arr[], int n, int k, Point p)
{
	double start, calc_end, merge_end, full_end;

	start = omp_get_wtime();
	calc_distance(p);

	// printf("DISTANCE DONE\n");
	// for (int i=0; i < N; i++) {
	// 	printf("%.2f ", arr[i].distance);
	// }
	// putchar('\n');

	calc_end = omp_get_wtime();
	mergeSort(arr, n * n);
	// printf("MERGE DONE\n");

	// for (int i=0; i < N; i++) {
	// 	printf("%.2f ", arr[i].distance);
	// }
	// putchar('\n');
	
	// for (int i=0; i < N; i++) {
	// 	printf("%d ", arr[i].val);
	// }
	// putchar('\n');

	// Now consider the first k elements and only
	// two groups
	
	merge_end = omp_get_wtime();
	
	uint8_t color[N] = {0};
	#pragma omp parallel for num_threads(NUM_THREADS) reduction(+: color)
	for (int i = 0; i < k; i++)
	{
		color[arr[i].val]++;
		// if (arr[i].val == 0)
		// 	freq1++;
		// else if (arr[i].val == 1)
		// 	freq2++;
	}

	// printf("PARALLEL freq1: %d\n", freq1);
	// printf("PARALLEL freq2: %d\n", freq2);

	full_end = omp_get_wtime();
	printf("Distance: %.4f | Merge: %.4f | Count: %.4f | Total: %.4f\n",
	 calc_end - start, merge_end - calc_end, full_end - merge_end, full_end - start);
	

	// freq1 = 0;	 // Frequency of group 0
	// freq2 = 0;	 // Frequency of group 1
	// for (int i = 0; i < k; i++)
	// {
	// 	if (arr[i].val == 0)
	// 		freq1++;
	// 	else if (arr[i].val == 1)
	// 		freq2++;
	// }

	// printf("SEQ freq1: %d\n", freq1);
	// printf("SEQ freq2: %d\n", freq2);
	// return (freq1 > freq2 ? 0 : 1);
	return get_max(color);
}

// Driver code
int main()
{
	gen_array();
	
	// for (int i=0; i < N; i++) {
	// 	printf("%.3f\n", arr[i].x);
	// }
	// printf("Array Gen Done\n");
	
	/*Testing Point*/
	Point p;
	p.x = (double)rand() / RAND_MAX;
	p.y = (double)rand() / RAND_MAX;

	// Parameter to decide group of the testing point
	int k = 50;
	printf ("The value classified to unknown point"
			" is %d.\n", classifyAPoint(A, N, k, p));
	return 0;
}
