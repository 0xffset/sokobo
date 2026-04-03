#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"

#include "../../external/catch.hpp".\te
#include "utils.h"


using namespace Catch::Matchers;
using namespace Catch::Detail;

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

  float expected_lambda_1 = -2.124515;
  float expected_lambda_2 = 30.124516;
  float expected_lambda_3 = -0.074920;

  std::vector<float> result = a.eigenvalues();

  REQUIRE(result.size() == 3);
  REQUIRE(std::to_string(expected_lambda_2) == std::to_string(result[0]));
  REQUIRE(std::to_string(expected_lambda_3) == std::to_string(result[2]));
  REQUIRE(std::to_string(expected_lambda_1) == std::to_string(result[1]));
}


TEST_CASE("Matrix Operations", "[matrix]") {
    SECTION("Matrix Multiplication") {
        Matrix<double> A(2, 2);
        A(0, 0) = 1; A(0, 1) = 2;
        A(1, 0) = 3; A(1, 1) = 4;
        
        Matrix<double> B(2, 2);
        B(0, 0) = 5; B(0, 1) = 6;
        B(1, 0) = 7; B(1, 1) = 8;
        
        Matrix<double> C = A * B;
        
        printMatrix("A", A);
        printMatrix("B", B);
        printMatrix("A * B", C);
        
        // A * B should be [[19, 22], [43, 50]]
        REQUIRE_THAT(C(0, 0), WithinAbs(19.0, 1e-10));
        REQUIRE_THAT(C(0, 1), WithinAbs(22.0, 1e-10));
        REQUIRE_THAT(C(1, 0), WithinAbs(43.0, 1e-10));
        REQUIRE_THAT(C(1, 1), WithinAbs(50.0, 1e-10));
    }
    
    SECTION("Matrix Transpose") {
        Matrix<double> A(2, 2);
        A(0, 0) = 1; A(0, 1) = 2;
        A(1, 0) = 3; A(1, 1) = 4;
        
        Matrix<double> At = A.transpose();
        
        printMatrix("A", A);
        printMatrix("A^T", At);
        
        REQUIRE_THAT(At(0, 0), WithinAbs(1.0, 1e-10));
        REQUIRE_THAT(At(0, 1), WithinAbs(3.0, 1e-10));
        REQUIRE_THAT(At(1, 0), WithinAbs(2.0, 1e-10));
        REQUIRE_THAT(At(1, 1), WithinAbs(4.0, 1e-10));
    }
    
    SECTION("Matrix-Vector Multiplication") {
        Matrix<double> A(2, 2);
        A(0, 0) = 1; A(0, 1) = 2;
        A(1, 0) = 3; A(1, 1) = 4;
        
        std::vector<double> v = {1, 2};
        auto result = A.matVec(v);
        
        printMatrix("A", A);
        printVector("v", v);
        printVector("A*v", result);
        
        // A * [1, 2] = [5, 11]
        REQUIRE_THAT(result[0], WithinAbs(5.0, 1e-10));
        REQUIRE_THAT(result[1], WithinAbs(11.0, 1e-10));
    }
}

TEST_CASE("QR Decomposition - Symmetric", "[qr]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 2; A(0, 1) = 1;
    A(1, 0) = 1; A(1, 1) = 2;
    
    auto [Q, R] = A.QRDecomposition();

    printMatrix("Q", Q);
    printMatrix("R", R);
    
    // Verify Q * R = A
    Matrix<double> QR = Q * R;
    printMatrix("Q * R (should equal A)", QR);
    
    SECTION("Q * R should equal A") {
        REQUIRE_THAT(QR(0, 0), WithinAbs(2.0, 1e-10));
        REQUIRE_THAT(QR(0, 1), WithinAbs(1.0, 1e-10));
        REQUIRE_THAT(QR(1, 0), WithinAbs(1.0, 1e-10));
        REQUIRE_THAT(QR(1, 1), WithinAbs(2.0, 1e-10));
    }

    SECTION("Q should be orthogonal") {
        Matrix<double> QtQ = Q.transpose() * Q;
        REQUIRE_THAT(QtQ(0, 0), WithinAbs(1.0, 1e-10));
        REQUIRE_THAT(QtQ(1, 1), WithinAbs(1.0, 1e-10));
        REQUIRE_THAT(std::abs(QtQ(0, 1)), WithinAbs(0.0, 1e-10));
    }
}


