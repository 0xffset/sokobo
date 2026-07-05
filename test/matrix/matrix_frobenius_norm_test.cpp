#define CATCH_CONFIG_MAIN
#include <vector> 

#include "../../sokobo/source/include/matrix.h"
#include "../../external/catch.hpp"

#include "utils.h"
using Catch::Matchers::Message; 
using Catch::Matchers::StartsWith; 


TEST_CASE("Norm of 1 x1 non-zero matrix - frobeniusNorm", "[norm]") {

  Matrix<double> m(1,1); 
  m(0,0) = 5.0; 
  std::cout << "TEST: 1x1 non-zero matrix \n"; 
  printMatrix("M(1x1)", m); 
  double result = m.frobeniusNorm();
  double expected = 5.0;
  CHECK(result == expected);

}

TEST_CASE("Frobenius norm of 1x1 zero matrix - frobeniusNorm", "[norm]") {

  Matrix<double> m(1,1); 
  m(0,0) = 0.0; 
  std::cout << "TEST: 1x1 zero matrix \n"; 
  printMatrix("M(1x1)", m); 
  double result = m.frobeniusNorm();
  double expected = 0.0;
  CHECK(result == expected);

}

