#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;

long double func(long double x) {
    //  f(x)
    return sin(x) + cos(x) * 20;
}

long double simpson(double a, double b, int n) {
    long double h = (b - a) / n;
    long double sum = func(a) + func(b);

//    cout << h <<endl;
//    cout << func(a) <<endl;
//    cout << func(b) <<endl;
//    cout << sum <<endl;
//    cout << "------" <<endl;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < n; i += 2) {
        sum += 4 * func(a + i * h);
    }

    #pragma omp parallel for reduction(+:sum)
    for (int i = 2; i < n - 1; i += 2) {
        sum += 2 * func(a + i * h);
    }

    return sum * h / 3;
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();
    omp_set_num_threads(16);

    /**
     * ООФ ПО Б < 1
     * */

    long double a = -1000000; // Нижний предел интегрирования
    long double b = 1000000; // Верхний предел интегрирования
    long double tol = 0.00001; // Требуемая точность
    int n = 4; // Начальное количество отрезков


    long double integral_prev = simpson(a, b, n);
    long double integral_cur = simpson(a, b, 2 * n);
//    cout << integral_prev << endl;
    long double error = std::abs(integral_cur - integral_prev) / 15; // Формула Рунге
    while (error > tol) {
        n *= 2;
        integral_prev = integral_cur;
        integral_cur = simpson(a, b, 2 * n);
        cout << error << endl;
        error = std::abs(integral_cur - integral_prev) / 15;
    }


    std::cout << "I = " << int(integral_cur) << std::endl;//интеграл
    std::cout << "N = " << n << std::endl;//кол-во отрезков
    std::cout << "Error = " << error << std::endl;//погрешность

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    printf("The time: %.8ld microseconds\n", duration.count());//время работы

    return 0;
}