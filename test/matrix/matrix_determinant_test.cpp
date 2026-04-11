#define CATCH_CONFIG_MAIN
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../../sokobo/source/include/matrix.h"

#include "../../external/catch.hpp"
#include "utils.h"
using Catch::Matchers::Message;
using Catch::Matchers::StartsWith;

using namespace Catch::Matchers;

TEST_CASE("No square matrix - Determinant", "[determinant]")
{
  Matrix<double> M(2, 3);
  M(0, 0) = 1;
  M(0, 1) = 1;
  M(0, 2) = 4;
  M(1, 0) = 2;
  M(1, 1) = 4;
  M(1, 2) = 9;

  std::cout << "--- TEST: Determinant No Square Matrix ---\n";
  printMatrix("M", M);

  try {
    M.determinant();
  } catch (const std::invalid_argument& e) {
    CHECK(std::string(e.what())
          == "Determinant is only defined for square matrices");
  }
}

TEST_CASE("Determinant for matrices", "[determinant]")
{
  SECTION("2x2 Matrix Determiant")
  {
    Matrix<double> M(2, 2);
    M(0, 0) = 1;
    M(0, 1) = 3;
    M(1, 0) = -3;
    M(1, 1) = -5;

    std::cout << "--- TEST: 2x2 Determinant matrix ---\n";
    printMatrix("M (2x2)", M);

    double result = M.determinant();
    double expected = 4;

    CHECK(result == expected);
  }
  SECTION("3x3 Matrix Determinant")
  {
    Matrix<float> A(3, 3);
    A[0][0] = 1;
    A[0][1] = 2;
    A[0][2] = 3;
    A[1][0] = 0;
    A[1][1] = 1;
    A[1][2] = 4;
    A[2][0] = 5;
    A[2][1] = 6;
    A[2][2] = 0;
    std::cout << "--- TEST: 3x3 Determinant matrix ---\n";
    printMatrix("A (3x3)", A);

    double result = A.determinant();
    double expected = 1;

    CHECK(result == expected);
  }
  SECTION("4x4 Matrix Determinant")
  {
    Matrix<float> A(4, 4);
    A[0][0] = 1;
    A[0][1] = 2;
    A[0][2] = 3;
    A[0][3] = 3;
    A[1][0] = 0;
    A[1][1] = 1;
    A[1][2] = 4;
    A[1][3] = 4;
    A[2][0] = 5;
    A[2][1] = 6;
    A[2][2] = 0;
    A[2][3] = 0;
    A[3][0] = 5;
    A[3][1] = 6;
    A[3][2] = 0;
    A[3][3] = 0;
    std::cout << "--- TEST: 4x4 Determinant matrix ---\n";
    printMatrix("A (4x4)", A);

    double result = A.determinant();
    double expected = 0;

    CHECK(result == expected);
  }

  SECTION("4x4 Matrix Determinant")
  {
    Matrix<float> A(5, 5);
    A[0][0] = 4;
    A[0][1] = 8;
    A[0][2] = 7;
    A[0][3] = 1;
    A[0][4] = 4;

    A[1][0] = 8;
    A[1][1] = 3;
    A[1][2] = 9;
    A[1][3] = 6;
    A[1][4] = 9;

    A[2][0] = 2;
    A[2][1] = 3;
    A[2][2] = 2;
    A[2][3] = 6;
    A[2][4] = 7;

    A[3][0] = 0;
    A[3][1] = 1;
    A[3][2] = 6;
    A[3][3] = 1;
    A[3][4] = 7;

    A[4][0] = 7;
    A[4][1] = 3;
    A[4][2] = 5;
    A[4][3] = 2;
    A[4][4] = 1;

    std::cout << "--- TEST: 5x5 Determinant matrix ---\n";
    printMatrix("A (5x5)", A);

    double result = A.determinant(); // aprox  -347.9979553223
    double expected = -348;

    CHECK(std::round(result) == expected);
  }
}
