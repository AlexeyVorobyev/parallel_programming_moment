#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <chrono>


using namespace std;

int main() {
    setlocale(LC_ALL, "rus");
    int n = 1000;
    int digitals[1000]; // объявили массив на 10 ячеек

    omp_set_num_threads(6);

    cout << "Массив для сортировки:\n";
    for (int i = 0; i < n; i++) {
        digitals[i] = rand() % 10 + 2;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for( int i = 0; i < n; i++ )
    {
        int first = i % 2;
        #pragma omp parallel for default(none),shared(digitals,first, n)
        for( int j = first; j < n-1; j += 2 )
        {
            if( digitals[ j ] > digitals[ j+1 ] )
            {
                std::swap( digitals[ j ], digitals[ j+1 ] );
            }
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();

    cout << "\nМассив в отсортированном виде:\n";

    for (int i = 0; i < n; i++) {
        cout << digitals[i] << " "; // выводим элементы массива
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    printf("The time: %.8ld microseconds\n", duration.count());//время работы

    return 0;
}