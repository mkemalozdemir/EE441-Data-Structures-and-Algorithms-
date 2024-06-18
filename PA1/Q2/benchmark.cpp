#include <iostream>
#include <chrono>
#include "complexity.hpp"
#include "hanoi.hpp"

void benchmark(void (*func)(int))
{
    int const AMOUNT = 20;
    int const REPEAT = 10;
    double results[AMOUNT];
    for (int i = 0 ; i < AMOUNT ; ++i) {
        auto start = std::chrono::steady_clock::now();
        for (int j = 0 ; j < REPEAT ; ++j) {
            func(i+1);
        }
        auto finish = std::chrono::steady_clock::now();
        results[i] = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count() / REPEAT;
    }

    std::cout << std::endl << std::endl;
    std::cout << "n\ttime (ns)\n";
    for (int i = 0 ; i < AMOUNT ; ++i) {
        std::cout << i+1 << '\t' << results[i] << '\n';
    }
    std::cout << std::endl;
}

void wrapper_solve_hanoi(int n)
{
    auto game = Hanoi(n);
    solve_hanoi(game);
}

void wrapper_print_backwards(int n)
{
    char * str = new char[n+1];
    for (int i = 0 ; i < n ; ++i) {
        str[i] = 'a' + i;
    }
    str[n] = '\0';

    print_backwards(str);

    delete[] str;
}

void wrapper_gcd(int n)
{
    volatile int temp = gcd(n, 60);
}

void wrapper_lcm(int n)
{
    volatile int temp = lcm(n, 60);
}

int main()
{
    benchmark(wrapper_solve_hanoi);
    benchmark(wrapper_print_backwards);
    benchmark(wrapper_gcd);
	benchmark(wrapper_lcm);
}
