#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"
#include "../../external/catch.hpp"
#include "../utils.h"

using namespace Catch::Matchers;
using namespace Catch::Detail;


void init2x2(Matrix<double>& M, double m00, double m01, double m10, double m11) {
    M(0,0) = m00, M(0,1) = m01;
    M(1,0) =  m10, M(1,1) = m11;
}

TEST_CASE("Spectral Radius - Symmetric Positive Matrix", "[spectral_radius]")
{
    // Eigenvalues are 3 and 1 -> max(|3|, |1|) = 3
    Matrix<double> A(2,2);
    init2x2(A, 2.0,1.0,1.0,2.0);
    REQUIRE(A.getCols() == A.getRows());
    REQUIRE(A.spectralRadius() ==  Catch::Detail::Approx(3.0));
}

TEST_CASE("Spectral Radius - Negative Diagonal Elements", "[spectral_radius]")
{
    // Eigenvalues are -3 and 2 -> max(|-3|, |2|) = 3
    Matrix<double> A(2, 2);
    init2x2(A, -3.0,  0.0,
                0.0,  2.0);

    REQUIRE(A.spectralRadius() == Catch::Detail::Approx(3.0));
}

// FIX: Not implemented yet.
// TEST_CASE("Spectral Radius - Complex Eigenvalues (Rotation)", "[spectral_radius]")
// {
//     // Eigenvalues are +/- i -> Modulus is sqrt(0^2 + 1^2) = 1
//     Matrix<double> A(2, 2);
//     init2x2(A, 0.0, -1.0,
//                1.0,  0.0);

//     REQUIRE(A.spectralRadius() == Catch::Detail::Approx(1.0));
// }

TEST_CASE("Spectral Radius - Nilpotent Matrix", "[spectral_radius]")
{
    // Eigenvalues are 0 and 0 -> max(|0|, |0|) = 0
    Matrix<double> A(2, 2);
    init2x2(A, 0.0, 5.0,
               0.0, 0.0);

    REQUIRE(A.spectralRadius() == Catch::Detail::Approx(0.0));
}

TEST_CASE("Spectral Radius - Identity Matrix", "[spectral_radius]")
{
    // Eigenvalues are 1 and 1 -> max(|1|, |1|) = 1
    Matrix<double> A(2, 2);
    init2x2(A, 1.0, 0.0,
               0.0, 1.0);

    REQUIRE(A.spectralRadius() == Catch::Detail::Approx(1.0));
}
