#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../external/catch.hpp".\te
#include "../../sokobo/source/include/matrix.h"
#include "utils.h"

TEST_CASE("Matrix Transpose", "[matrix]")
{
  SECTION("2x2 Matrix Transpose")
  {
    Matrix<int> a(2, 2);
    a[0][0] = 1;
    a[0][1] = 2;
    a[1][0] = 3;
    a[1][1] = 4;

    Matrix<int> result = a.transpose();
    CHECK(result[0][0] == 1);
    CHECK(result[0][1] == 3);
    CHECK(result[1][0] == 2);
    CHECK(result[1][1] == 4);
  }

  SECTION("3x3 Matrix Transpose")
  {
    Matrix<int> a(3, 3);
    int val = 1;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        a[i][j] = val++;
      }
    }

    Matrix<int> result = a.transpose();
    CHECK(result[0][0] == 1);
    CHECK(result[0][1] == 4);
    CHECK(result[0][2] == 7);
    CHECK(result[1][0] == 2);
    CHECK(result[1][1] == 5);
    CHECK(result[1][2] == 8);
    CHECK(result[2][0] == 3);
    CHECK(result[2][1] == 6);
    CHECK(result[2][2] == 9);
  }

  SECTION("4x4 Matrix Transpose")
  {
    Matrix<int> a(4, 4);
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        a[i][j] = (i * 4) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("5x5 Matrix Transpose")
  {
    Matrix<int> a(5, 5);
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        a[i][j] = (i * 5) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("6x6 Matrix Transpose")
  {
    Matrix<int> a(6, 6);
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        a[i][j] = (i * 6) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("7x7 Matrix Transpose")
  {
    Matrix<int> a(7, 7);
    for (int i = 0; i < 7; ++i) {
      for (int j = 0; j < 7; ++j) {
        a[i][j] = (i * 7) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 7; ++i) {
      for (int j = 0; j < 7; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("8x8 Matrix Transpose")
  {
    Matrix<int> a(8, 8);
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        a[i][j] = (i * 8) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("9x9 Matrix Transpose")
  {
    Matrix<int> a(9, 9);
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        a[i][j] = (i * 9) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("10x10 Matrix Transpose")
  {
    Matrix<int> a(10, 10);
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        a[i][j] = (i * 10) + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("100x100 Matrix Transpose")
  {
    Matrix<int> a(100, 100);
    for (int i = 0; i < 100; ++i) {
      for (int j = 0; j < 100; ++j) {
        a[i][j] = i + j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 100; ++i) {
      for (int j = 0; j < 100; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }

  SECTION("1000x1000 Matrix Transpose")
  {
    Matrix<int> a(1000, 1000);
    for (int i = 0; i < 1000; ++i) {
      for (int j = 0; j < 1000; ++j) {
        a[i][j] = i ^ j;
      }
    }
    Matrix<int> result = a.transpose();
    for (int i = 0; i < 1000; ++i) {
      for (int j = 0; j < 1000; ++j) {
        CHECK(result[i][j] == a[j][i]);
      }
    }
  }
}
