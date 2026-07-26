#include <stdexcept>

#include "include/vector.h"

#include "string"

template<typename T>
Vector<T>::Vector()
{
}

template<typename T>
Vector<T>::Vector(int size)
    : data(size, 0)
{
}

template<typename T>
Vector<T>::Vector(const std::vector<T>& vec)
    : data(vec)
{
}

template<typename T>
T& Vector<T>::operator[](int i)
{
  return data[i];
}

template<typename T>
const T& Vector<T>::operator[](int i) const
{
  return data[i];
}

template<typename T>
int Vector<T>::size() const
{
  return data.size();
}

// Arithmetic
template<typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& other) const
{
  if (size() != other.size()) {
    throw std::runtime_error("Size mismatch");
  }

  Vector<T> result(size());
  for (int i = 0; i < size(); ++i) {
    result[i] = data[i] + other[i];
  }

  return result;
}

template<typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& other) const
{
  if (size() != other.size()) {
    throw std::runtime_error("Size mismatch");
  }

  Vector<T> result(size());
  for (int i = 0; i < size(); ++i) {
    result[i] = data[i] - other[i];
  }

  return result;
}

template<typename T>
Vector<T> Vector<T>::operator*(T scalar) const
{
  Vector<T> result(size());
  for (int i = 0; i < size(); ++i) {
    result[i] = data[i] * scalar;
  }

  return result;
}

template<typename T>
Vector<T> operator*(const Matrix<T>& mat, const Vector<T>& vec)
{
  if (mat.getCols() != vec.size()) {
    throw std::invalid_argument("Matrix columns must match vector size.");
  }

  Vector<T> result(mat.getRows());
  for (int i = 0; i < mat.getRows(); ++i) {
    T sum = 0;
    for (int j = 0; j < mat.getCols(); ++j) {
      sum += mat(i, j) * vec[j];
    }
    result[i] = sum;
  }
  return result;
}

template<typename T>
Vector<T> Vector<T>::operator/(T scalar) const
{
  if (scalar == 0) {
    throw std::runtime_error("Division by zero");
  }

  Vector<T> result(size());
  for (int i = 0; i < size(); ++i) {
    result[i] = data[i] / scalar;
  }

  return result;
}

// Dot product
template<typename T>
T Vector<T>::dot(const Vector<T>& other) const
{
  if (size() != other.size()) {
    throw std::runtime_error("Size mismatch");
  }

  T sum = 0;
  for (int i = 0; i < size(); ++i) {
    sum += data[i] * other[i];
  }

  return sum;
}

// Norm
template<typename T>
double Vector<T>::norm() const
{
  return std::sqrt((double)dot(*this));
}

// Normalize
template<typename T>
void Vector<T>::normalize()
{
  double n = norm();
  if (n == 0) {
    return;
  }

  for (auto& x : data) {
    x /= n;
  }
}

// Utility
template<typename T>
std::string Vector<T>::toString() const
{
  std::stringstream ss;
  ss << "[ ";
  for (const auto& x : data) {
    ss << x << " ";
  }
  ss << "]";
  return ss.str();
}

// Static
template<typename T>
Vector<T> Vector<T>::zeros(int size)
{
  return Vector<T>(size);
}

template<typename T>
Vector<T> Vector<T>::ones(int size)
{
  Vector<T> v(size);
  for (int i = 0; i < size; ++i) {
    v[i] = 1;
  }
  return v;
}
template class Vector<double>;
template Vector<double> operator*<double>(const Matrix<double>& mat, const Vector<double>& vec);


template class Vector<float>;
template Vector<float> operator*<float>(const Matrix<float>& mat, const Vector<float>& vec);

template class Vector<int>;
template Vector<int> operator*<int>(const Matrix<int>& mat, const Vector<int>& vec);
