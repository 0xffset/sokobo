#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"

#include "catch.hpp"

/* Adding two matrices */
TEST_CASE("add_two_matrices", "[matrix]")
{
  Matrix<int> matrix1(2, 2);
  Matrix<int> matrix2(2, 2);
  Matrix<int> expected(2, 2);

  matrix1[0][0] = 1;
  matrix1[0][1] = 2;
  matrix1[1][0] = 3;
  matrix1[1][1] = 4;

  matrix2[0][0] = 5;
  matrix2[0][1] = 6;
  matrix2[1][0] = 7;
  matrix2[1][1] = 8;

  expected[0][0] = 6;
  expected[0][1] = 8;
  expected[1][0] = 10;
  expected[1][1] = 12;

  Matrix<int> result = matrix1 + matrix2;

  REQUIRE(result.getRows() == expected.getRows());
  REQUIRE(result.getCols() == expected.getCols());

  for (int i = 0; i < result.getRows(); ++i) {
    for (int j = 0; j < result.getCols(); ++j) {
      REQUIRE(result[i][j] == expected[i][j]);
    }
  }
}

// Subtracting two matrices
TEST_CASE("subtract_two_matrices", "[matrix]")
{
  Matrix<int> a(2, 2);
  Matrix<int> b(2, 2);
  Matrix<int> expected(2, 2);

  a[0][0] = 1;
  a[0][1] = 2;
  a[1][0] = 3;
  a[1][1] = 4;

  b[0][0] = 2;
  b[0][1] = 0;
  b[1][0] = 1;
  b[1][1] = 2;

  expected[0][0] = -1;
  expected[0][1] = 2;
  expected[1][0] = 2;
  expected[1][1] = 2;

  Matrix<int> result = a - b;
  REQUIRE(result.getRows() == expected.getRows());
  REQUIRE(result.getCols() == expected.getCols());

  for (int i = 0; i < result.getRows(); ++i) {
    for (int j = 0; j < result.getCols(); ++j) {
      REQUIRE(result[i][j] == expected[i][j]);
    }
  }
}

// Multiplying two matrices
TEST_CASE("multiply_two_matrices", "[matrix]")
{
  Matrix<int> a(2, 2);
  Matrix<int> b(2, 2);
  Matrix<int> expected(2, 2);

  a[0][0] = 1;
  a[0][1] = 2;
  a[1][0] = 3;
  a[1][1] = 4;

  b[0][0] = 2;
  b[0][1] = 0;
  b[1][0] = 1;
  b[1][1] = 2;

  expected[0][0] = 4;
  expected[0][1] = 4;
  expected[1][0] = 10;
  expected[1][1] = 8;

  Matrix<int> result = a * b;

  REQUIRE(result.getRows() == expected.getRows());
  REQUIRE(result.getCols() == expected.getCols());

  for (int i = 0; i < result.getRows(); ++i) {
    for (int j = 0; j < result.getCols(); ++j) {
      REQUIRE(result[i][j] == expected[i][j]);
    }
  }
}

// Determinant matrix
TEST_CASE("determinant_matrix", "[matrix]")
{
  Matrix<int> a(2, 2);

  a[0][0] = 4;
  a[0][1] = 1;
  a[1][0] = 6;
  a[1][1] = 2;

  int result = a.determinant();
  int expected = 2;

  REQUIRE(result == expected);
}

// Eigenvalues of a matrix
TEST_CASE("eigenvalues of a 2x2 matrix", "[matrix]")
{
  Matrix<int> a(2, 2);
  a[0][0] = 1;
  a[0][1] = 2;
  a[1][0] = 3;
  a[1][1] = 4;

  int expected_lambada_1 = 5;
  int expected_lambada_2 = 0;

  std::vector<int> result = a.eigenvalues();

  REQUIRE(result.size() == 2);
  REQUIRE(expected_lambada_1 == result[0]);
  REQUIRE(expected_lambada_2 == result[1]);
}

TEST_CASE("eigenvalues of 3x3 matrix", "[matrix]")
{
  Matrix<float> a(3, 3);
  a[0][0] = 2;
  a[0][1] = 4;
  a[0][2] = 6;
  a[1][0] = 8;
  a[1][1] = 12;
  a[1][2] = 16;
  a[2][0] = 10;
  a[2][1] = 12;
  a[2][2] = 14;

  float expected_lambda_1 = -2.1245;
  float expected_lambda_2 = 30.1245;
  float expected_lambda_3 = 0;

  std::vector<float> result = a.eigenvalues();

  REQUIRE(result.size() == 3);
  REQUIRE(expected_lambda_1 == result[0]);
  REQUIRE(expected_lambda_2 == result[1]);
  REQUIRE(expected_lambda_3 == result[2]);

}
