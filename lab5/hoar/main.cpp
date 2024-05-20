/* C implementation QuickSort */
#include<stdio.h>
#include<omp.h>
#include<math.h>
#include <stdlib.h>

#define ARRAY_MAX_SIZE 1000000

int arr[ARRAY_MAX_SIZE];

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
	array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort_parallel_omp(int arr[], int low, int high)
{
    int pi;
    if (low < high)
    {
        pi = partition(arr, low, high);
        omp_set_nested(1);
#pragma omp parallel sections
        {
#pragma omp section
            quickSort_parallel_omp(arr, low, pi - 1);
#pragma omp section
            quickSort_parallel_omp(arr, pi + 1, high);
        }
    }
}


void quickSort_Optparallel_omp(int arr[], int low, int high)   //optimized version
{
    int pi;
    if (low < high)
    {
        pi = partition(arr, low, high);
        omp_set_nested(1);
#pragma omp parallel sections
        {
#pragma omp section
            quickSort_Optparallel_omp(arr, low, pi - 1);
#pragma omp section
            quickSort_Optparallel_omp(arr, pi + 1, high);
        }
    }
}

void quickSort_tasks(int arr[], int low, int high)
{
    int pi;
    if (low < high)
    {
        pi = partition(arr, low, high);
#pragma omp task
        quickSort_Optparallel_omp(arr, low, pi - 1);
#pragma omp task
        quickSort_Optparallel_omp(arr, pi + 1, high);
    }
}

void quickSort_omp(int arr[], int low, int high) {
#pragma omp parallel
#pragma omp single
    quickSort_tasks(arr,low,high);
    // implicit synchronization at the end of parallel section
    // otherwise use
    // #pragma omp taskwait
}

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Driver program to test above functions
int main()
{
    int nthreads;

    for(nthreads=1; nthreads <= 2; nthreads++) {
        //Define different number of threads
        omp_set_num_threads(nthreads);

        double start_time, run_time;
        for( int i = 0; i < ARRAY_MAX_SIZE-1; i++ )
        {
            arr[i] = rand() % 50 +1;
        }
        int n = sizeof(arr)/sizeof(arr[0]);

        start_time = omp_get_wtime();
        #pragma omp parallel
        {

            #pragma omp single nowait
            quickSort_omp(arr, 0, n-1);
        }
        run_time = omp_get_wtime() - start_time;
        printf("\n Execution time was %lf seconds\n ",run_time);
        printf("\n");
    }
    return 0;
}