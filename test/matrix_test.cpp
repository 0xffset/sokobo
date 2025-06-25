#include <iostream>
#include "include/matrix.h"

// Función auxiliar para comparar matrices
template<typename T>
bool matrices_iguales(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        return false;

    for (int i = 0; i < a.getRows(); ++i)
        for (int j = 0; j < a.getCols(); ++j)
            if (a[i][j] != b[i][j])
                return false;

    return true;
}

bool test_suma_matriz() {
    Matrix<int> a(2, 2), b(2, 2), esperado(2, 2);

    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;

    b[0][0] = 5; b[0][1] = 6;
    b[1][0] = 7; b[1][1] = 8;

    esperado[0][0] = 6; esperado[0][1] = 8;
    esperado[1][0] = 10; esperado[1][1] = 12;

    Matrix<int> resultado = a + b;

    return matrices_iguales(resultado, esperado);
}

bool test_multiplicacion_matriz() {
    Matrix<int> a(2, 2), b(2, 2), esperado(2, 2);

    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;

    b[0][0] = 2; b[0][1] = 0;
    b[1][0] = 1; b[1][1] = 2;

    esperado[0][0] = 4; esperado[0][1] = 4;
    esperado[1][0] = 10; esperado[1][1] = 8;

    Matrix<int> resultado = a * b;

    return matrices_iguales(resultado, esperado);
}

int main() {
    int total = 0;
    int exitosos = 0;

    total++;
    if (test_suma_matriz()) {
        std::cout << "[success] test_suma_matriz\n";
        exitosos++;
    } else {
        std::cout << "[failure] test_suma_matriz \n";
    }

    total++;
    if (test_multiplicacion_matriz()) {
        std::cout << "[success] test_multiplicacion_matriz\n";
        exitosos++;
    } else {
        std::cout << "[failure] test_multiplicacion_matriz\n";
    }

    std::cout << "\nResumen: " << exitosos << "/" << total << " pruebas pasaron.\n";

    return exitosos == total ? 0 : 1;
}
