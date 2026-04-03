#define CATCH_CONFIG_MAIN
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "../../sokobo/source/include/matrix.h"
#include "../../external/catch.hpp"
#include "utils.h" // Asegúrate de que utils.h tenga las definiciones de print

using namespace Catch::Matchers;

// --- TEST 1: QR ---
TEST_CASE("QR Decomposition - Symmetric", "[qr]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 2; A(0, 1) = 1;
    A(1, 0) = 1; A(1, 1) = 2;
    
    std::cout << "--- TEST: QR Decomposition ---\n";
    printMatrix("Matriz Original A", A);

    auto [Q, R] = A.QRDecomposition();
    
    printMatrix("Resultado Q", Q);
    printMatrix("Resultado R", R);

    Matrix<double> QR = Q * R;
    printMatrix("Verificación Q * R", QR);

    SECTION("Q * R should equal A") {
        for (int i = 0; i < A.getRows(); ++i) {
            for (int j = 0; j < A.getCols(); ++j) {
                REQUIRE_THAT(QR(i, j), WithinAbs(A(i, j), 1e-10));
            }
        }
    }

    SECTION("Q should be orthogonal") {
        Matrix<double> QtQ = Q.transpose() * Q;
        printMatrix("Ortogonalidad Q^T * Q", QtQ);
        
        for (int i = 0; i < QtQ.getRows(); ++i) {
            for (int j = 0; j < QtQ.getCols(); ++j) {
                double expected = (i == j) ? 1.0 : 0.0;
                REQUIRE_THAT(QtQ(i, j), WithinAbs(expected, 1e-10));
            }
        }
    }
}

// --- TEST 2: LANCZOS ALGORITHM ---
TEST_CASE("Lanczos Algorithm - Symmetric Matrix", "[lanczos]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 2; A(0, 1) = 1;
    A(1, 0) = 1; A(1, 1) = 2;
    
    std::cout << "\n--- TEST: Lanczos Algorithm ---\n";
    printMatrix("Matriz A", A);

    std::vector<double> alpha, beta;
    Matrix<double> Q_lanc;
    
    // Intentamos 2 pasos
    A.lanczos(2, alpha, beta, Q_lanc);

    printVector("Alpha (Diagonal de T)", alpha);
    printVector("Beta (Subdiagonal de T)", beta);
    printMatrix("Matriz Q (Vectores de Lanczos)", Q_lanc);

    // T debe construirse con el tamaño real de alpha (puede ser 1 o 2)
    int actual_m = alpha.size();
    Matrix<double> T = A.buildTridiagonal(alpha, beta, actual_m);
    printMatrix("Matriz Tridiagonal T", T);
    
    // Verificación fundamental: Q^T * A * Q = T
    Matrix<double> QtAQ = Q_lanc.transpose() * A * Q_lanc;
    printMatrix("Verificación Q^T * A * Q", QtAQ);

    for (int i = 0; i < T.getRows(); ++i) {
        for (int j = 0; j < T.getCols(); ++j) {
            REQUIRE_THAT(QtAQ(i, j), WithinAbs(T(i, j), 1e-8));
        }
    }
}

// --- TEST 3: ITERACIONES QR SOBRE TRIDIAGONAL ---
TEST_CASE("QR Iterations and Eigenvalues", "[qr_iterations]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 2; A(0, 1) = 1;
    A(1, 0) = 1; A(1, 1) = 2;
    
    std::cout << "\n--- TEST: QR Iterations ---\n";

    std::vector<double> alpha, beta;
    Matrix<double> Q_lanc;
    A.lanczos(2, alpha, beta, Q_lanc);

    Matrix<double> T = A.buildTridiagonal(alpha, beta, alpha.size());
    Matrix<double> T_copy = T;
    
    for (int iter = 0; iter < 100; ++iter) {
        auto [Qk, Rk] = T_copy.QRDecomposition();
        T_copy = Rk * Qk;
    }
    
    printMatrix("T Final (Diagonalizada)", T_copy);

    for(int i = 0; i < T_copy.getRows(); ++i) {
        double lambda = T_copy(i, i);
        std::cout << "Autovalor encontrado lambda_" << i << ": " << lambda << "\n";
        bool is_valid = (std::abs(lambda - 3.0) < 1e-6 || std::abs(lambda - 1.0) < 1e-6);
        REQUIRE(is_valid);
    }
}

// --- TEST 4: RECONSTRUCCIÓN COMPLETA DE AUTOVECTORES ---
TEST_CASE("Full Eigenvector Method - Symmetric", "[eigenvectors_full]") {
    Matrix<double> A(2, 2);
    A(0, 0) = 2; A(0, 1) = 1;
    A(1, 0) = 1; A(1, 1) = 2;
    
    std::cout << "\n--- TEST: Full Eigenvector Reconstruction ---\n";

    Matrix<double> eigvecs = A.eigenvectors();
    printMatrix("Matriz de Autovectores (Columnas)", eigvecs);
    
    for (int k = 0; k < eigvecs.getCols(); ++k) {
        std::vector<double> v(A.getRows());
        for(int i = 0; i < A.getRows(); ++i) {
            v[i] = eigvecs(i, k);
        }

        printVector("Autovector v", v);

        auto Av = A.matVec(v);
        double lambda = A.dot(v, Av) / A.dot(v, v);
        
        std::cout << "Autovalor calculado (Rayleigh): " << lambda << "\n";

        bool isCorrectLambda = (std::abs(lambda - 3.0) < 1e-6 || std::abs(lambda - 1.0) < 1e-6);
        REQUIRE(isCorrectLambda);

        double residual_sq = 0;
        for (int i = 0; i < A.getRows(); ++i) {
            double diff = Av[i] - lambda * v[i];
            residual_sq += diff * diff;
        }
        double residual = std::sqrt(residual_sq);
        std::cout << "Residuo ||Av - lv||: " << residual << "\n";
        
        REQUIRE(residual < 1e-8);
    }
    std::cout << "-------------------------------------------\n";
}