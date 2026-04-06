#include <benchmark/benchmark.h>
#include <random>
#include "../source/include/matrix.h"
#include "../source/include/complex_number.h"
#include "../source/matrix.cpp"
#include "../source/complex_number.cpp"

template<typename T>
Matrix<T> generate_invertible_matrix(int n)
{
    Matrix<T> m(n, n);

    std::mt19937 rng(42);
    std::uniform_real_distribution<T> dist(1.0, 10.0);

    for (int i = 0; i < n; ++i) {
        T row_sum = 0;
        for (int j = 0; j < n; ++j) {
            m(i, j) = dist(rng);
            row_sum += std::abs(m(i, j));
        }
        m(i, i) += row_sum;
    }

    return m;
}

static void BM_MatrixInverse(benchmark::State& state)
{
    int n = state.range(0);

    Matrix<double> m = generate_invertible_matrix<double>(n);

    for (auto _ : state) {
        auto inv = m.inverse();

        benchmark::DoNotOptimize(inv);
    }

    state.SetComplexityN(n);
}

BENCHMARK(BM_MatrixInverse)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Complexity(benchmark::oNCubed); // Gauss-Jordan ~ O(n^3)

static void BM_MatrixInverse_2x2(benchmark::State& state)
{
    Matrix<double> m(2, 2);
    m(0,0) = 4; m(0,1) = 7;
    m(1,0) = 2; m(1,1) = 6;

    for (auto _ : state) {
        auto inv = m.inverse();
        benchmark::DoNotOptimize(inv);
    }
}

BENCHMARK(BM_MatrixInverse_2x2);

static void BM_MatrixInverse_1x1(benchmark::State& state)
{
    Matrix<double> m(1, 1);
    m(0,0) = 5;

    for (auto _ : state) {
        auto inv = m.inverse();
        benchmark::DoNotOptimize(inv);
    }
}

BENCHMARK(BM_MatrixInverse_1x1);

BENCHMARK_MAIN();