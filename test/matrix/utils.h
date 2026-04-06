#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "../../sokobo/source/include/matrix.h"

/**
 * Imprime un vector de tipo T. 
 * T debe ser un tipo básico (int, float, double) que std::cout ya conozca.
 */
template<typename T>
void printVector(const std::string& name, const std::vector<T>& v) {
    std::cout << name << ": [";
    for (size_t i = 0; i < v.size(); ++i) {
        // Usamos double para asegurar que el formateo de iomanip funcione bien
        std::cout << std::setw(10) << std::fixed << std::setprecision(6) << static_cast<double>(v[i]);
        if (i < v.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

/**
 * Imprime la matriz accediendo elemento a elemento.
 * No requiere 'operator<<' para la clase Matrix, solo para el tipo T interno.
 */
template<typename T>
void printMatrix(const std::string& name, const Matrix<T>& m) {
    std::cout << name << " (" << m.getRows() << "x" << m.getCols() << "):\n";
    for (int i = 0; i < m.getRows(); ++i) {
        for (int j = 0; j < m.getCols(); ++j) {
          
            std::cout << std::setw(12) << std::fixed << std::setprecision(6) << static_cast<double>(m(i, j));
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

#endif // UTILS_H