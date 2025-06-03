#pragma once
#include <complex>
#include <ostream>
#include <string>
#include <vector>

class ComplexNumber
{
private:
  std::complex<double> value;
  double real;
  double img;

public:
  ComplexNumber(double real = 0, double imag = 0)
      : value(real, imag) {};

  ComplexNumber(std::complex<double> val);

  ComplexNumber operator-() const
  {
    return ComplexNumber(-value.real(), -value.imag());
  }

  bool operator>=(const ComplexNumber& other) const
  {
    return this->magnitude() >= other.magnitude();
  }

  bool operator>(const ComplexNumber& other) const
  {
    return this->magnitude() > other.magnitude();
  }

  bool operator<(const ComplexNumber& other) const
  {
    return this->magnitude() < other.magnitude();
  }

  friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& c)
  {
    os << c.toString();
    return os;
  }

  ComplexNumber& operator/=(const ComplexNumber& other)
  {
    this->value /= other.value;  // usa std::complex<double>::operator/=
    return *this;
  }

  ComplexNumber& operator-=(const ComplexNumber& other)
  {
    this->value -= other.value;  // usa std::complex<double>::operator-=
    return *this;
  }

   ComplexNumber& operator+=(const ComplexNumber& other)
  {
    this->value += other.value;  // usa std::complex<double>::operator-=
    return *this;
  }

      ComplexNumber& operator*=(const ComplexNumber& other)
  {
    this->value *= other.value;  // usa std::complex<double>::operator-=
    return *this;
  }




  // Basic operations
  ComplexNumber operator+(const ComplexNumber& other) const;
  ComplexNumber operator-(const ComplexNumber& other) const;
  ComplexNumber operator*(const ComplexNumber& other) const;
  ComplexNumber operator/(const ComplexNumber& other) const;
  ComplexNumber operator^(int n) const;

  // Complex functions
  ComplexNumber conjugate() const;
  double magnitude() const;
  double phase() const;
  ComplexNumber exp() const;
  ComplexNumber log() const;
  ComplexNumber sin() const;
  ComplexNumber cos() const;
  ComplexNumber tan() const;
  ComplexNumber sqrt() const;

  // Roots of unity
  static std::vector<ComplexNumber> rootsOfUnity(int n);

  std::string toString() const;
  std::string toPolarString() const;

  std::complex<double> getValue() const { return value; }
};
