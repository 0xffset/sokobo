#include "include/polynomial.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "include/polynomial.h"

Polynomial::Polynomial(const std::vector<double>& coeffs)
    : coefficients(coeffs)
{
  removeLeadingZeros();
}

Polynomial::Polynomial(double constant)
{
  coefficients.push_back(constant);
}

void Polynomial::removeLeadingZeros()
{
  while (coefficients.size() > 1 && std::abs(coefficients.back()) < 1e-10) {
    coefficients.pop_back();
  }
  if (coefficients.empty()) {
    coefficients.push_back(0);
  }
}

int Polynomial::degree() const
{
  return static_cast<int>(coefficients.size()) - 1;
}

double Polynomial::getCoeff(int i) const
{
  if (i < 0 || i >= static_cast<int>(coefficients.size())) {
    return 0;
  }
  return coefficients[i];
}

void Polynomial::setCoeff(int i, double val)
{
  if (i < 0) {
    return;
  }
  if (i >= static_cast<int>(coefficients.size())) {
    coefficients.resize(i + 1, 0);
  }
  coefficients[i] = val;
  removeLeadingZeros();
}

int Polynomial::getDegree() const
{
  return 0;
}

Polynomial Polynomial::operator+(const Polynomial& other) const
{
  int maxDegree = std::max(degree(), other.degree());
  std::vector<double> result(maxDegree + 1, 0);

  for (int i = 0; i <= maxDegree; i++) {
    result[i] = getCoeff(i) + other.getCoeff(i);
  }

  return Polynomial(result);
}

Polynomial Polynomial::operator-(const Polynomial& other) const
{
  int maxDegree = std::max(degree(), other.degree());
  std::vector<double> result(maxDegree + 1, 0);

  for (int i = 0; i <= maxDegree; i++) {
    result[i] = getCoeff(i) - other.getCoeff(i);
  }

  return Polynomial(result);
}

Polynomial Polynomial::operator*(const Polynomial& other) const
{
  if (degree() == 0 && std::abs(getCoeff(0)) < 1e-10) {
    return Polynomial(0);
  }
  if (other.degree() == 0 && std::abs(other.getCoeff(0)) < 1e-10) {
    return Polynomial(0);
  }

  int resultDegree = degree() + other.degree();
  std::vector<double> result(resultDegree + 1, 0);

  for (int i = 0; i <= degree(); i++) {
    for (int j = 0; j <= other.degree(); j++) {
      result[i + j] += getCoeff(i) * other.getCoeff(j);
    }
  }

  return Polynomial(result);
}

std::pair<Polynomial, Polynomial> Polynomial::divide(
    const Polynomial& divisor) const
{
  if (divisor.degree() == 0 && std::abs(divisor.getCoeff(0)) < 1e-10) {
    throw std::runtime_error("Division by zero polynomial");
  }

  Polynomial dividend = *this;
  Polynomial quotient(0);

  while (dividend.degree() >= divisor.degree()
         && !(dividend.degree() == 0 && std::abs(dividend.getCoeff(0)) < 1e-10))
  {
    double leadCoeff =
        dividend.coefficients.back() / divisor.coefficients.back();
    int degreeDiff = dividend.degree() - divisor.degree();

    std::vector<double> termCoeffs(degreeDiff + 1, 0);
    termCoeffs[degreeDiff] = leadCoeff;
    Polynomial term(termCoeffs);

    quotient = quotient + term;
    dividend = dividend - (term * divisor);
  }

  return {quotient, dividend};
}

Polynomial Polynomial::gcd(const Polynomial& other) const
{
  Polynomial a = *this;
  Polynomial b = other;

  while (!(b.degree() == 0 && std::abs(b.getCoeff(0)) < 1e-10)) {
    auto division = a.divide(b);
    a = b;
    b = division.second;
  }

  return a;
}

Polynomial Polynomial::derivative() const
{
  if (degree() == 0) {
    return Polynomial(0);
  }

  std::vector<double> result(degree(), 0);
  for (int i = 1; i <= degree(); i++) {
    result[i - 1] = i * getCoeff(i);
  }

  return Polynomial(result);
}

Polynomial Polynomial::integral() const
{
  std::vector<double> result(degree() + 2, 0);
  for (int i = 0; i <= degree(); i++) {
    result[i + 1] = getCoeff(i) / (i + 1);
  }
  return Polynomial(result);
}

double Polynomial::evaluate(double x) const
{
  if (coefficients.empty()) {
    return 0;
  }

  double result = coefficients[0];
  double x_power = 1;

  for (size_t i = 1; i < coefficients.size(); i++) {
    x_power *= x;
    result += coefficients[i] * x_power;
  }

  return result;
}

std::complex<double> Polynomial::evaluate(std::complex<double> x) const
{
  if (coefficients.empty()) {
    return std::complex<double>(0, 0);
  }

  std::complex<double> result(coefficients[0], coefficients[1]);
  std::complex<double> xPower(1, 1);

  for (size_t i = 1; i < coefficients.size(); i++) {
    xPower *= x;
    result += coefficients[i] * xPower;
  }

  return result;
}

std::vector<std::complex<double>> Polynomial::roots() const
{
  std::vector<std::complex<double>> result;

  if (degree() == 0) {
    return result;
  } if (degree() == 1) {
    result.push_back(std::complex<double>(-getCoeff(0) / getCoeff(1), 0));
  } else if (degree() == 2) {
    double a = getCoeff(2);
    double b = getCoeff(1);
    double c = getCoeff(0);

    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
      result.push_back(
          std::complex<double>((-b + std::sqrt(discriminant)) / (2 * a), 0));
      result.push_back(
          std::complex<double>((-b - std::sqrt(discriminant)) / (2 * a), 0));
    } else {
      result.push_back(std::complex<double>(
          -b / (2 * a), std::sqrt(-discriminant) / (2 * a)));
      result.push_back(std::complex<double>(
          -b / (2 * a), -std::sqrt(-discriminant) / (2 * a)));
    }
  }

  return result;
}

std::vector<Polynomial> Polynomial::factor() const
{
  std::vector<Polynomial> factors;

  if (degree() <= 1) {
    factors.push_back(*this);
    return factors;
  }

  auto r = roots();
  for (const auto& root : r) {
    if (std::abs(root.imag()) < 1e-10) {
      std::vector<double> linearCoeffs = {-root.real(), 1};
      factors.push_back(Polynomial(linearCoeffs));
    }
  }

  if (factors.empty()) {
    factors.push_back(*this);
  }

  return factors;
}

std::string Polynomial::toString() const
{
  if (coefficients.empty()
      || (coefficients.size() == 1 && std::abs(coefficients[0]) < 1e-10))
  {
    return "0";
  }

  std::ostringstream oss;
  bool first = true;

  for (int i = degree(); i >= 0; i--) {
    double coeff = getCoeff(i);
    if (std::abs(coeff) < 1e-10) {
      continue;
    }

    if (!first) {
      oss << (coeff > 0 ? " + " : " - ");
      coeff = std::abs(coeff);
    } else if (coeff < 0) {
      oss << "-";
      coeff = -coeff;
    }

    if (i == 0 || std::abs(coeff - 1) > 1e-10) {
      oss << coeff;
    } else if (i > 0 && std::abs(coeff - 1) < 1e-10 && first && coeff < 0) {
      oss << "1";
    }

    if (i > 0) {
      oss << "x";
      if (i > 1) {
        oss << "^" << i;
      }
    }

    first = false;
  }

  return oss.str();
}

Polynomial Polynomial::lagrangeInterpolation(const std::vector<double>& x,
                                             const std::vector<double>& y)
{
  if (x.size() != y.size()) {
    throw std::runtime_error("x and y vectors must have the same size");
  }

  int n = x.size();
  Polynomial result(0);

  for (int i = 0; i < n; i++) {
    Polynomial li(1);

    for (int j = 0; j < n; j++) {
      if (i != j) {
        std::vector<double> linearCoeffs = {-x[j], 1};
        Polynomial linear(linearCoeffs);
        li = li * linear;
        li = li * Polynomial(1.0 / (x[i] - x[j]));
      }
    }

    result = result + (li * Polynomial(y[i]));
  }

  return result;
}

Polynomial Polynomial::newtonInterpolation(const std::vector<double>& x,
                                           const std::vector<double>& y)
{
  if (x.size() != y.size()) {
    throw std::runtime_error("x and y vectors must have the same size");
  }

  int n = x.size();
  std::vector<std::vector<double>> dividedDiffs(n, std::vector<double>(n, 0));

  for (int i = 0; i < n; i++) {
    dividedDiffs[i][0] = y[i];
  }

  for (int j = 1; j < n; j++) {
    for (int i = 0; i < n - j; i++) {
      dividedDiffs[i][j] = (dividedDiffs[i + 1][j - 1] - dividedDiffs[i][j - 1])
          / (x[i + j] - x[i]);
    }
  }

  Polynomial result(dividedDiffs[0][0]);

  for (int i = 1; i < n; i++) {
    Polynomial term(dividedDiffs[0][i]);

    for (int j = 0; j < i; j++) {
      std::vector<double> linearCoeffs = {-x[j], 1};
      term = term * Polynomial(linearCoeffs);
    }

    result = result + term;
  }

  return result;
}
