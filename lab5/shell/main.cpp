#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string>
#include <omp.h>


void insertionsort(int a[], int n, int stride) {
    for (int j=stride; j<n; j+=stride) {
        int key = a[j];
        int i = j - stride;
        while (i >= 0 && a[i] > key) {
            a[i+stride] = a[i];
            i-=stride;
        }
        a[i+stride] = key;
    }
}

void shellsort(int a[], int n)
{
    int i, m;

    for(m = n/2; m > 0; m /= 2)
    {
        #pragma omp parallel for shared(a,m,n) private (i) default(none)
        for(i = 0; i < m; i++)
            insertionsort(&(a[i]), n-i, m);
    }
}

void printlist(std::string s, int a[], int n) {
    printf("%s\n",s.c_str());
    for (int i=0; i<n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int checklist(int a[], int n) {
    int result = 0;
    for (int i=0; i<n; i++) {
        if (a[i] != i) {
            result++;
        }
    }
    return result;
}

void seedprng() {
    struct timeval t;

    /* seed prng */
    gettimeofday(&t, NULL);
    srand((unsigned int)(1000000*(t.tv_sec)+t.tv_usec));
}

int main(int argc, char **argv) {
    const int n=30000000;
    int nthreads;

    for(nthreads=1; nthreads <= 10; nthreads++) {
        double start_time = omp_get_wtime();

        int *data;
        int missorted;
        //Define different number of threads
        omp_set_num_threads(nthreads);

        data = (int *)malloc(n*sizeof(int));
        for (int i=0; i<n; i++)
            data[i] = i;

        seedprng();
        /* shuffle */
        for (int i=0; i<n; i++) {
            int i1 = rand() % n;
            int i2 = rand() % n;
            int tmp = data[i1];
            data[i1] = data[i2];
            data[i2] = tmp;
        }

//        printlist("Unsorted List:",data,n);

        shellsort(data,n);

//        printlist("Sorted List:",data,n);
        missorted = checklist(data,n);
        if (missorted != 0) printf("%d missorted nubmers\n",missorted);

        double time = omp_get_wtime() - start_time;
        printf("Total time for thread %d (in sec):%.2f\n", nthreads, time);
    }

    return 0;
}