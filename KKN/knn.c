#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>

#define NUM_THREADS 4
// #define N 1000000
#define N 10

struct Point
{
	int val;	 // Group of point
	double x, y;	 // Co-ordinate of point
	double distance; // Distance from test point
} typedef Point;


Point arr[N];


void gen_array() {
	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < N; i++) {
		arr[i].x = rand();
		arr[i].y = rand();
		arr[i].val = rand() % 2;
	}
	
}




void mergeSort(Point arr[], int size) {
	//Main merge sort call, call this and will take care of everything
	int depth = 0;
	uint8_t x = NUM_THREADS;
	while (x >>= 1) depth++; //Log base two to find, basically just number of right shift (/2s) is a log
	mergeSortHelpPrl(arr, size, depth);
}


void mergeSortHelp(Point arr[], int size) { //ADD A BASE CASE STUPID
	Point arr_result[size];

	if (size == 1) {
		return;
	} else {
		mergeSortHelp(arr, size / 2);
		mergeSortHelp(&arr[size/2], size - (size / 2));
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
			right_top++;
		}
	}

	memcpy(arr, arr_result, sizeof(Point) * size);
}

void mergeSortHelpPrl(Point arr[], int size, int depth) {
	Point arr_result[size];

	if (depth-- > 0) {
		#pragma omp parallel
		{
			#pragma omp task
			mergeSortHelpPrl(arr, size / 2, depth);
			#pragma omp task
			mergeSortHelpPrl(&arr[size/2], size - (size / 2), depth);
		}
	} else {
		mergeSortHelp(arr, size / 2);
		mergeSortHelp(&arr[size/2], size - (size / 2));
	}
	
	#pragma omp barrier

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
			right_top++;
		}
	}

	memcpy(arr, arr_result, sizeof(Point) * size);
}

void calc_distance(Point p) {
	// Fill distances of all points from p
	#pragma omp parallel for num_threads(NUM_THREADS) 
	for (int i = 0; i < N; i++) {
		arr[i].distance =
			sqrt((arr[i].x - p.x) * (arr[i].x - p.x) +
				(arr[i].y - p.y) * (arr[i].y - p.y));
	}
}


// This function finds classification of point p using
// k nearest neighbour algorithm. It assumes only two
// groups and returns 0 if p belongs to group 0, else
// 1 (belongs to group 1).
int classifyAPoint(Point arr[], int n, int k, Point p)
{
	
	calc_distance(p);
	//parallel sort
	mergeSort(arr, n);

	for (int i=0; i < N; i++) {
		printf('%f', arr[i].distance);
	}
	


	// Now consider the first k elements and only
	// two groups
	// int freq1 = 0;	 // Frequency of group 0
	// int freq2 = 0;	 // Frequency of group 1
	
	// for (int i = 0; i < k; i++)
	// {
	// 	if (arr[i].val == 0)
	// 		freq1++;
	// 	else if (arr[i].val == 1)
	// 		freq2++;
	// }

	// return (freq1 > freq2 ? 0 : 1);
}

// Driver code
int main()
{
	// arr[0].x = 1;
	// arr[0].y = 12;
	// arr[0].val = 0;

	// arr[1].x = 2;
	// arr[1].y = 5;
	// arr[1].val = 0;

	// arr[2].x = 5;
	// arr[2].y = 3;
	// arr[2].val = 1;

	// arr[3].x = 3;
	// arr[3].y = 2;
	// arr[3].val = 1;

	// arr[4].x = 3;
	// arr[4].y = 6;
	// arr[4].val = 0;

	// arr[5].x = 1.5;
	// arr[5].y = 9;
	// arr[5].val = 1;

	// arr[6].x = 7;
	// arr[6].y = 2;
	// arr[6].val = 1;

	// arr[7].x = 6;
	// arr[7].y = 1;
	// arr[7].val = 1;

	// arr[8].x = 3.8;
	// arr[8].y = 3;
	// arr[8].val = 1;

	// arr[9].x = 3;
	// arr[9].y = 10;
	// arr[9].val = 0;

	// arr[10].x = 5.6;
	// arr[10].y = 4;
	// arr[10].val = 1;

	// arr[11].x = 4;
	// arr[11].y = 2;
	// arr[11].val = 1;

	// arr[12].x = 3.5;
	// arr[12].y = 8;
	// arr[12].val = 0;

	// arr[13].x = 2;
	// arr[13].y = 11;
	// arr[13].val = 0;

	// arr[14].x = 2;
	// arr[14].y = 5;
	// arr[14].val = 1;

	// arr[15].x = 2;
	// arr[15].y = 9;
	// arr[15].val = 0;

	// arr[16].x = 1;
	// arr[16].y = 7;
	// arr[16].val = 0;

	gen_array();
	
	/*Testing Point*/
	Point p;
	p.x = 2.5;
	p.y = 7;

	// Parameter to decide group of the testing point
	int k = 3;
	printf ("The value classified to unknown point"
			" is %d.\n", classifyAPoint(arr, N, k, p));
	return 0;
}
