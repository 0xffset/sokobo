#define CATCH_CONFIG_MAIN
#include <cmath>

#include "../../sokobo/source/include/matrix.h"

#include "../../external/catch.hpp"
#include "utils.h"
using Catch::Matchers::Message;
using Catch::Matchers::StartsWith;

using namespace Catch::Matchers;

TEST_CASE("Rank of 1 x 1 non-zero matrix - Rank", "[rank]")
{
  Matrix<int> m(1, 1);
  m(0, 0) = 5;

  std::cout << "--- TEST: 1x1 non-zero matrix ---\n";
  printMatrix("M (1x1)", m);
  int result = m.rank();
  int expected = 1;
  CHECK(result == expected);
}

TEST_CASE("Rank of 1 x 1 zero matrix - Rank", "[rank]")
{
  Matrix<int> m(1, 1);
  m(0, 0) = 0;

  std::cout << "--- TEST: 1x1 zero matrix ---\n";
  printMatrix("M (1x1)", m);
  int result = m.rank();
  int expected = 0;
  CHECK(result == expected);
}

TEST_CASE("Rank of 2 x 2 identity matrix - Rank", "[rank]")
{
  Matrix<int> m(2, 2);

  m(0, 0) = 1;
  m(0, 1) = 0;
  m(1, 0) = 0;
  m(1, 1) = 1;

  std::cout << "--- TEST: 2x2 identity matrix ---\n";

  printMatrix("M (2x2)", m);

  int result = m.rank();
  int expected = 2;

  CHECK(result == expected);
}

TEST_CASE("Rank of 2 x 2 singular matrix - Rank", "[rank]")
{
  Matrix<int> m(2, 2);

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(1, 0) = 2;
  m(1, 1) = 4;

  std::cout << "--- TEST: 2x2 singular matrix ---\n";

  printMatrix("M (2x2)", m);

  int result = m.rank();
  int expected = 1;

  CHECK(result == expected);
}

TEST_CASE("Rank of 3 x 3 full rank matrix - Rank", "[rank]")
{
  Matrix<int> m(3, 3);

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(0, 2) = 3;
  m(1, 0) = 0;
  m(1, 1) = 1;
  m(1, 2) = 4;
  m(2, 0) = 5;
  m(2, 1) = 6;
  m(2, 2) = 0;

  std::cout << "--- TEST: 3x3 full rank matrix ---\n";

  printMatrix("M (3x3)", m);

  int result = m.rank();
  int expected = 3;

  CHECK(result == expected);
}

TEST_CASE("Rank of 3 x 3 matrix with dependent rows - Rank", "[rank]")
{
  Matrix<int> m(3, 3);

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(0, 2) = 3;
  m(1, 0) = 2;
  m(1, 1) = 4;
  m(1, 2) = 6;
  m(2, 0) = 3;
  m(2, 1) = 6;
  m(2, 2) = 9;

  std::cout << "--- TEST: 3x3 matrix with dependent rows ---\n";

  printMatrix("M (3x3)", m);

  int result = m.rank();
  int expected = 1;

  CHECK(result == expected);
}

TEST_CASE("Rank of zero matrix - Rank", "[rank]")
{
  Matrix<int> m(3, 3);

  m(0, 0) = 0;
  m(0, 1) = 0;
  m(0, 2) = 0;
  m(1, 0) = 0;
  m(1, 1) = 0;
  m(1, 2) = 0;
  m(2, 0) = 0;
  m(2, 1) = 0;
  m(2, 2) = 0;

  std::cout << "--- TEST: 3x3 zero matrix ---\n";

  printMatrix("M (3x3)", m);

  int result = m.rank();
  int expected = 0;

  CHECK(result == expected);
}

TEST_CASE("Rank of rectangular matrix with full row rank - Rank", "[rank]")
{
  Matrix<int> m(2, 3);

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(0, 2) = 3;
  m(1, 0) = 4;
  m(1, 1) = 5;
  m(1, 2) = 3;

  std::cout << "--- TEST: 2x3 rectangular matrix with full row rank ---\n";

  printMatrix("M (2x3)", m);

  int result = m.rank();
  int expected = 2;

  CHECK(result == expected);
}

TEST_CASE("Rank of rectangular matrix with dependent rows - Rank", "[rank]")
{
  Matrix<int> m(2, 3);

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(0, 2) = 3;
  m(1, 0) = 2;
  m(1, 1) = 4;
  m(1, 2) = 6;

  std::cout << "--- TEST: 3x3 rectangular matrix with dependent rows ---\n";

  printMatrix("M (2x3)", m);

  int result = m.rank();
  int expected = 1;

  CHECK(result == expected);
}
TEST_CASE("Rank of matrix with linearly independent columns - Rank", "[rank]")
{
    Matrix<int> m(3, 2);

    m(0, 0) = 1;
    m(0, 1) = 0;

    m(1, 0) = 0;
    m(1, 1) = 1;

    m(2, 0) = 1;
    m(2, 1) = 1;

    std::cout << "--- TEST: Matrix with linearly independent columns ---\n";

    printMatrix("M (3x2)", m);

    int result = m.rank();
    int expected = 2;

    CHECK(result == expected);
}

