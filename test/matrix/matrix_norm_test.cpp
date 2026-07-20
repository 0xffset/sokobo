#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"
#include "../../external/catch.hpp"
using Catch::Matchers::Message;
using Catch::Matchers::StartsWith;

TEST_CASE("Norm of 1 x 1 non-zero matrix - Norm", "[norm]") {

    Matrix<int> m = Matrix<int>(1,1);
    std::vector<int> v = {5};
    double result = m.norm(v);

    int expected = 5;
    CHECK(result == expected);
}
