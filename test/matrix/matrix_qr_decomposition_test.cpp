#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"
#include "../../external/catch.hpp"
#include "utils.h"

using namespace Catch::Matchers;
using namespace Catch::Detail;


TEST_CASE("QR Decomposition", "[qr]") {
    Matrix<double> A(2,2);
    A(0,0) = 2;
    A(0,1) = 1;
    A(1,0) = 1;
    A(1,1) = 2;

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
}
