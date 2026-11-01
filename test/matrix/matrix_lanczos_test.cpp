#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../source/include/matrix.h"
#include "../../external/catch.hpp"
#include "./utils.h"

using namespace std;
using namespace Catch::Matchers;
using namespace Catch::Detail;

TEST_CASE("Lanczos algorithm invariants on symmetric matrix", "[lanczos]") {
    // 1. Setup a symmetric 3x3 test matrix A
    int n = 3;
    Matrix<double> A(n, n);

    A(0, 0) =  2.0; A(0, 1) = -1.0; A(0, 2) =  0.0;
    A(1, 0) = -1.0; A(1, 1) =  2.0; A(1, 2) = -1.0;
    A(2, 0) =  0.0; A(2, 1) = -1.0; A(2, 2) =  2.0;

    int m = 3;
    std::vector<double> alpha;
    std::vector<double> beta;
    Matrix<double> Q;

    // 2. Execute Lanczos algorithm
    A.lanczos(m, alpha, beta, Q);

    std::cout << "--- TEST: Lanczos algorithm invariant checks ---\n";

    // 3. Verify dimensions
    CHECK(alpha.size() == m);
    CHECK(beta.size() == m - 1);
    CHECK(Q.getRows() == n);
    CHECK(Q.getCols() == m);

    // 4. Verify Q columns are orthonormal (Unit norm & mutually orthogonal)
    for (int j = 0; j < m; ++j) {
        double colNormSq = 0.0;
        for (int i = 0; i < n; ++i) {
            colNormSq += Q(i, j) * Q(i, j);
        }
        CAPTURE(j, colNormSq);
        CHECK(colNormSq == Catch::Detail::Approx(1.0).margin(1e-4));
    }

    for (int c1 = 0; c1 < m; ++c1) {
        for (int c2 = c1 + 1; c2 < m; ++c2) {
            double dotProd = 0.0;
            for (int i = 0; i < n; ++i) {
                dotProd += Q(i, c1) * Q(i, c2);
            }
            CAPTURE(c1, c2, dotProd);
            CHECK(dotProd == Catch::Detail::Approx(0.0).margin(1e-4));
        }
    }

    // 5. Reconstruct the tridiagonal matrix Tm
    Matrix<double> Tm = A.buildTridiagonal(alpha, beta, m);
    printMatrix("Tridiagonal Matrix Tm (3x3)", Tm);

    // 6. Verify Galerkin condition: Q^T * A * Q should approximately equal Tm
    // Compute AQ column by column
    Matrix<double> AQ(n, m);
    for (int j = 0; j < m; ++j) {
        std::vector<double> q_col(n);
        for (int i = 0; i < n; ++i) {
            q_col[i] = Q(i, j);
        }
        std::vector<double> aq_col = A.matVec(q_col);
        for (int i = 0; i < n; ++i) {
            AQ(i, j) = aq_col[i];
        }
    }

    // Compute Qt * AQ -> results in an m x m matrix (Q^T * A * Q)
    Matrix<double> QtAQ(m, m);
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < m; ++c) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += Q(k, r) * AQ(k, c); // Q(k, r) is transpose element (r, k)
            }
            QtAQ(r, c) = sum;
        }
    }

    printMatrix("Computed Q^T * A * Q", QtAQ);

    // Compare QtAQ with Tm element-wise
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < m; ++c) {
            CAPTURE(r, c, QtAQ(r, c), Tm(r, c));
            CHECK(QtAQ(r, c) == Catch::Detail::Approx(Tm(r, c)).margin(1e-4));
        }
    }
}
