#include <benchmark/benchmark.h>
#include "../source/include/matrix.h"
#include "../source/include/complex_number.h"
#include "../source/matrix.cpp"
#include "../source/complex_number.cpp"

template<typename T>
void fill_m(Matrix<T>& mat)
{
    for (int i = 0; i < mat.getRows(); ++i)
    {
        for (int j = 0; j < mat.getCols(); ++j)
        {
            if (i == j)
                mat(i, j) = static_cast<T>(1000);
            else
                mat(i, j) = static_cast<T>(1);
        }
    }
}
static void determinant_BM(benchmark::State& state)
{
  const int size = state.range(0);
  Matrix<double> m(size, size);

  fill_m(m);

  for (auto _ : state) {
    benchmark::DoNotOptimize(m.determinant());
  }

 state.SetBytesProcessed(
    int64_t(state.iterations()) *
    int64_t(size) *
    int64_t(size) *
    sizeof(double));
  state.SetComplexityN(size);
}

BENCHMARK(determinant_BM)->RangeMultiplier(2)->Range(8, 2024)->Complexity();

BENCHMARK_MAIN();
