#define CATCH_CONFIG_MAIN
#include "matrix.h"

#include "catch.hpp"

TEST_CASE("Suma de matrices", "[matrix]")
{
  Matrix<int> a(2, 2), b(2, 2), esperado(2, 2);

  a[0][0] = 1;
  a[0][1] = 2;
  a[1][0] = 3;
  a[1][1] = 4;

  b[0][0] = 5;
  b[0][1] = 6;
  b[1][0] = 7;
  b[1][1] = 8;

  esperado[0][0] = 6;
  esperado[0][1] = 8;
  esperado[1][0] = 10;
  esperado[1][1] = 12;

  Matrix<int> resultado = a + b;

  REQUIRE(resultado.getRows() == esperado.getRows());
  REQUIRE(resultado.getCols() == esperado.getCols());

  for (int i = 0; i < resultado.getRows(); ++i) {
    for (int j = 0; j < resultado.getCols(); ++j) {
      REQUIRE(resultado[i][j] == esperado[i][j]);
    }
  }
}

TEST_CASE("Multiplicación de matrices", "[matrix]")
{
  Matrix<int> a(2, 2), b(2, 2), esperado(2, 2);

  a[0][0] = 1;
  a[0][1] = 2;
  a[1][0] = 3;
  a[1][1] = 4;

  b[0][0] = 2;
  b[0][1] = 0;
  b[1][0] = 1;
  b[1][1] = 2;

  esperado[0][0] = 4;
  esperado[0][1] = 4;
  esperado[1][0] = 10;
  esperado[1][1] = 8;

  Matrix<int> resultado = a * b;

  REQUIRE(resultado.getRows() == esperado.getRows());
  REQUIRE(resultado.getCols() == esperado.getCols());

  for (int i = 0; i < resultado.getRows(); ++i) {
    for (int j = 0; j < resultado.getCols(); ++j) {
      REQUIRE(resultado[i][j] == esperado[i][j]);
    }
  }
}
