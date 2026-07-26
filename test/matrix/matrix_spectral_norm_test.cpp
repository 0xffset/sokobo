#define CATCH_CONFIG_MAIN

#include "../../source/include/matrix.h"

#include "../../external/catch.hpp"
#include "../../source/include/vector.h"
#include "../utils.h"

using namespace Catch::Matchers;
using namespace Catch::Detail;

#define M_PI 3.14159265358979323846

TEST_CASE("Spectral Norm 2x2 Matrix |A|_2", "[spectral]")
{
  Matrix<double> A(2, 2);
  A(0, 0) = 2;
  A(0, 1) = 1;
  A(1, 0) = 1;
  A(1, 1) = 2;
  printMatrix("Matrix A", A);
  double result = A.spectralNorm();
  double expected = 3.0;

  CHECK(expected == result);

  SECTION("Non-Negativity 2x2")
  {
    // |A|_2 >= 0 AND |A|_2 = 0 iff |A| = 0
    CHECK(A.spectralNorm() >= 0.0);

    Matrix<double> zeroMatrix(2, 2);
    zeroMatrix(0, 0) = 0.0;
    zeroMatrix(0, 1) = 0.0;
    zeroMatrix(1, 0) = 0.0;
    zeroMatrix(1, 1) = 0.0;

    CHECK(zeroMatrix.spectralNorm() == 0.0);
    CHECK(A.spectralNorm() > 0.0);
  }
  SECTION("Homogeneity 2x2")
  {
    //  (‖αA‖₂ = |{α| ⋅ ‖A‖₂) for any scalar α
    double alpha = -2.5;
    Matrix<double> sA = A * alpha;
    double left_side = sA.spectralNorm();

    double right_side = std::abs(alpha) * A.spectralNorm();

    CHECK(left_side == Catch::Detail::Approx(right_side));
  }
  SECTION("Triangle Inequality (Sub-addtivity)")
  {
    // (A + B‖₂ ≤ ‖A‖₂ + ‖B‖₂) for matrices of the same dimensions
    Matrix<double> B(2, 2);
    B(0, 0) = 1.0;
    B(0, 1) = -2.0;
    B(1, 0) = 3.0;
    B(1, 1) = 5.0;

    Matrix<double> sumAB = A + B;
    double left_side = sumAB.spectralNorm();

    double right_side = A.spectralNorm() + B.spectralNorm();
    CHECK(left_side <= right_side);
  }

  SECTION("Sub-multiplicativity 2x2")
  {
    // (‖AB‖₂ ≤ ‖A‖₂ ⋅ ‖B‖₂) for conformable matrices

    Matrix<double> B(2, 2);
    B(0, 0) = 1.0;
    B(0, 1) = -2.0;
    B(1, 0) = 3.0;
    B(1, 1) = 0.5;

    Matrix<double> prodAB = A * B;
    double left_side = prodAB.spectralNorm();
    double right_side = A.spectralNorm() * B.spectralNorm();

    CHECK(left_side <= right_side + 1e-12);
  }

  SECTION("Vector Compatibility")
  {
    //  (‖Ax‖₂ ≤ ‖A‖₂ ⋅ ‖x‖₂) for any vector x
    ::Vector<double> X(2);
    X[0] = 1.5;
    X[0] = -2.0;

    ::Vector<double> prodAX = A * X;
    double left_side = prodAX.norm();

    double right_side = A.spectralNorm() * X.norm();

    CHECK(left_side <= right_side + 1e-12);
  }

  SECTION("Uninary Invariance")
  {
    // (‖UAV‖₂ = ‖A‖₂) for any unitary (or orthogonal) matrices U and V
    double theta = 30.0 * M_PI / 180.0;
    double c = std::cos(theta);
    double s = std::sin(theta);

    Matrix<double> U(2, 2);
    U(0, 0) = c;
    U(0, 1) = -s;
    U(1, 0) = s;
    U(1, 1) = c;

    double phi = 45.0 * M_PI / 180.0;
    double c2 = std::cos(phi);
    double s2 = std::sin(phi);

    Matrix<double> V(2, 2);
    V(0, 0) = c2;
    V(0, 1) = -s2;
    V(1, 0) = s2;
    V(1, 1) = c2;

    Matrix<double> UAV = U * A * V;
    double left_side = UAV.spectralNorm();

    double right_side = A.spectralNorm();

    CHECK(left_side == Catch::Detail::Approx(right_side).margin(1e-12));
  }
  SECTION("Adjoin Equality")
  {
    //  (‖Aᴴ‖₂ = ‖{}A‖₂) and (‖Aᴴ A‖₂ = ‖A‖₂²)
    Matrix<double> AH = A.transpose();

    double norm_A = A.spectralNorm();
    double norm_AH = AH.spectralNorm();

    CHECK(norm_AH == Catch::Detail::Approx(norm_A).margin(1e-12));

    Matrix<double> AHA = AH * A;
    double left_side = AHA.spectralNorm();
    double right_side = norm_A * norm_A;

    CHECK(left_side == Catch::Detail::Approx(right_side).margin(1e-12));
  }
  SECTION("Spefctral Radius Bound")
  {
    // \(ρ(A) ≤ ‖)A|₂, where (ρ)(A) is the spectral radius (largest absolute
    // eigenvalue) of a square matrix A. If A is symmetric or Hermitian, (ρ)(A)
    // = (‖A‖₂
    double rho_a = A.spectralRadius();
    double norm_A = A.spectralNorm();

    CHECK(rho_a == Catch::Detail::Approx(norm_A).margin(1e-12));

    Matrix<double> B(2, 2);
    B(0, 0) = 1.0;
    B(0, 1) = 2.0;
    B(1, 0) = 0.0;
    B(1, 1) = 1.0;

    double rho_B = B.spectralRadius();
    double norm_B = B.spectralNorm();

    CHECK(rho_B <= norm_B);
    CHECK(rho_B < norm_B);
  }
}
