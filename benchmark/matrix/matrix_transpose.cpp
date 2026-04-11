#include <benchmark/benchmark.h>
#include <random>
#include "../source/include/matrix.h"
#include "../source/include/complex_number.h"
#include "../source/matrix.cpp"
#include "../source/complex_number.cpp"

template<typename T>
void fill_m(Matrix<T>& mat)
{
  for (int i = 0; i < mat.getRows(); ++i) {
    for (int j = 0; mat.getCols(); ++j) {
      mat(i, j) = static_cast<T>(i, j);
    }
  }
}

static void transpose_BM(benchmark::State& state)
{
  const int size = state.range(0);
  Matrix<double> m(size, size);

  fill_m(m);
  for (auto _ : state) {
    benchmark::DoNotOptimize(m.transpose());
  }
  state.SetBytesProcessed(
      uint64_t(state.iterations() * size * size * sizeof(double)));
  state.SetComplexityN(size);
}

BENCHMARK(transpose_BM)->RangeMultiplier(2)->Range(8, 2024)->Complexity();

BENCHMARK_MAIN();