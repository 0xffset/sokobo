#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"

#include "../../external/catch.hpp"
#include "utils.h"

TEST_CASE("Matrix Inverse", "[matrix]")
{
    auto is_identity = [](const Matrix<float>& m, float epsilon = 0.001f) {
        if (m.getRows() != m.getCols()) return false;
        for (int i = 0; i < m.getRows(); ++i) {
            for (int j = 0; j < m.getCols(); ++j) {
                float expected = (i == j) ? 1.0f : 0.0f;
                if (std::abs(m[i][j] - expected) > epsilon) return false;
            }
        }
        return true;
    };

    SECTION("2x2 Matrix Inverse") {
        Matrix<float> a(2, 2);
        a[0][0] = 4; a[0][1] = 7;
        a[1][0] = 2; a[1][1] = 6;
        // det = 24 - 14 = 10
        Matrix<float> res = a.inverse();
        CHECK(res[0][0] == Approx(0.6f));
        CHECK(res[0][1] == Approx(-0.7f));
        CHECK(res[1][0] == Approx(-0.2f));
        CHECK(res[1][1] == Approx(0.4f));
    }

    SECTION("3x3 Matrix Inverse") {
        Matrix<float> a(3, 3);
        a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
        a[1][0] = 0; a[1][1] = 1; a[1][2] = 4;
        a[2][0] = 5; a[2][1] = 6; a[2][2] = 0;
        
        Matrix<float> inv = a.inverse();
        CHECK(is_identity(a * inv));
    }

    SECTION("4x4 Matrix Inverse") {
        Matrix<float> a(4, 4);
        for(int i=0; i<4; ++i) {
            for(int j=0; j<4; ++j) a[i][j] = (i == j) ? 10.0f : 1.0f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("5x5 Matrix Inverse") {
        Matrix<float> a(5, 5);
        for(int i=0; i<5; ++i) {
            for(int j=0; j<5; ++j) a[i][j] = (i == j) ? 5.0f : 0.5f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("6x6 Matrix Inverse") {
        Matrix<float> a(6, 6);
        for(int i=0; i<6; ++i) {
            for(int j=0; j<6; ++j) a[i][j] = (i == j) ? 6.0f : 1.0f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("7x7 Matrix Inverse") {
        Matrix<float> a(7, 7);
        for(int i=0; i<7; ++i) {
            for(int j=0; j<7; ++j) a[i][j] = (i == j) ? 7.0f : 1.0f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("8x8 Matrix Inverse") {
        Matrix<float> a(8, 8);
        for(int i=0; i<8; ++i) {
            for(int j=0; j<8; ++j) a[i][j] = (i == j) ? 8.0f : 0.2f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("9x9 Matrix Inverse") {
        Matrix<float> a(9, 9);
        for(int i=0; i<9; ++i) {
            for(int j=0; j<9; ++j) a[i][j] = (i == j) ? 15.0f : 1.0f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("10x10 Matrix Inverse") {
        Matrix<float> a(10, 10);
        for(int i=0; i<10; ++i) {
            for(int j=0; j<10; ++j) a[i][j] = (i == j) ? 20.0f : 1.0f;
        }
        CHECK(is_identity(a * a.inverse()));
    }

    SECTION("100x100 Matrix Inverse") {
        Matrix<float> a(100, 100);
        for(int i=0; i<100; ++i) {
            for(int j=0; j<100; ++j) a[i][j] = (i == j) ? 100.0f : 0.1f;
        }
        CHECK(is_identity(a * a.inverse(), 0.01f)); 
    }

    SECTION("1000x1000 Matrix Inverse") {
        Matrix<float> a(1000, 1000);
        for(int i=0; i<1000; ++i) {
            for(int j=0; j<1000; ++j) a[i][j] = (i == j) ? 1000.0f : 0.01f;
        }
        CHECK(is_identity(a * a.inverse(), 0.05f));
    }
}
