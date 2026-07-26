#define CATCH_CONFIG_MAIN
#include <random>

#include "../../sokobo/source/include/matrix.h"

#include "../../external/catch.hpp"
#include "utils.h"

using namespace Catch::Matchers;
using namespace Catch::Detail;

TEST_CASE("QR Decomposition 2 x 2 matrix", "[qr]")
{
  Matrix<double> A(2, 2);
  A(0, 0) = 2;
  A(0, 1) = 1;
  A(1, 0) = 1;
  A(1, 1) = 2;

  auto [Q, R] = A.QRDecomposition();
  printMatrix("Q", Q);
  printMatrix("R", R);

  // Q * R = A
  Matrix<double> QR = Q * R;
  printMatrix("Q * R (should equal A)", QR);

  SECTION("Q * R should equal A")
  {
    REQUIRE_THAT(QR(0, 0), WithinAbs(2.0, 1e-10));
    REQUIRE_THAT(QR(0, 1), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(QR(1, 0), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(QR(1, 1), WithinAbs(2.0, 1e-10));
  }

  SECTION("Q should be orthogonal")
  {
    Matrix<double> QtQ = Q.transpose() * Q;
    REQUIRE_THAT(QtQ(0, 0), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(QtQ(1, 1), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(std::abs(QtQ(0, 1)), WithinAbs(0.0, 1e-10));
  }

  SECTION("Triangularity of R")
  {
    // All entries below the main diagonal of R are zero.
    for (int i = 0; i < R.getRows(); ++i) {
      for (int j = 0; j < R.getCols(); ++j) {
        if (i > j) {
          REQUIRE_THAT(R(i, j), WithinAbs(0.0, 1e-10));
        }
      }
    }
  }
}

TEST_CASE("QR Decomposition 100 x 100 matrix", "[qr][large]")
{
  const size_t size = 100;
  Matrix<double> A(size, size);

  std::mt19937 gen(42);
  std::uniform_real_distribution<double> dis(-10.0, 10.0);

  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      A(i, j) = dis(gen);
    }
  }

  // Perform decomposition
  auto [Q, R] = A.QRDecomposition();

  // Q * R = A
  Matrix<double> QR = Q * R;

  SECTION("Q * R should equal A")
  {
    Matrix<double> QR = Q * R;
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        REQUIRE_THAT(QR(i, j), WithinAbs(A(i, j), 1e-9));
      }
    }
  }

  SECTION("Q should be orthogonal")
  {
    Matrix<double> QtQ = Q.transpose() * Q;
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        double expected = (i == j) ? 1.0 : 0.0;
        REQUIRE_THAT(QtQ(i, j), WithinAbs(expected, 1e-9));
      }
    }
  }

  SECTION("Triangularity of R")
  {
    // All entries below the main diagonal must be zero
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        if (i > j) {
          REQUIRE_THAT(R(i, j), WithinAbs(0.0, 1e-9));
        }
      }
    }
  }
}
