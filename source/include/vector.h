#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>

template<typename T>
class Vector
{
private:
  std::vector<T> data;

public:
  Vector();
  Vector(int size);
  Vector(const std::vector<T>& vec);

  T& operator[](int i);
  const T& operator[](int i) const;
  int size() const;

  Vector<T> operator+(const Vector<T>& other) const;
  Vector<T> operator-(const Vector<T>& other) const;
  Vector<T> operator*(T scalar) const;
  Vector<T> operator/(T scalar) const;

  T dot(const Vector<T>& other) const;
  double norm() const;
  void normalize();

  std::string toString() const;

  static Vector<T> zeros(int size);
  static Vector<T> ones(int size);
 
};
