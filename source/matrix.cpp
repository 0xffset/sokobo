#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "include/matrix.h"

#include "include/complex_number.h"

/**
 * @brief Computes the square root of a value with type-safe handling for
 * arithmetic and complex types.
 *
 * This template function provides a unified interface for computing square
 * roots across different numeric types. For arithmetic types, it returns the
 * square root of the value. For complex types, it returns the square root of
 * the magnitude (modulus) as a double.
 *
 * @tparam T The type of the input value. Can be arithmetic (integral or
 * floating-point) or complex.
 *
 * @param value The value whose square root is to be computed.
 *
 * @return For arithmetic types: returns the square root as type T (integral
 * types are cast to double for computation, then cast back to T). For
 * floating-point types: returns std::sqrt(value). For complex types: returns
 * the square root of the magnitude as a double.
 *
 * @note For integral types, the result is truncated when cast back to the
 * original type.
 * @note For complex types, this computes sqrt(|z|), not the principal complex
 * square root.
 *
 * Example usage:
 * @code
 * double d = safe_sqrt(4.0);           // Returns 2.0
 * int i = safe_sqrt(9);                // Returns 3
 * ComplexNumber c(3.0, 4.0);
 * double mag_sqrt = safe_sqrt(c);      // Returns sqrt(5.0) ≈ 2.236
 * @endcode
 */
template<typename T>
auto safe_sqrt(T value) ->
    typename std::conditional_t<std::is_arithmetic_v<T>, T, double>
{
  if constexpr (std::is_floating_point_v<T>) {
    return std::sqrt(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    return static_cast<T>(std::sqrt(static_cast<double>(value)));
  } else {
    // For complex types, return the square root of the magnitude as double
    return std::sqrt(value.magnitude());
  }
}

/**
 * @brief Computes the absolute value of a value with type-safe handling for
 * arithmetic and complex types.
 *
 * This template function provides a unified interface for computing absolute
 * values across different numeric types. For arithmetic types, it returns the
 * absolute value. For complex types, it returns the absolute value of the
 * magnitude (modulus).
 *
 * @tparam T The type of the input value. Can be arithmetic (integral or
 * floating-point) or complex.
 *
 * @param value The value whose absolute value is to be computed.
 *
 * @return For arithmetic types: returns the absolute value as type T. For
 * floating-point types: returns std::abs(value). For integral types: value is
 * cast to double, abs is computed, then cast back to T. For complex types:
 * returns the absolute value of the magnitude as a double.
 *
 * @note For integral types, the intermediate double conversion ensures proper
 * handling of the most negative value (e.g., INT_MIN).
 * @note For complex types, this computes |magnitude(z)|, which is equivalent to
 * |z| for properly implemented magnitude functions.
 *
 * Example usage:
 * @code
 * double d = safe_abs(-4.5);           // Returns 4.5
 * int i = safe_abs(-7);                // Returns 7
 * ComplexNumber c(-3.0, -4.0);
 * double mag_abs = safe_abs(c);        // Returns 5.0
 * @endcode
 */
template<typename T>
auto safe_abs(T value) ->
    typename std::conditional_t<std::is_arithmetic_v<T>, T, double>
{
  if constexpr (std::is_floating_point_v<T>) {
    return std::abs(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    return static_cast<T>(std::abs(static_cast<double>(value)));
  } else {
    // For complex types, return the absolute value of the magnitude as double
    return std::abs(value.magnitude());
  }
}

template<typename T>
double getAbsoluteValue(const T& value)
{
  if constexpr (std::is_arithmetic_v<T>) {
    // For built-in numeric types
    return std::abs(static_cast<double>(value));
  } else {
    // For custom types like ComplexNumber
    return value
        .magnitude();  // or value.abs() depending on your implementation
  }
}

template<typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& mat)
{
  if (mat.empty() || mat[0].empty()) {
    throw std::invalid_argument("Matrix cannot be empty");
  }

  rows = mat.size();
  cols = mat[0].size();

  // Check that all rows have the same number of columns
  for (const auto& row : mat) {
    if (row.size() != cols) {
      throw std::invalid_argument(
          "All matrix rows must have the same  number of columns");
    }
  }

  data = mat;
}

template<typename T>
T& Matrix<T>::operator()(int row, int column)
{
  if (row < 0 || row >= rows || column < 0 || column >= cols) {
    throw std::out_of_range(
        std::string("Matrix index out of bounds\n") + "[row]: "
        + std::to_string(row) + " [col]: " + std::to_string(column) + "\n"
        + "[dimension]: " + std::to_string(rows) + "x" + std::to_string(cols));
  }
  return data[row][column];
}

template<typename T>
const T& Matrix<T>::operator()(int row, int column) const
{
  if (row < 0 || row >= rows || column < 0 || column >= cols) {
    throw std::out_of_range(
        std::string("Matrix index out of bounds\n") + "[row]: "
        + std::to_string(row) + " [col]: " + std::to_string(column) + "\n"
        + "[dimension]: " + std::to_string(rows) + "x" + std::to_string(cols));
  }
  return data[row][column];
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const
{
  if (rows != other.rows || cols != other.cols) {
    throw std::invalid_argument("Matrix dimensions must match for addition");
  }

  Matrix<T> result(rows, cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result(i, j) = data[i][j] + other(i, j);
    }
  }
  return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const
{
  if (rows != other.rows || cols != other.cols) {
    throw std::invalid_argument("Matrix dimensions must match for subtraction");
  }

  Matrix<T> result(rows, cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result(i, j) = data[i][j] - other(i, j);
    }
  }
  return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
{
  if (cols != other.rows) {
    throw std::invalid_argument(
        "Matrix dimensions incompatible for multiplication");
  }

  Matrix<T> result(rows, other.cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < other.cols; ++j) {
      T sum = T {};
      for (int k = 0; k < cols; ++k) {
        sum += data[i][k] * other(k, j);
      }
      result(i, j) = sum;
    }
  }
  return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T& scalar) const
{
  Matrix<T> result(rows, cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result(i, j) = data[i][j] * scalar;
    }
  }
  return result;
}

template<typename T>
T Matrix<T>::rank() const
{
  std::vector<std::vector<T>> matrix = data;
  int n = matrix.size();
  int m = matrix[0].size();
  const double EPS = 1E-9;
  int rank = 0;
  std::vector<bool> row_selected(n, false);

  for (int i = 0; i < m; ++i) {
    int j;
    for (j = 0; j < n; ++j) {
      if (!row_selected[j] && abs(matrix[j][i]) > EPS) {
        break;
      }
    }
    if (j != n) {
      ++rank;
      row_selected[j] = true;
      for (int p = i + 1; p < m; ++p) {
        matrix[j][p] /= matrix[j][i];
      }
      for (int k = 0; k < n; ++k) {
        if (k != j && abs(matrix[k][i]) > EPS) {
          for (int p = i + 1; p < m; ++p) {
            matrix[k][p] -= (matrix[j][p] * matrix[k][i]);
          }
        }
      }
    }
  }
  return rank;
}

template<typename T>
bool Matrix<T>::isSymmetric() const
{
  if (getRows() != getCols()) {
    return false;
  }

  int rows = getRows();
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < i; j++) {
      if (data[i][j] != data[j][i]) {
        return false;
      }
    }
  }
  return true;
}

template<typename T>
T Matrix<T>::determinant() const
{
  if (rows != cols) {
    throw std::invalid_argument(
        "Determinant is only defined for square matrices");
  }

  if (rows == 1) {
    return data[0][0];
  }

  if (rows == 2) {
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
  }

  // Use LU decomposition for larger matrices
  auto [L, U] = LUDecomposition();

  T det = T {1};
  for (int i = 0; i < rows; ++i) {
    det *= U(i, i);
  }

  return det;
}

template<typename T>
Matrix<T> Matrix<T>::inverse() const
{
  if (rows != cols) {
    throw std::invalid_argument("Inverse is only defined for square matrices");
  }

  T det = determinant();
  if (safe_abs(det) < 1e-12) {
    throw std::runtime_error("Matrix is singular (determinant is zero)");
  }

  if (rows == 1) {
    Matrix<T> result(1, 1);
    result[0][0] = T {1} / data[0][0];
    return result;
  }

  if (rows == 2) {
    Matrix<T> result(2, 2);
    T invDet = T {1} / det;
    result(0, 0) = data[1][1] * invDet;
    result(0, 1) = -data[0][1] * invDet;
    result(1, 0) = -data[1][0] * invDet;
    result(1, 1) = data[0][0] * invDet;
    return result;
  }

  // Gauss-Jordan elimination for larger matrices
  Matrix<T> augmented(rows, 2 * cols);

  // Create augmented matrix [A|I]
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      augmented(i, j) = data[i][j];
      augmented(i, j + cols) = (i == j) ? T {1} : T {};
    }
  }

  // Forward elimination
  for (int i = 0; i < rows; ++i) {
    // Find pivot
    int maxRow = i;
    for (int k = i + 1; k < rows; ++k) {
      if (safe_abs(augmented(k, i)) > safe_abs(augmented(maxRow, i))) {
        maxRow = k;
      }
    }

    // Swap rows
    if (maxRow != i) {
      for (int j = 0; j < 2 * cols; ++j) {
        std::swap(augmented(i, j), augmented(maxRow, j));
      }
    }

    // Scale pivot row
    T pivot = augmented(i, i);
    if (safe_abs(pivot) < 1e-12) {
      throw std::runtime_error("Matrix is singular");
    }

    for (int j = 0; j < 2 * cols; ++j) {
      augmented(i, j) /= pivot;
    }

    // Eliminate column
    for (int k = 0; k < rows; ++k) {
      if (k != i) {
        T factor = augmented(k, i);
        for (int j = 0; j < 2 * cols; ++j) {
          augmented(k, j) -= factor * augmented(i, j);
        }
      }
    }
  }

  // Extract inverse matrix
  Matrix<T> result(rows, cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result(i, j) = augmented(i, j + cols);
    }
  }

  return result;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const
{
  Matrix<T> result(cols, rows);
  const int blockSize = 16;

  for (int i = 0; i < rows; i += blockSize) {
    for (int j = 0; j <= cols; j += blockSize) {
      for (int ii = i; ii < std::min(i + blockSize, rows); ++ii) {
        for (int jj = j; jj < std::min(j + blockSize, rows); ++jj) {
          result(jj, ii) = data[ii][jj];
        }
      }
    }
  }
  return result;
}

template<typename T>
Matrix<T> Matrix<T>::adjoint() const
{
  if (rows != cols) {
    throw std::invalid_argument("Adjoint requires a square matrix.");
  }
  Matrix<T> adj(rows, cols);

  if (rows == 1) {
    adj(0, 0) = T(1);
    return adj;
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Matrix<T> minorMat = getMinor(i, j);
      T det = minorMat.determinant();
      T cofactor = ((i + j) % 2 == 0) ? det : (det * (T(-1)));
      adj(j, i) = cofactor;
    }
  }
  return adj;
}

template<typename T>
std::vector<T> Matrix<T>::eigenvalues() const
{
  if (rows != cols) {
    throw std::invalid_argument(
        "Eigenvalues are only defined for square matrices");
  }

  // For 2x2 matrices, use analytical solution
  if (rows == 2) {
    T trace = data[0][0] + data[1][1];
    T det = data[0][0] * data[1][1] - data[0][1] * data[1][0];
    T discriminant = trace * trace - T {4} * det;

    std::vector<T> eigenvals(2);
    if (discriminant >= 0) {
      T sqrtDisc = safe_sqrt(discriminant);
      eigenvals[0] = (trace + sqrtDisc) / T {2};
      eigenvals[1] = (trace - sqrtDisc) / T {2};
    } else {
      // Complex eigenvalues - return real parts only for now
      eigenvals[0] = trace / T {2};
      eigenvals[1] = trace / T {2};
    }
    return eigenvals;
  }

  // For larger matrices, use QR algorithm (simplified version)
  Matrix<T> A = *this;
  std::vector<T> eigenvals;

  // Power iteration for dominant eigenvalue (simplified approach)
  const int maxIter = 1000;
  const double tolerance = 1e-10;

  for (int ev = 0; ev < std::min(rows, 3);
       ++ev) {  // Compute first few eigenvalues
    std::vector<T> v(rows, T {1});

    for (int iter = 0; iter < maxIter; ++iter) {
      std::vector<T> Av(rows, T {});
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          Av[i] += A(i, j) * v[j];
        }
      }

      // Normalize
      T norm = T {};
      for (int i = 0; i < rows; ++i) {
        norm += Av[i] * Av[i];
      }
      norm = safe_sqrt(norm);

      if (norm < tolerance) {
        break;
      }

      for (int i = 0; i < rows; ++i) {
        Av[i] /= norm;
      }

      // Check convergence
      T diff = T {};
      for (int i = 0; i < rows; ++i) {
        diff += (Av[i] - v[i]) * (Av[i] - v[i]);
      }

      v = Av;

      if (safe_sqrt(diff) < tolerance) {
        break;
      }
    }

    // Compute eigenvalue
    T eigenval = T {};
    T vNorm = T {};
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        eigenval += v[i] * A(i, j) * v[j];
      }
      vNorm += v[i] * v[i];
    }
    eigenval /= vNorm;
    eigenvals.push_back(eigenval);

    // Deflate matrix (simplified)
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        A(i, j) -= eigenval * v[i] * v[j];
      }
    }
  }

  return eigenvals;
}

// template<typename T>
// Matrix<T> Matrix<T>::eigenvectors() const
// {
//   // Simplified implementation - returns identity matrix
//   // Full implementation would require more sophisticated algorithms
//   return identity(rows);
// }

template<typename T>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::LUDecomposition() const
{
  if (rows != cols) {
    throw std::invalid_argument("LU decomposition requires a square matrix");
  }

  Matrix<T> L = identity(rows);
  Matrix<T> U = *this;

  for (int i = 0; i < rows - 1; ++i) {
    for (int j = i + 1; j < rows; ++j) {
      if (safe_abs(U(i, i)) < 1e-12) {
        throw std::runtime_error(
            "Matrix is singular - cannot perform LU decomposition");
      }

      T factor = U(j, i) / U(i, i);
      L(j, i) = factor;

      for (int k = i; k < cols; ++k) {
        U(j, k) -= factor * U(i, k);
      }
    }
  }

  return {L, U};
}

// QR (Gram-Schmidt)
template<typename T>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::QRDecomposition() const
{
  Matrix<T> Q(rows, cols);  // Start with empty Q
  Matrix<T> R = zeros(cols, cols);

  // Gram-Schmidt process
  for (int j = 0; j < cols; ++j) {
    // Extract column j from ORIGINAL matrix (this)
    std::vector<T> v(rows);
    for (int i = 0; i < rows; ++i) {
      v[i] = (*this)(i, j);  // Read from original matrix, not Q
    }

    // Orthogonalize against previous Q columns
    for (int k = 0; k < j; ++k) {
      T dot = T {};
      for (int i = 0; i < rows; ++i) {
        dot += Q(i, k) * v[i];  // Use already-computed Q columns
      }
      R(k, j) = dot;

      for (int i = 0; i < rows; ++i) {
        v[i] -= dot * Q(i, k);
      }
    }

    // Normalize
    T norm = T {};
    for (int i = 0; i < rows; ++i) {
      norm += v[i] * v[i];
    }
    norm = safe_sqrt(norm);
    R(j, j) = norm;

    if (norm > 1e-12) {
      for (int i = 0; i < rows; ++i) {
        Q(i, j) = v[i] / norm;  // Write to Q
      }
    } else {
      // Handle zero norm case
      for (int i = 0; i < rows; ++i) {
        Q(i, j) = 0;
      }
    }
  }

  return {Q, R};
}

double abs(const ComplexNumber& c)
{
  return c.magnitude();
}

// Then your original code will work:
template<typename T>
double Matrix<T>::frobeniusNorm() const
{
  double sum = 0.0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      // Better to use:
      double val = abs(data[i][j]);  // Uses ADL to find your overload
      sum += val * val;
    }
  }
  return std::sqrt(sum);
}

template<typename T>
double Matrix<T>::spectralNorm() const
{
  // Simplified implementation using power method
  if (rows == 0 || cols == 0) {
    return 0.0;
  }

  Matrix<T> AtA = transpose() * (*this);
  std::vector<T> eigenvals = AtA.eigenvalues();

  double maxEigenval = 0.0;
  for (const auto& val : eigenvals) {
    double absVal = getAbsoluteValue(val);
    if (absVal > maxEigenval) {
      maxEigenval = absVal;
    }
  }

  return std::sqrt(maxEigenval);
}

template<typename T>
Matrix<T> Matrix<T>::getMinor(int row, int col) const
{
  if (rows <= 1 || cols <= 1) {
    throw std::invalid_argument("Matrix too small for minor");
  }

  Matrix<T> minor(rows - 1, cols - 1);
  int minorRow = 0;

  for (int i = 0; i < rows; ++i) {
    if (i == row) {
      continue;
    }

    int minorCol = 0;
    for (int j = 0; j < cols; ++j) {
      if (j == col) {
        continue;
      }
      minor(minorRow, minorCol) = data[i][j];
      ++minorCol;
    }
    ++minorRow;
  }

  return minor;
}

template<typename T>
std::string Matrix<T>::toString() const
{
  std::ostringstream oss;
  oss << "[" << '\n';
  for (int i = 0; i < rows; ++i) {
    if (i > 0) {
      oss << ",";
    }
    oss << "[";
    for (int j = 0; j < cols; ++j) {
      if (j > 0) {
        oss << ", ";
      }
      oss << data[i][j];
    }
    oss << "]" << '\n';
  }
  oss << "]";
  return oss.str();
}

template<typename T>
Matrix<T> Matrix<T>::identity(int n)
{
  if (n <= 0) {
    throw std::invalid_argument("Identity matrix size must be positive");
  }

  Matrix<T> result(n, n);
  for (int i = 0; i < n; ++i) {
    result(i, i) = T {1};
  }
  return result;
}

// Helper
template<typename T>
std::vector<T> Matrix<T>::matVec(const std::vector<T>& v) const
{
  std::vector<T> r(rows, 0);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      r[i] += data[i][j] * v[j];
    }
  }
  return r;
}

template<typename T>
T Matrix<T>::dot(const std::vector<T>& a, const std::vector<T>& b) const
{
  T s = 0;
  for (int i = 0; i < a.size(); ++i) {
    s += a[i] * b[i];
  }
  return s;
}

template<typename T>
double Matrix<T>::norm(const std::vector<T>& v) const
{
  return std::sqrt((double)dot(v, v));
}

template<typename T>
void Matrix<T>::normalize(std::vector<T>& v) const
{
  double n = norm(v);
  if (n == 0) {
    return;
  }
  for (auto& x : v) {
    x /= n;
  }
}

// Extract a submatrix from (row_start, col_start) to (row_end, col_end)
// [exclusive]
template<typename T>
Matrix<T> Matrix<T>::submatrix(int row_start,
                               int row_end,
                               int col_start,
                               int col_end) const
{
  // Validate bounds
  if (row_start < 0 || row_end > rows || col_start < 0 || col_end > cols) {
    throw std::out_of_range("Submatrix indices out of bounds");
  }
  if (row_start >= row_end || col_start >= col_end) {
    throw std::invalid_argument("Invalid submatrix range");
  }

  int sub_rows = row_end - row_start;
  int sub_cols = col_end - col_start;

  Matrix<T> result(sub_rows, sub_cols);

  for (int i = 0; i < sub_rows; ++i) {
    for (int j = 0; j < sub_cols; ++j) {
      result(i, j) = (*this)(row_start + i, col_start + j);
    }
  }

  return result;
}

// Lanczos's algorithm implementation to find eigenvectors
template<typename T>
void Matrix<T>::lanczos(int m,
                        std::vector<T>& alpha,
                        std::vector<T>& beta,
                        Matrix<T>& Q) const
{
  int n = rows;
  alpha.resize(m);
  beta.resize(m - 1);  // FIX: beta has m-1 elements
  Q = Matrix<T>(n, m);

  std::vector<T> q(n, 1);
  normalize(q);

  std::vector<T> q_prev(n, 0);

  for (int j = 0; j < m; ++j) {
    auto v = matVec(q);

    if (j > 0) {
      for (int i = 0; i < n; ++i) {
        v[i] -= beta[j - 1] * q_prev[i];
      }
    }
    alpha[j] = dot(q, v);

    for (int i = 0; i < n; ++i) {
      v[i] -= alpha[j] * q[i];
    }

    // FIX: Store current q in column j of Q before computing next q
    for (int i = 0; i < n; ++i) {
      Q(i, j) = q[i];  // Store in column j, not row j
    }

    // FIX: Only compute beta if not last iteration
    if (j < m - 1) {
      beta[j] = norm(v);
      if (beta[j] < 1e-10) {
        // FIX: Resize arrays to actual size
        alpha.resize(j + 1);
        beta.resize(j);
        Q = Q.submatrix(0, n, 0, j + 1);  // Assuming you have submatrix method
        break;
      }

      q_prev = q;
      for (int i = 0; i < n; ++i) {
        q[i] = v[i] / beta[j];
      }
    }
  }
}

// Build T
template<typename T>
Matrix<T> Matrix<T>::buildTridiagonal(const std::vector<T>& alpha,
                                      const std::vector<T>& beta,
                                      int m) const
{
  Matrix<T> Tm(m, m);
  for (int i = 0; i < m; ++i) {
    Tm(i, i) = alpha[i];
    if (i > 0) {
      Tm(i, i - 1) = beta[i - 1];
    }
    if (i < m - 1) {
      Tm(i, i + 1) = beta[i];
    }
  }
  return Tm;
}

template<typename T>
Matrix<T> Matrix<T>::eigenvectors() const
{
  int m = std::min(rows, 20);
  std::vector<T> alpha, beta;
  Matrix<T> Ql;

  lanczos(m, alpha, beta, Ql);

  // FIX: Use actual size (in case of early termination)
  int actual_m = alpha.size();
  Matrix<T> Tm = buildTridiagonal(alpha, beta, actual_m);

  Matrix<T> V = Matrix<T>::identity(actual_m);

  for (int i = 0; i < 100; ++i) {
    auto [Qk, Rk] = Tm.QRDecomposition();
    Tm = Rk * Qk;
    V = V * Qk;
  }

  Matrix<T> result(rows, actual_m);

  // FIX: Now Ql(i, j) correctly has vectors in columns
  for (int k = 0; k < actual_m; ++k) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < actual_m; ++j) {
        result(i, k) += Ql(i, j) * V(j, k);
      }
    }
  }
  return result;
}

template<typename T>
Matrix<T> Matrix<T>::zeros(int r, int c)
{
  if (r <= 0 || c <= 0) {
    throw std::invalid_argument("Matrix dimensions must be positive");
  }

  return Matrix<T>(r, c);  // Default constructor initializes with zeros
}

template<typename T>
Matrix<T> Matrix<T>::ones(int r, int c)
{
  if (r <= 0 || c <= 0) {
    throw std::invalid_argument("Matrix dimensions must be positive");
  }

  Matrix<T> result(r, c);
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      result(i, j) = T {1};
    }
  }
  return result;
}

// Explicit template instantiations
template class Matrix<double>;
template class Matrix<float>;
template class Matrix<int>;
// template class Matrix<ComplexNumber>;
