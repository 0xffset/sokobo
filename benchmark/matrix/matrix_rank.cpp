#include <random>

#include "../source/include/matrix.h"

#include <benchmark/benchmark.h>

#include "../source/complex_number.cpp"
#include "../source/include/complex_number.h"
#include "../source/matrix.cpp"

template<typename T>
void fill_matrix(Matrix<T>& matrix)
{
  for (int i = 0; i < matrix.getRows(); ++i) {
    for (int j = 0; j < matrix.getCols(); ++j) {
      if (i == j) {
        matrix(i, j) = static_cast<T>(1000);
      } else {
        matrix(i, j) = static_cast<T>(1);
      }
    }
  }
}

static void rank_BM(benchmark::State& state)
{
  const int size = state.range(0);

  Matrix<double> matrix(size, size);
  fill_matrix(matrix);
  for (auto _ : state) {
    benchmark::DoNotOptimize(matrix.rank());
  }

  state.SetBytesProcessed(
      uint64_t(state.iterations() * size * size * sizeof(double)));
  state.SetComplexityN(size);
}

BENCHMARK(rank_BM)->RangeMultiplier(2)->Range(8, 2024)->Complexity();
BENCHMARK_MAIN();
