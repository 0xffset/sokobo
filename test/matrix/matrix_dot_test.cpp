
#define CATCH_CONFIG_MAIN
#include "../../sokobo/source/include/matrix.h"
#include "../../external/catch.hpp"
#include <iostream>
#include <vector>

using Catch::Matchers::Message;
using Catch::Matchers::StartsWith;

using namespace Catch::Matchers;

Matrix<int> int_matrix;
Matrix<double> double_matrix;

TEST_CASE("Matrix::dot - 1x1 Non-Zero Vectors", "[matrix][dot]") {
    std::vector<int> a = {5};
    std::vector<int> b = {3};
    REQUIRE(int_matrix.dot(a, b) == 15);
}

TEST_CASE("Matrix::dot - Standard Integer Vectors", "[matrix][dot]") {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    REQUIRE(int_matrix.dot(a, b) == 32);
}

TEST_CASE("Matrix::dot - Floating Point Precision", "[matrix][dot]") {
    std::vector<double> a = {1.5, 2.5};
    std::vector<double> b = {2.0, 4.0};
    REQUIRE(double_matrix.dot(a, b) == 13.0);
}

TEST_CASE("Matrix::dot - Orthogonal Vectors (Zero Result)", "[matrix][dot]") {
    std::vector<int> a = {1, 0};
    std::vector<int> b = {0, 1};
    REQUIRE(int_matrix.dot(a, b) == 0);
}

TEST_CASE("Matrix::dot - Negative Elements", "[matrix][dot]") {
    std::vector<int> a = {-1, -2, 3};
    std::vector<int> b = {4, -5, -6};
    REQUIRE(int_matrix.dot(a, b) == -12);
}

TEST_CASE("Matrix::dot - Empty Vectors", "[matrix][dot]") {
    std::vector<int> a = {};
    std::vector<int> b = {};
    REQUIRE(int_matrix.dot(a, b) == 0);
}

TEST_CASE("Matrix::dot - Large Elements (No Overflow Check)", "[matrix][dot]") {
    std::vector<int> a = {1000, 2000};
    std::vector<int> b = {3000, 4000};
    REQUIRE(int_matrix.dot(a, b) == 11000000);
}

TEST_CASE("Matrix::dot - Zero Vectors", "[matrix][dot]") {
    std::vector<int> a = {0, 0, 0};
    std::vector<int> b = {0, 0, 0};
    REQUIRE(int_matrix.dot(a, b) == 0);
}
