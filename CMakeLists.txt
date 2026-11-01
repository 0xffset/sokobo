
cmake_minimum_required(VERSION 3.20)

project(
    sokobo
    VERSION 0.1.0
    DESCRIPTION "C++ Computational Algebra CLI"
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(sokobo_lib
    source/calculus.cpp
    source/cli.cpp
    source/complex_number.cpp
    source/differential_equations.cpp
    source/expression.cpp
    source/fourier.cpp
    source/laplace.cpp
    source/matrix.cpp
    source/numerical_methods.cpp
    source/polynomial.cpp
    source/vector.cpp
)

target_include_directories(sokobo_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/include
)

if(MSVC)
    target_compile_options(sokobo_lib PRIVATE /W4)
else()
    target_compile_options(sokobo_lib PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

# Main executable
add_executable(sokobo
    source/main.cpp
)

target_link_libraries(sokobo PRIVATE sokobo_lib)

# ============================================================
# Google Benchmark
# ============================================================

include(FetchContent)
set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
option(BUILD_BENCHMARKS "Build performance benchmarks" ON)

if(BUILD_BENCHMARKS)

    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(googlebenchmark)

    function(add_matrix_benchmark BENCHMARK_NAME SOURCE_FILE)

        add_executable(${BENCHMARK_NAME} ${SOURCE_FILE})

        target_link_libraries(${BENCHMARK_NAME}
            PRIVATE
                sokobo_lib
                benchmark::benchmark
        )

    endfunction()

    add_matrix_benchmark(MatrixDeterminantBenchmark
        benchmark/matrix/matrix_determinant.cpp
    )

    add_matrix_benchmark(MatrixInverseBenchmark
        benchmark/matrix/matrix_inverse.cpp
    )

    add_matrix_benchmark(MatrixRankBenchmark
        benchmark/matrix/matrix_rank.cpp
    )

    add_matrix_benchmark(MatrixTransposeBenchmark
        benchmark/matrix/matrix_transpose.cpp
    )

endif()

# ============================================================
# Matrix Unit Tests
# ============================================================

function(add_matrix_test TEST_NAME SOURCE_FILE)
    add_executable(${TEST_NAME} ${SOURCE_FILE})

    target_link_libraries(${TEST_NAME}
        PRIVATE sokobo_lib
    )

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )
endfunction()

add_matrix_test(MatrixTests
    test/matrix/matrix_test.cpp
)

add_matrix_test(MatrixBuildTriangularTests
    test/matrix/matrix_build_triangular_test.cpp
)

add_matrix_test(MatrixDeterminantTests
    test/matrix/matrix_determinant_test.cpp
)

add_matrix_test(MatrixDotTests
    test/matrix/matrix_dot_test.cpp
)

add_matrix_test(MatrixEigenvectorSymmetricTests
    test/matrix/matrix_eigenvector_symmetric_test.cpp
)

add_matrix_test(MatrixFrobeniusNormTests
    test/matrix/matrix_frobenius_norm_test.cpp
)

add_matrix_test(MatrixInverseTests
    test/matrix/matrix_inverse_test.cpp
)

add_matrix_test(MatrixLanczosTests
    test/matrix/matrix_lanczos_test.cpp
)

add_matrix_test(MatrixNormTests
    test/matrix/matrix_norm_test.cpp
)

add_matrix_test(MatrixQRDecompositionTests
    test/matrix/matrix_qr_decomposition_test.cpp
)

add_matrix_test(MatrixRankTests
    test/matrix/matrix_rank_test.cpp
)

add_matrix_test(MatrixSpectralNormTests
    test/matrix/matrix_spectral_norm_test.cpp
)

add_matrix_test(MatrixSpectralRadiusTests
    test/matrix/matrix_spectral_radius_test.cpp
)

add_matrix_test(MatrixTransposeTests
    test/matrix/matrix_transpose_test.cpp
)
