#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"
#include "../../sokobo/source/include/vector.h"
#include "../../external/catch.hpp"
#include "utils.h"
using namespace std;

using namespace Catch::Matchers;
using namespace Catch::Detail;

TEST_CASE("Build tridiagonal matrix 1 x 1", "[tridiagonal]") {
    Matrix<double> dummy;
    std::vector<double> alpha = {5.0};
    std::vector<double> beta = {};
    int m = 1;

    Matrix<double> Tm = dummy.buildTridiagonal(alpha, beta, m);
    std::cout << "--- TEST: Build tridiagonal matrix 1 x 1  ---\n";
    printMatrix("M (1x1)", Tm);

    CHECK(Tm(0, 0) == Catch::Detail::Approx(5.0));
}

TEST_CASE("Build tridiagonal matrix 3 x 3 standard", "[tridiagonal]") {
    Matrix<double> dummy;
    std::vector<double> alpha = {1.0, 2.0, 3.0};
    std::vector<double> beta = {4.0, 5.0};
    int m = 3;

    Matrix<double> Tm = dummy.buildTridiagonal(alpha, beta, m);

    std::cout << "--- TEST: Build tridiagonal matrix 3 x 3 standard  ---\n";
    printMatrix("M (2x2)", Tm);

    // Principal diagonal
    CHECK(Tm(0, 0) == Catch::Detail::Approx(1.0));
    CHECK(Tm(1, 1) == Catch::Detail::Approx(2.0));
    CHECK(Tm(2, 2) == Catch::Detail::Approx(3.0));

    // Secondary diagonals
    CHECK(Tm(0, 1) == Catch::Detail::Approx(4.0));
    CHECK(Tm(1, 0) == Catch::Detail::Approx(4.0));
    CHECK(Tm(1, 2) == Catch::Detail::Approx(5.0));
    CHECK(Tm(2, 1) == Catch::Detail::Approx(5.0));

    CHECK(Tm(0, 2) == Catch::Detail::Approx(0.0));
    CHECK(Tm(2, 0) == Catch::Detail::Approx(0.0));
}

TEST_CASE("Build tridiagonal matrix with negative and decimal values", "[tridiagonal]") {
    Matrix<double> dummy;
    std::vector<double> alpha = {-2.5, 0.0};
    std::vector<double> beta = {1.75};
    int m = 2;

    Matrix<double> Tm = dummy.buildTridiagonal(alpha, beta, m);
    std::cout << "--- TEST: Build tridiagonal matrix with negative and decimal values  ---\n";
    printMatrix("M (2x2)", Tm);

    CHECK(Tm(0, 0) == Catch::Detail::Approx(-2.5));
    CHECK(Tm(1, 1) == Catch::Detail::Approx(0.0));
    CHECK(Tm(0, 1) == Catch::Detail::Approx(1.75));
    CHECK(Tm(1, 0) == Catch::Detail::Approx(1.75));
}


TEST_CASE("Build tridiagonal matrix with m smaller than vectors size", "[tridiagonal]") {
    Matrix<double> dummy;
    std::vector<double> alpha = {10.0, 20.0, 30.0, 40.0};
    std::vector<double> beta = {1.0, 2.0, 3.0};
    int m = 2;

    Matrix<double> Tm = dummy.buildTridiagonal(alpha, beta, m);
    std::cout << "--- TEST: Build tridiagonal matrix with m smaller than vectors size  ---\n";
    printMatrix("M (2x2)", Tm);

    CHECK(Tm(0, 0) == Catch::Detail::Approx(10.0));
    CHECK(Tm(0, 1) == Catch::Detail::Approx(1.0));
    CHECK(Tm(1, 0) == Catch::Detail::Approx(1.0));
    CHECK(Tm(1, 1) == Catch::Detail::Approx(20.0));
}
