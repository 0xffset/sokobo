#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"
#include "../../sokobo/source/include/vector.h"
#include "../../external/catch.hpp"

using namespace std;

using namespace Catch::Matchers;
using namespace Catch::Detail;


TEST_CASE("Norm of 1 x 1 zero matrix", "[norm]") {
    Matrix<double> m(1, 1);
    m(0, 0) = 0.0;
    std::vector<double> v = {5.0};

    double result = m.norm(v);
    double expected = 0.0;

    CHECK(result == Catch::Detail::Approx(expected));
}

TEST_CASE("Norm of Identity matrix with standard vector", "[norm]") {
    Matrix<double> m(3, 3);
    m(0, 0) = 1.0; m(1, 1) = 1.0; m(2, 2) = 1.0;
    std::vector<double> v = {3.0, 4.0, 0.0};

    double result = m.norm(v);
    double expected = 5.0;

    CHECK(result == Catch::Detail::Approx(expected));
}

TEST_CASE("Norm with zero vector", "[norm]") {
    Matrix<double> m(2, 2);
    m(0, 0) = 2.0; m(0, 1) = 3.0;
    m(1, 0) = 1.0; m(1, 1) = 4.0;
    std::vector<double> v = {0.0, 0.0};

    double result = m.norm(v);
    double expected = 0.0;

    CHECK(result == Catch::Detail::Approx(expected));
}

TEST_CASE("Norm of rectangular matrix", "[norm]") {
    Matrix<double> m(2, 3);
    m(0, 0) = 1.0; m(0, 1) = 2.0; m(0, 2) = 3.0;
    m(1, 0) = 4.0; m(1, 1) = 5.0; m(1, 2) = 6.0;
    std::vector<double> v = {1.0, 0.0, 0.0};

    double result = m.norm(v);
    double expected = 4.123105625617661;

    CHECK(result == Catch::Detail::Approx(expected));
}

TEST_CASE("Norm matrix size mismatch throws exception", "[norm]") {
    Matrix<double> m(2, 2);
    std::vector<double> v = {1.0, 2.0, 3.0};

    CHECK_THROWS_AS(m.norm(v), std::invalid_argument);
}
