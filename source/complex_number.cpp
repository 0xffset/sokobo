#include <cmath>
#include <sstream>
#include <stdexcept>
#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#  define M_E 2.71828
#endif
#include "include/complex_number.h"
#include "include/numerical_methods.h"

//ComplexNumber::ComplexNumber(double real, double imag)
//    : value(real, imag)
//{
//}



ComplexNumber::ComplexNumber(std::complex<double> val)
    : value(val)
{
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const
{
  return ComplexNumber(value + other.value);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const
{

  return ComplexNumber(value - other.value);
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) const
{
  return ComplexNumber(value * other.value);
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber& other) const
{
  if (std::abs(other.value) < 1e-10) {
    throw std::runtime_error("Division by zero complex number");
  }
  return ComplexNumber(value / other.value);
}

ComplexNumber ComplexNumber::operator^(int n) const
{
  if (n == 0) {
    return ComplexNumber(1, 0);
  }

  ComplexNumber result(1, 0);
  ComplexNumber base = *this;
  int exp = std::abs(n);

  while (exp > 0) {
    if (exp % 2 == 1) {
      result = result * base;
    }
    base = base * base;
    exp /= 2;
  }

  if (n < 0) {
    return ComplexNumber(1, 0) / result;
  }

  return result;
}

ComplexNumber ComplexNumber::conjugate() const
{
  return ComplexNumber(std::conj(value));
}

double ComplexNumber::magnitude() const
{
  return std::abs(value);
}

double ComplexNumber::phase() const
{
  return std::arg(value);
}

ComplexNumber ComplexNumber::exp() const
{
  return ComplexNumber(std::exp(value));
}

ComplexNumber ComplexNumber::log() const
{
  if (std::abs(value) < 1e-10) {
    throw std::runtime_error("Logarithm of zero");
  }
  return ComplexNumber(std::log(value));
}

ComplexNumber ComplexNumber::sin() const
{
  return ComplexNumber(std::sin(value));
}

ComplexNumber ComplexNumber::cos() const
{
  return ComplexNumber(std::cos(value));
}

ComplexNumber ComplexNumber::tan() const
{
  return ComplexNumber(std::tan(value));
}

ComplexNumber ComplexNumber::sqrt() const
{
  return ComplexNumber(std::sqrt(value));
}

std::vector<ComplexNumber> ComplexNumber::rootsOfUnity(int n)
{
  if (n <= 0) {
    throw std::runtime_error("n must be positive for roots of unity");
  }

  std::vector<ComplexNumber> roots;
  double angle = 2.0 * M_PI / n;

  for (int k = 0; k < n; k++) {
    double theta = k * angle;
    roots.emplace_back(std::cos(theta), std::sin(theta));
  }

  return roots;
}

std::string ComplexNumber::toString() const
{
  std::ostringstream oss;
  double real = value.real();
  double imag = value.imag();

  if (std::abs(imag) < 1e-10) {
    oss << real;
  } else if (std::abs(real) < 1e-10) {
    if (std::abs(imag - 1) < 1e-10) {
      oss << "i";
    } else if (std::abs(imag + 1) < 1e-10) {
      oss << "-i";
    } else {
      oss << imag << "i";
    }
  } else {
    oss << real;
    if (imag > 0) {
      if (std::abs(imag - 1) < 1e-10) {
        oss << " + i";
      } else {
        oss << " + " << imag << "i";
      }
    } else {
      if (std::abs(imag + 1) < 1e-10) {
        oss << " - i";
      } else {
        oss << " - " << (-imag) << "i";
      }
    }
  }

  return oss.str();
}

std::string ComplexNumber::toPolarString() const
{
  double r = magnitude();
  double theta = phase();

  std::ostringstream oss;
  oss << r << " * e^(i * " << theta << ")";
  return oss.str();
}
