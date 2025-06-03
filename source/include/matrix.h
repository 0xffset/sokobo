#pragma once
#include <vector>
#include <string>
#include "complex_number.h"

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    int rows, cols;
    
public:
    Matrix()
        : rows(0)
        , cols(0)
        , data {} {};

     Matrix(int r, int c)
      : rows(r)
      , cols(c)
      , data(r, std::vector<T>(c))
  {
  }

  Matrix(const std::vector<std::vector<T>>& mat);

    
    T& operator()(int i, int j);
    const T& operator()(int i, int j) const;
    
    // Basic operations
    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T> operator-(const Matrix<T>& other) const;
    Matrix<T> operator*(const Matrix<T>& other) const;
    Matrix<T> operator*(const T& scalar) const;
    
    // Advanced operations
    T determinant() const;
    Matrix<T> inverse() const;
    Matrix<T> transpose() const;
    std::vector<T> eigenvalues() const;
    Matrix<T> eigenvectors() const;
    
    // Decompositions
    std::pair<Matrix<T>, Matrix<T>> LUDecomposition() const;
    std::pair<Matrix<T>, Matrix<T>> QRDecomposition() const;
    
    // Matrix norms
    double frobeniusNorm() const;
    double spectralNorm() const;
    
    // Utility
    Matrix<T> getMinor(int row, int col) const;
    std::string toString() const;
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    
    // Static methods
    static Matrix<T> identity(int n);
    static Matrix<T> zeros(int r, int c);
    static Matrix<T> ones(int r, int c);
};

