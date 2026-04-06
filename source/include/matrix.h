#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "complex_number.h"
#include "vector.h"

/**
 * @brief A templated matrix class supporting various linear algebra operations.
 *
 * This class provides a comprehensive implementation of matrix operations
 * including basic arithmetic, advanced operations (determinant, inverse,
 * eigenvalues), matrix decompositions (LU, QR), and iterative algorithms
 * (Lanczos). Supports both real arithmetic types and complex number types.
 *
 * @tparam T The element type. Can be arithmetic types (int, float, double) or
 *           complex number types (ComplexNumber).
 */
template<typename T>
class Matrix
{
private:
  std::vector<std::vector<T>>
      data;  ///< Internal 2D storage for matrix elements
  int rows;  ///< Number of rows in the matrix
  int cols;  ///< Number of columns in the matrix

public:
  /**
   * @brief Default constructor creating an empty 0×0 matrix.
   */
  Matrix()
      : rows(0)
      , cols(0)
      , data {} {};

  /**
   * @brief Constructs a matrix with specified dimensions, initialized with
   * default values.
   *
   * @param r Number of rows
   * @param c Number of columns
   */
  Matrix(int r, int c)
      : rows(r)
      , cols(c)
      , data(r, std::vector<T>(c))
  {
  }

  /**
   * @brief Constructs a matrix from a 2D vector.
   *
   * @param mat A 2D vector containing the matrix data
   * @throws std::invalid_argument if rows have inconsistent lengths
   */
  Matrix(const std::vector<std::vector<T>>& mat);

  /**
   * @brief Accesses matrix element at specified position (non-const).
   *
   * @param row Row index (0-based)
   * @param column Column index (0-based)
   * @return Reference to the element at (row, column)
   */
  T& operator()(int row, int column);

  /**
   * @brief Accesses matrix element at specified position (const).
   *
   * @param row Row index (0-based)
   * @param column Column index (0-based)
   * @return Const reference to the element at (row, column)
   */
  const T& operator()(int row, int column) const;

  /**
   * @brief Accesses a row of the matrix (non-const).
   *
   * @param i Row index (0-based)
   * @return Reference to the row vector
   */
  std::vector<T>& operator[](int i) { return data[i]; }

  /**
   * @brief Accesses a row of the matrix (const).
   *
   * @param i Row index (0-based)
   * @return Const reference to the row vector
   */
  const std::vector<T>& operator[](int i) const { return data[i]; }

  // ==================== Basic Operations ====================

  /**
   * @brief Matrix addition.
   *
   * @param other Matrix to add
   * @return Result of element-wise addition
   * @throws std::invalid_argument if matrices have different dimensions
   */
  Matrix<T> operator+(const Matrix<T>& other) const;

  /**
   * @brief Matrix subtraction.
   *
   * @param other Matrix to subtract
   * @return Result of element-wise subtraction
   * @throws std::invalid_argument if matrices have different dimensions
   */
  Matrix<T> operator-(const Matrix<T>& other) const;

  /**
   * @brief Matrix multiplication.
   *
   * @param other Matrix to multiply with
   * @return Result of matrix multiplication
   * @throws std::invalid_argument if dimensions are incompatible (this.cols !=
   * other.rows)
   */
  Matrix<T> operator*(const Matrix<T>& other) const;

  /**
   * @brief Scalar multiplication.
   *
   * @param scalar Scalar value to multiply all elements by
   * @return Result of scalar multiplication
   */
  Matrix<T> operator*(const T& scalar) const;

  /**
   * @brief Element-wise division.
   *
   * @param other Matrix to divide by
   * @return Result of element-wise division
   * @throws std::invalid_argument if matrices have different dimensions
   * @throws std::runtime_error if division by zero occurs
   */
  Matrix<T> operator/=(const Matrix<T>& other) const;

  /**
   * @brief In-place subtraction.
   *
   * @param other Matrix to subtract
   * @return Result of element-wise subtraction
   * @throws std::invalid_argument if matrices have different dimensions
   */
  Matrix<T> operator-=(const Matrix<T>& other) const;

  /**
   * @brief Stream output operator for formatted matrix display.
   *
   * Outputs the matrix with fixed-point notation, 6 decimal places, and field
   * width of 12.
   *
   * @param os Output stream
   * @param m Matrix to output
   * @return Reference to the output stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
  {
    for (int i = 0; i < m.rows; ++i) {
      for (int j = 0; j < m.cols; ++j) {
        os << std::setw(12) << std::fixed << std::setprecision(6)
           << m.data[i][j];
      }
      if (i < m.rows - 1) {
        os << "\n";
      }
    }
    return os;
  }

  // ==================== Advanced Operations ====================
  /**
   * @brief Computes the determinant of the matrix.
   *
   * The calculation follows these rules based on the matrix dimension \f$ n
   * \times n \f$:
   *
   * - For \f$ n = 1 \f$:
   *   \f[ \det(A) = a_{1,1} \f]
   * - For \f$ n = 2 \f$:
   *   \f[ \det(A) = a_{0,0}a_{1,1} - a_{0,1}a_{1,0} \f]
   * - For \f$ n > 2 \f$, using **LU Decomposition** \f$ A = LU \f$:
   *   \f[ \det(A) = \det(L)\det(U) = \prod_{i=0}^{n-1} u_{i,i} \f]
   *   *(Note: \f$ \det(L) = 1 \f$ as it is a unit triangular matrix)*.
   *
   * @return Determinant value of type \f$ T \f$.
   * @throws std::invalid_argument If the matrix is not square (\f$ rows \neq
   * cols \f$).
   */
  T determinant() const;

  /**
   * @brief Computes the inverse of the matrix.
   *
   * @return Inverse matrix
   * @throws std::invalid_argument if matrix is not square
   * @throws std::runtime_error if matrix is singular (determinant is zero)
   */
  Matrix<T> inverse() const;

  /**
   * @brief Computes the transpose of the matrix.
   *
   * @return Transposed matrix with dimensions swapped
   */
  Matrix<T> transpose() const;

  /**
   * @brief Computes the eigenvalues of the matrix.
   *
   * @return Vector containing eigenvalues
   * @throws std::invalid_argument if matrix is not square
   */
  std::vector<T> eigenvalues() const;

  /**
   * @brief Computes the eigenvectors of the matrix.
   *
   * @return Matrix where each column is an eigenvector
   * @throws std::invalid_argument if matrix is not square
   */
  Matrix<T> eigenvectors() const;

  /**
   * @brief Computes the adjoint (adjugate) matrix.
   *
   * The adjoint is the transpose of the cofactor matrix.
   *
   * @return Adjoint matrix
   * @throws std::invalid_argument if matrix is not square
   */
  Matrix<T> adjoint() const;

  /**
   * @brief Extracts a submatrix from specified row and column ranges.
   *
   * Extracts rows [row_start, row_end) and columns [col_start, col_end) using
   * exclusive upper bounds.
   *
   * @param row_start Starting row index (inclusive)
   * @param row_end Ending row index (exclusive)
   * @param col_start Starting column index (inclusive)
   * @param col_end Ending column index (exclusive)
   * @return Extracted submatrix
   * @throws std::out_of_range if indices are out of bounds
   */
  Matrix<T> submatrix(int row_start,
                      int row_end,
                      int col_start,
                      int col_end) const;

  /**
   * @brief Computes the rank of the matrix.
   *
   * The rank is the number of linearly independent rows or columns.
   *
   * @return Rank of the matrix
   */
  T rank() const;

  /**
   * @brief Checks if the matrix is symmetric.
   *
   * A matrix is symmetric if A = A^T.
   *
   * @return true if matrix is symmetric, false otherwise
   * @throws std::invalid_argument if matrix is not square
   */
  bool isSymmetric() const;

  // ==================== Decompositions ====================

  /**
   * @brief Performs LU decomposition of the matrix.
   *
   * Decomposes the matrix into lower triangular (L) and upper triangular (U)
   * matrices such that A = LU.
   *
   * @return Pair of matrices (L, U)
   * @throws std::invalid_argument if matrix is not square
   * @throws std::runtime_error if decomposition fails (singular matrix)
   */
  std::pair<Matrix<T>, Matrix<T>> LUDecomposition() const;

  /**
   * @brief Performs QR decomposition of the matrix using Gram-Schmidt
   * orthogonalization.
   *
   * Decomposes the matrix into an orthogonal matrix (Q) and an upper triangular
   * matrix (R) such that A = QR.
   *
   * @return Pair of matrices (Q, R)
   * @throws std::invalid_argument if matrix has more columns than rows
   */
  std::pair<Matrix<T>, Matrix<T>> QRDecomposition() const;

  // ==================== Vector Operations ====================

  /**
   * @brief Performs matrix-vector multiplication.
   *
   * @param v Vector to multiply with
   * @return Resulting vector after multiplication
   * @throws std::invalid_argument if vector size doesn't match matrix columns
   */
  std::vector<T> matVec(const std::vector<T>& v) const;

  /**
   * @brief Computes the dot product of two vectors.
   *
   * @param a First vector
   * @param b Second vector
   * @return Dot product value
   * @throws std::invalid_argument if vectors have different sizes
   */
  T dot(const std::vector<T>& a, const std::vector<T>& b) const;

  /**
   * @brief Normalizes a vector to unit length (in-place).
   *
   * @param v Vector to normalize
   * @throws std::runtime_error if vector has zero norm
   */
  void normalize(std::vector<T>& v) const;

  /**
   * @brief Performs the Lanczos algorithm for tridiagonalization.
   *
   * The Lanczos algorithm is an iterative method for computing eigenvalues and
   * eigenvectors of large sparse symmetric matrices. It reduces the matrix to
   * tridiagonal form.
   *
   * @param m Number of Lanczos iterations
   * @param alpha Output vector of diagonal elements (size m)
   * @param beta Output vector of off-diagonal elements (size m-1)
   * @param Q Output matrix of orthonormal Lanczos vectors (n × m)
   * @throws std::invalid_argument if matrix is not square or m > matrix size
   */
  void lanczos(int m,
               std::vector<T>& alpha,
               std::vector<T>& beta,
               Matrix<T>& Q) const;

  /**
   * @brief Constructs a tridiagonal matrix from diagonal and off-diagonal
   * elements.
   *
   * @param alpha Diagonal elements
   * @param beta Off-diagonal elements
   * @param m Dimension of the tridiagonal matrix
   * @return m × m tridiagonal matrix
   */
  Matrix<T> buildTridiagonal(const std::vector<T>& alpha,
                             const std::vector<T>& beta,
                             int m) const;

  // ==================== Matrix Norms ====================

  /**
   * @brief Computes the Frobenius norm of the matrix.
   *
   * The Frobenius norm is the square root of the sum of squares of all
   * elements.
   *
   * @return Frobenius norm value
   */
  double frobeniusNorm() const;

  /**
   * @brief Computes the spectral norm (2-norm) of the matrix.
   *
   * The spectral norm is the largest singular value, equal to the square root
   * of the largest eigenvalue of A^T * A.
   *
   * @return Spectral norm value
   */
  double spectralNorm() const;

  /**
   * @brief Computes the Euclidean (L2) norm of a vector.
   *
   * @param v Vector to compute norm for
   * @return L2 norm value
   */
  double norm(const std::vector<T>& v) const;

  // ==================== Utility ====================

  /**
   * @brief Gets the minor matrix by removing a specified row and column.
   *
   * Used in computing determinants and adjoints via cofactor expansion.
   *
   * @param row Row to remove (0-based)
   * @param col Column to remove (0-based)
   * @return (rows-1) × (cols-1) minor matrix
   * @throws std::out_of_range if row or column index is out of bounds
   */
  Matrix<T> getMinor(int row, int col) const;

  /**
   * @brief Converts the matrix to a formatted string representation.
   *
   * @return String representation of the matrix
   */
  std::string toString() const;

  /**
   * @brief Gets the number of rows in the matrix.
   *
   * @return Number of rows
   */
  int getRows() const { return rows; }

  /**
   * @brief Gets the number of columns in the matrix.
   *
   * @return Number of columns
   */
  int getCols() const { return cols; }

  // ==================== Static Factory Methods ====================

  /**
   * @brief Creates an n × n identity matrix.
   *
   * The identity matrix has ones on the main diagonal and zeros elsewhere.
   *
   * @param n Dimension of the identity matrix
   * @return n × n identity matrix
   */
  static Matrix<T> identity(int n);

  /**
   * @brief Creates an r × c matrix filled with zeros.
   *
   * @param r Number of rows
   * @param c Number of columns
   * @return r × c zero matrix
   */
  static Matrix<T> zeros(int r, int c);

  /**
   * @brief Creates an r × c matrix filled with ones.
   *
   * @param r Number of rows
   * @param c Number of columns
   * @return r × c matrix of ones
   */
  static Matrix<T> ones(int r, int c);
};
