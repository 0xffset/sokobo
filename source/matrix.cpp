#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "include/matrix.h"

#include "include/complex_number.h"
#include <cstdlib>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// template<typename T>
// Matrix<T>::Matrix(int r, int c)
//     : rows(r)
//     , cols(c)
//{
//   if (r <= 0 || c <= 0) {
//     throw std::invalid_argument("Matrix dimensions must be positive");
//   }
//   data.resize(rows, std::vector<T>(cols, T {}));
// }

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

template<typename T>
auto safe_abs(T value) ->
    typename std::conditional_t<std::is_arithmetic_v<T>, T, double>
{
  if constexpr (std::is_floating_point_v<T>) {
    return std::abs(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    return static_cast<T>(std::abs(static_cast<double>(value)));
  } else {
    // For complex types, return the square root of the magnitude as double
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
          "All matrix rows must have the same number of columns");
    }
  }

  data = mat;
}

template<typename T>
T& Matrix<T>::operator()(int row, int column)
{
  if (row < 0 || row >= rows ||column < 0 || column >= cols) {
    throw std::out_of_range("Matrix index out of bounds");
  }
  return data[row][column];
}

template<typename T>
const T& Matrix<T>::operator()(int row, int column) const
{
  if (row < 0 || row >= rows || column < 0 || column >= cols) {
    throw std::out_of_range("Matrix index out of bounds");
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
    result[0][0] = T{1} / data[0][0];
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
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result(j, i) = data[i][j];
    }
  }
  return result;
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

  for (int ev = 0; ev < std::min(rows, 3); ++ev)
  {  // Compute first few eigenvalues
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

template<typename T>
Matrix<T> Matrix<T>::eigenvectors() const
{
  // Simplified implementation - returns identity matrix
  // Full implementation would require more sophisticated algorithms
  return identity(rows);
}

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

template<typename T>
std::pair<Matrix<T>, Matrix<T>> Matrix<T>::QRDecomposition() const
{
  Matrix<T> Q = *this;
  Matrix<T> R = zeros(cols, cols);

  // Gram-Schmidt process
  for (int j = 0; j < cols; ++j) {
    // Extract column j
    std::vector<T> v(rows);
    for (int i = 0; i < rows; ++i) {
      v[i] = Q(i, j);
    }

    // Orthogonalize against previous columns
    for (int k = 0; k < j; ++k) {
      T dot = T {};
      for (int i = 0; i < rows; ++i) {
        dot += Q(i, k) * v[i];
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
        Q(i, j) = v[i] / norm;
      }
    }
  }

  return {Q, R};
}

// Add this outside your class, in the same namespace as ComplexNumber
double abs(const ComplexNumber& c)
{
  return c.magnitude();  // or whatever method calculates |c|
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
  oss << "[";
  for (int i = 0; i < rows; ++i) {
    if (i > 0) {
      oss << ", ";
    }
    oss << "[";
    for (int j = 0; j < cols; ++j) {
      if (j > 0) {
        oss << ", ";
      }
      oss << data[i][j];
    }
    oss << "]";
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
template class Matrix<ComplexNumber>;

