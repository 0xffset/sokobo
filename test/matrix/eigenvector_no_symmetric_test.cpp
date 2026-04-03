#define CATCH_CONFIG_MAIN
#include <vector>

#include "../../sokobo/source/include/matrix.h"

#include "../../external/catch.hpp"

#include "utils.h"

using namespace Catch::Matchers;
using namespace Catch::Detail;



TEST_CASE("QR Decomposition", "[qr]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    printMatrix("A", A);
    
    auto [Q, R] = A.QRDecomposition();
    
    printMatrix("Q", Q);
    printMatrix("R", R);
    
    // Verify Q * R = A
    Matrix<double> QR = Q * R;
    printMatrix("Q * R (should equal A)", QR);
    
    REQUIRE_THAT(QR(0, 0), WithinAbs(A(0, 0), 1e-10));
    REQUIRE_THAT(QR(0, 1), WithinAbs(A(0, 1), 1e-10));
    REQUIRE_THAT(QR(1, 0), WithinAbs(A(1, 0), 1e-10));
    REQUIRE_THAT(QR(1, 1), WithinAbs(A(1, 1), 1e-10));
    
    // Verify Q is orthogonal: Q^T * Q = I
    Matrix<double> QtQ = Q.transpose() * Q;
    printMatrix("Q^T * Q (should be identity)", QtQ);
    
    REQUIRE_THAT(QtQ(0, 0), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(QtQ(0, 1), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(QtQ(1, 0), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(QtQ(1, 1), WithinAbs(1.0, 1e-10));
}

TEST_CASE("Lanczos Algorithm", "[lanczos]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    std::cout << "\n=== LANCZOS ALGORITHM DEBUG ===\n";
    printMatrix("Original Matrix A", A);
    
    std::vector<double> alpha, beta;
    Matrix<double> Q;
    
    int m = 2;
    A.lanczos(m, alpha, beta, Q);
    
    printVector("alpha", alpha);
    printVector("beta", beta);
    printMatrix("Q (Lanczos vectors)", Q);
    
    // Build tridiagonal matrix
    Matrix<double> T = A.buildTridiagonal(alpha, beta, alpha.size());
    printMatrix("T (tridiagonal)", T);
    
    // Verify: Q^T * A * Q = T
    Matrix<double> Qt = Q.transpose();
    Matrix<double> QtAQ = Qt * A * Q;
    printMatrix("Q^T * A * Q (should equal T)", QtAQ);
    
    // Check if Q^T * A * Q ≈ T
    for (int i = 0; i < T.getRows(); ++i) {
        for (int j = 0; j < T.getCols(); ++j) {
            REQUIRE(QtAQ(i, j) == Catch::Detail::Approx(T(i, j)).margin(1e-8));
        }
    }
}

TEST_CASE("QR Iterations on Tridiagonal", "[qr_iterations]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    std::vector<double> alpha, beta;
    Matrix<double> Q;
    A.lanczos(2, alpha, beta, Q);
    
    Matrix<double> T = A.buildTridiagonal(alpha, beta, alpha.size());
    
    std::cout << "\n=== QR ITERATIONS ===\n";
    printMatrix("Initial T", T);
    
    Matrix<double> T_copy = T;
    Matrix<double> V = Matrix<double>::identity(alpha.size());
    
    for (int iter = 0; iter < 100; ++iter) {
        auto [Qk, Rk] = T_copy.QRDecomposition();
        T_copy = Rk * Qk;
        V = V * Qk;
        
        if (iter == 0 || iter == 10 || iter == 50 || iter == 99) {
            std::cout << "\n--- Iteration " << iter << " ---\n";
            printMatrix("T", T_copy);
            printMatrix("V", V);
        }
    }
    
    std::cout << "\n=== FINAL RESULTS ===\n";
    printMatrix("T (should be diagonal)", T_copy);
    printMatrix("V (eigenvectors of T)", V);
    
    // Extract eigenvalues from diagonal
    std::cout << "Eigenvalues from T diagonal:\n";
    std::cout << "  λ1 = " << T_copy(0, 0) << "\n";
    std::cout << "  λ2 = " << T_copy(1, 1) << "\n";
    
    // Expected eigenvalues: 5.372281... and -0.372281...
    double lambda1 = T_copy(0, 0);
    double lambda2 = T_copy(1, 1);
    
    // One should be ~5.37, the other ~-0.37
    bool hasLargeEigenvalue = std::abs(lambda1 - 5.372281) < 0.01 || std::abs(lambda2 - 5.372281) < 0.01;
    bool hasSmallEigenvalue = std::abs(lambda1 + 0.372281) < 0.01 || std::abs(lambda2 + 0.372281) < 0.01;
    
    REQUIRE(hasLargeEigenvalue);
    REQUIRE(hasSmallEigenvalue);
}

TEST_CASE("Eigenvector Reconstruction", "[eigenvectors]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    std::cout << "\n=== EIGENVECTOR RECONSTRUCTION ===\n";
    printMatrix("A", A);
    
    std::vector<double> alpha, beta;
    Matrix<double> Q;
    A.lanczos(2, alpha, beta, Q);
    
    Matrix<double> T = A.buildTridiagonal(alpha, beta, alpha.size());
    Matrix<double> T_copy = T;
    Matrix<double> V = Matrix<double>::identity(alpha.size());
    
    for (int iter = 0; iter < 100; ++iter) {
        auto [Qk, Rk] = T_copy.QRDecomposition();
        T_copy = Rk * Qk;
        V = V * Qk;
    }
    
    printMatrix("Q (Lanczos)", Q);
    printMatrix("V (eigenvectors of T)", V);
    
    // Compute eigenvectors: Q * V
    Matrix<double> eigvecs(A.getRows(), alpha.size());
    for (int k = 0; k < alpha.size(); ++k) {
        for (int i = 0; i < A.getRows(); ++i) {
            for (int j = 0; j < alpha.size(); ++j) {
                eigvecs(i, k) += Q(i, j) * V(j, k);
            }
        }
    }
    
    printMatrix("Eigenvectors (Q * V)", eigvecs);
    
    // Verify each eigenvector
    std::cout << "\n=== VERIFICATION ===\n";
    for (int k = 0; k < eigvecs.getCols(); ++k) {
        std::vector<double> v(A.getRows());
        for (int i = 0; i < A.getRows(); ++i) {
            v[i] = eigvecs(i, k);
        }
        
        auto Av = A.matVec(v);
        double lambda = A.dot(v, Av) / A.dot(v, v);
        
        std::cout << "\nEigenvector " << k << ":\n";
        printVector("  v", v);
        printVector("  A*v", Av);
        std::cout << "  λ = " << lambda << "\n";
        
        std::vector<double> lambda_v(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            lambda_v[i] = lambda * v[i];
        }
        printVector("  λ*v", lambda_v);
        
        // Compute residual ||A*v - λ*v||
        double residual = 0;
        for (int i = 0; i < A.getRows(); ++i) {
            double diff = Av[i] - lambda * v[i];
            residual += diff * diff;
        }
        residual = std::sqrt(residual);
        std::cout << "  Residual: " << residual << "\n";
        
        // Residual should be very small
        REQUIRE(residual < 1e-8);
    }
}

TEST_CASE("Full Eigenvector Method", "[eigenvectors_full]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    std::cout << "\n=== FULL eigenvectors() METHOD ===\n";
    printMatrix("A", A);
    
    Matrix<double> eigvecs = A.eigenvectors();
    printMatrix("Computed Eigenvectors", eigvecs);
    
    // Verify each eigenvector
    for (int k = 0; k < eigvecs.getCols(); ++k) {
        std::vector<double> v(A.getRows());
        for (int i = 0; i < A.getRows(); ++i) {
            v[i] = eigvecs(i, k);
        }
        
        auto Av = A.matVec(v);
        double lambda = A.dot(v, Av) / A.dot(v, v);
        
        std::cout << "\nEigenvector " << k << ":\n";
        printVector("  v", v);
        printVector("  A*v", Av);
        std::cout << "  Eigenvalue λ = " << lambda << "\n";
        
        // Compute residual
        double residual = 0;
        for (int i = 0; i < A.getRows(); ++i) {
            double diff = Av[i] - lambda * v[i];
            residual += diff * diff;
        }
        residual = std::sqrt(residual);
        std::cout << "  Residual: " << residual << "\n";
        
        // Check that we got one of the correct eigenvalues
        bool isCorrectEigenvalue = 
            std::abs(lambda - 5.372281) < 0.01 || 
            std::abs(lambda + 0.372281) < 0.01;
        
        REQUIRE(isCorrectEigenvalue);
        REQUIRE(residual < 1e-8);
    }
}


