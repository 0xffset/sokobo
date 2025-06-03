#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>

#include "include/numerical_methods.h"
#include "include/numerical_methods.h"
#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#  define M_E 2.71828
#endif


// Root Finding Methods

double NumericalMethods::bisectionMethod(std::function<double(double)> f,
                                         double a,
                                         double b,
                                         double tolerance)
{
  if (f(a) * f(b) >= 0) {
    throw std::invalid_argument(
        "Function must have opposite signs at endpoints for bisection method");
  }

  const int maxIterations = 1000;
  int iterations = 0;

  while (std::abs(b - a) > tolerance && iterations < maxIterations) {
    double c = (a + b) / 2.0;
    double fc = f(c);

    if (std::abs(fc) < tolerance) {
      return c;
    }

    if (f(a) * fc < 0) {
      b = c;
    } else {
      a = c;
    }

    iterations++;
  }

  if (iterations >= maxIterations) {
    throw std::runtime_error("Bisection method failed to converge");
  }

  return (a + b) / 2.0;
}

double NumericalMethods::newtonRaphson(std::function<double(double)> f,
                                       std::function<double(double)> df,
                                       double x0,
                                       double tolerance)
{
  const int maxIterations = 1000;
  double x = x0;

  for (int i = 0; i < maxIterations; ++i) {
    double fx = f(x);
    double dfx = df(x);

    if (std::abs(fx) < tolerance) {
      return x;
    }

    if (std::abs(dfx) < 1e-15) {
      throw std::runtime_error("Newton-Raphson method: derivative is zero");
    }

    double x_new = x - fx / dfx;

    if (std::abs(x_new - x) < tolerance) {
      return x_new;
    }

    x = x_new;
  }

  throw std::runtime_error("Newton-Raphson method failed to converge");
}

double NumericalMethods::secantMethod(std::function<double(double)> f,
                                      double x0,
                                      double x1,
                                      double tolerance)
{
  const int maxIterations = 1000;

  for (int i = 0; i < maxIterations; ++i) {
    double fx0 = f(x0);
    double fx1 = f(x1);

    if (std::abs(fx1) < tolerance) {
      return x1;
    }

    if (std::abs(fx1 - fx0) < 1e-15) {
      throw std::runtime_error("Secant method: function values are too close");
    }

    double x2 = x1 - fx1 * (x1 - x0) / (fx1 - fx0);

    if (std::abs(x2 - x1) < tolerance) {
      return x2;
    }

    x0 = x1;
    x1 = x2;
  }

  throw std::runtime_error("Secant method failed to converge");
}

std::vector<double> NumericalMethods::polynomialRoots(const Polynomial& p)
{
  std::vector<double> roots;
  int degree = p.degree();

  if (degree <= 0) {
    return roots;
  }

  if (degree == 1) {
    // Linear: ax + b = 0 => x = -b/a
    double a = p.getCoeff(1);
    double b = p.getCoeff(0);
    if (std::abs(a) > 1e-15) {
      roots.push_back(-b / a);
    }
    return roots;
  }

  if (degree == 2) {
    // Quadratic: ax² + bx + c = 0
    double a = p.getCoeff(2);
    double b = p.getCoeff(1);
    double c = p.getCoeff(0);

    if (std::abs(a) < 1e-15) {
      return polynomialRoots(Polynomial({c, b}));
    }

    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
      double sqrtDisc = std::sqrt(discriminant);
      roots.push_back((-b + sqrtDisc) / (2 * a));
      roots.push_back((-b - sqrtDisc) / (2 * a));
    }
    return roots;
  }

  // For higher degree polynomials, use numerical methods
  // Find initial bounds
  double maxCoeff = 0;
  for (int i = 0; i <= degree; ++i) {
    maxCoeff = std::max(maxCoeff, std::abs(p.getCoeff(i)));
  }
  double bound = 1 + maxCoeff / std::abs(p.getCoeff(degree));

  // Use multiple starting points to find roots
  const int numTrials = 20;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-bound, bound);

  auto f = [&p](double x) { return p.evaluate(x); };
  auto df = [&p](double x) { return p.derivative().evaluate(x); };

  for (int trial = 0; trial < numTrials; ++trial) {
    double x0 = dis(gen);
    try {
      double root = NumericalMethods::newtonRaphson(f, df, x0, 1e-10);

      // Check if this is a new root
      bool isNew = true;
      for (double existingRoot : roots) {
        if (std::abs(root - existingRoot) < 1e-8) {
          isNew = false;
          break;
        }
      }

      if (isNew && std::abs(f(root)) < 1e-8) {
        roots.push_back(root);
      }
    } catch (...) {
      // Try different starting point
      continue;
    }
  }

  std::sort(roots.begin(), roots.end());
  return roots;
}

// Linear System Solvers

std::vector<double> NumericalMethods::gaussianElimination(Matrix<double> A,
                                                          std::vector<double> b)
{
  int n = A.getRows();
  if (n != A.getCols() || n != b.size()) {
    throw std::invalid_argument(
        "Matrix dimensions incompatible for Gaussian elimination");
  }

  // Forward elimination with partial pivoting
  for (int i = 0; i < n - 1; ++i) {
    // Find pivot
    int maxRow = i;
    for (int k = i + 1; k < n; ++k) {
      if (std::abs(A(k, i)) > std::abs(A(maxRow, i))) {
        maxRow = k;
      }
    }

    // Swap rows
    if (maxRow != i) {
      for (int j = 0; j < n; ++j) {
        std::swap(A(i, j), A(maxRow, j));
      }
      std::swap(b[i], b[maxRow]);
    }

    // Check for singular matrix
    if (std::abs(A(i, i)) < 1e-12) {
      throw std::runtime_error("Matrix is singular");
    }

    // Eliminate
    for (int k = i + 1; k < n; ++k) {
      double factor = A(k, i) / A(i, i);
      for (int j = i; j < n; ++j) {
        A(k, j) -= factor * A(i, j);
      }
      b[k] -= factor * b[i];
    }
  }

  // Back substitution
  std::vector<double> x(n);
  for (int i = n - 1; i >= 0; --i) {
    x[i] = b[i];
    for (int j = i + 1; j < n; ++j) {
      x[i] -= A(i, j) * x[j];
    }
    x[i] /= A(i, i);
  }

  return x;
}

std::vector<double> NumericalMethods::LUDecomposition(
    const Matrix<double>& A, const std::vector<double>& b)
{
  auto [L, U] = A.LUDecomposition();
  int n = A.getRows();

  // Forward substitution: Ly = b
  std::vector<double> y(n);
  for (int i = 0; i < n; ++i) {
    y[i] = b[i];
    for (int j = 0; j < i; ++j) {
      y[i] -= L(i, j) * y[j];
    }
  }

  // Back substitution: Ux = y
  std::vector<double> x(n);
  for (int i = n - 1; i >= 0; --i) {
    x[i] = y[i];
    for (int j = i + 1; j < n; ++j) {
      x[i] -= U(i, j) * x[j];
    }
    x[i] /= U(i, i);
  }

  return x;
}

std::vector<double> NumericalMethods::jacobiMethod(const Matrix<double>& A,
                                                   const std::vector<double>& b,
                                                   double tolerance,
                                                   int maxIterations)
{
  int n = A.getRows();
  std::vector<double> x(n, 0.0);
  std::vector<double> x_new(n);

  for (int iter = 0; iter < maxIterations; ++iter) {
    for (int i = 0; i < n; ++i) {
      x_new[i] = b[i];
      for (int j = 0; j < n; ++j) {
        if (i != j) {
          x_new[i] -= A(i, j) * x[j];
        }
      }
      x_new[i] /= A(i, i);
    }

    // Check convergence
    double maxDiff = 0.0;
    for (int i = 0; i < n; ++i) {
      maxDiff = std::max(maxDiff, std::abs(x_new[i] - x[i]));
    }

    x = x_new;

    if (maxDiff < tolerance) {
      return x;
    }
  }

  throw std::runtime_error("Jacobi method failed to converge");
}

std::vector<double> NumericalMethods::gaussSeidelMethod(
    const Matrix<double>& A,
    const std::vector<double>& b,
    double tolerance,
    int maxIterations)
{
  int n = A.getRows();
  std::vector<double> x(n, 0.0);

  for (int iter = 0; iter < maxIterations; ++iter) {
    std::vector<double> x_old = x;

    for (int i = 0; i < n; ++i) {
      x[i] = b[i];
      for (int j = 0; j < n; ++j) {
        if (i != j) {
          x[i] -= A(i, j) * x[j];
        }
      }
      x[i] /= A(i, i);
    }

    // Check convergence
    double maxDiff = 0.0;
    for (int i = 0; i < n; ++i) {
      maxDiff = std::max(maxDiff, std::abs(x[i] - x_old[i]));
    }

    if (maxDiff < tolerance) {
      return x;
    }
  }

  throw std::runtime_error("Gauss-Seidel method failed to converge");
}

// Interpolation Methods

Polynomial NumericalMethods::lagrangeInterpolation(const std::vector<double>& x,
                                                   const std::vector<double>& y)
{
  if (x.size() != y.size() || x.empty()) {
    throw std::invalid_argument(
        "x and y vectors must have the same non-zero size");
  }

  int n = x.size();
  Polynomial result(0.0);

  for (int i = 0; i < n; ++i) {
    Polynomial Li(1.0);

    for (int j = 0; j < n; ++j) {
      if (i != j) {
        // Li *= (X - x[j]) / (x[i] - x[j])
        Polynomial factor({-x[j], 1.0});  // X - x[j]
        Li = Li * factor;
        Li = Li * (1.0 / (x[i] - x[j]));
      }
    }

    result = result + Li * y[i];
  }

  return result;
}

Polynomial NumericalMethods::newtonInterpolation(const std::vector<double>& x,
                                                 const std::vector<double>& y)
{
  if (x.size() != y.size() || x.empty()) {
    throw std::invalid_argument(
        "x and y vectors must have the same non-zero size");
  }

  int n = x.size();

  // Compute divided differences
  std::vector<std::vector<double>> dividedDiff(n, std::vector<double>(n));

  // Initialize first column
  for (int i = 0; i < n; ++i) {
    dividedDiff[i][0] = y[i];
  }

  // Compute higher order differences
  for (int j = 1; j < n; ++j) {
    for (int i = 0; i < n - j; ++i) {
      dividedDiff[i][j] = (dividedDiff[i + 1][j - 1] - dividedDiff[i][j - 1])
          / (x[i + j] - x[i]);
    }
  }

  // Build polynomial
  Polynomial result(dividedDiff[0][0]);
  Polynomial product(1.0);

  for (int i = 1; i < n; ++i) {
    Polynomial factor({-x[i - 1], 1.0});  // (X - x[i-1])
    product = product * factor;
    result = result + product * dividedDiff[0][i];
  }

  return result;
}

double NumericalMethods::splineInterpolation(const std::vector<double>& x,
                                             const std::vector<double>& y,
                                             double xi)
{
  if (x.size() != y.size() || x.size() < 2) {
    throw std::invalid_argument(
        "Need at least 2 points for spline interpolation");
  }

  int n = x.size();

  // Find interval containing xi
  int i = 0;
  for (i = 0; i < n - 1; ++i) {
    if (xi >= x[i] && xi <= x[i + 1]) {
      break;
    }
  }

  if (i == n - 1) {
    i = n - 2;  // Use last interval if xi is beyond range
  }

  // Linear interpolation within interval
  double t = (xi - x[i]) / (x[i + 1] - x[i]);
  return y[i] * (1 - t) + y[i + 1] * t;
}

// Optimization Methods

double NumericalMethods::goldenSectionSearch(std::function<double(double)> f,
                                             double a,
                                             double b,
                                             double tolerance)
{
  const double phi = (1 + std::sqrt(5)) / 2;  // Golden ratio
  const double resphi = 2 - phi;

  double x1 = a + resphi * (b - a);
  double x2 = a + (1 - resphi) * (b - a);
  double f1 = f(x1);
  double f2 = f(x2);

  while (std::abs(b - a) > tolerance) {
    if (f1 < f2) {
      b = x2;
      x2 = x1;
      f2 = f1;
      x1 = a + resphi * (b - a);
      f1 = f(x1);
    } else {
      a = x1;
      x1 = x2;
      f1 = f2;
      x2 = a + (1 - resphi) * (b - a);
      f2 = f(x2);
    }
  }

  return (a + b) / 2;
}

std::vector<double> NumericalMethods::gradientDescent(
    std::function<double(const std::vector<double>&)> f,
    std::function<std::vector<double>(const std::vector<double>&)> grad,
    std::vector<double> x0,
    double learningRate,
    double tolerance)
{
  const int maxIterations = 10000;
  std::vector<double> x = x0;

  for (int iter = 0; iter < maxIterations; ++iter) {
    std::vector<double> g = grad(x);

    // Check if gradient norm is small enough
    double gradNorm = 0.0;
    for (double gi : g) {
      gradNorm += gi * gi;
    }
    gradNorm = std::sqrt(gradNorm);

    if (gradNorm < tolerance) {
      return x;
    }

    // Update x
    for (size_t i = 0; i < x.size(); ++i) {
      x[i] -= learningRate * g[i];
    }
  }

  throw std::runtime_error("Gradient descent failed to converge");
}

std::vector<double> NumericalMethods::newtonOptimization(
    std::function<double(const std::vector<double>&)> f,
    std::function<std::vector<double>(const std::vector<double>&)> grad,
    std::function<Matrix<double>(const std::vector<double>&)> hessian,
    std::vector<double> x0,
    double tolerance)
{
  const int maxIterations = 1000;
  std::vector<double> x = x0;

  for (int iter = 0; iter < maxIterations; ++iter) {
    std::vector<double> g = grad(x);
    Matrix<double> H = hessian(x);

    // Check convergence
    double gradNorm = 0.0;
    for (double gi : g) {
      gradNorm += gi * gi;
    }
    gradNorm = std::sqrt(gradNorm);

    if (gradNorm < tolerance) {
      return x;
    }

    // Solve H * dx = -g
    std::vector<double> negGrad(g.size());
    for (size_t i = 0; i < g.size(); ++i) {
      negGrad[i] = -g[i];
    }

    try {
      std::vector<double> dx = gaussianElimination(H, negGrad);

      // Update x
      for (size_t i = 0; i < x.size(); ++i) {
        x[i] += dx[i];
      }
    } catch (...) {
      // Fallback to gradient descent step
      for (size_t i = 0; i < x.size(); ++i) {
        x[i] -= 0.01 * g[i];
      }
    }
  }

  throw std::runtime_error("Newton optimization failed to converge");
}

// Statistical Methods

double NumericalMethods::mean(const std::vector<double>& data)
{
  if (data.empty()) {
    throw std::invalid_argument("Cannot compute mean of empty dataset");
  }

  return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double NumericalMethods::variance(const std::vector<double>& data)
{
  if (data.size() < 2) {
    throw std::invalid_argument("Need at least 2 data points for variance");
  }

  double mu = mean(data);
  double sum = 0.0;

  for (double x : data) {
    sum += (x - mu) * (x - mu);
  }

  return sum / (data.size() - 1);
}

double NumericalMethods::standardDeviation(const std::vector<double>& data)
{
  return std::sqrt(variance(data));
}

double NumericalMethods::correlation(const std::vector<double>& x,
                                     const std::vector<double>& y)
{
  if (x.size() != y.size() || x.size() < 2) {
    throw std::invalid_argument("x and y must have the same size (at least 2)");
  }

  double mx = mean(x);
  double my = mean(y);

  double numerator = 0.0;
  double sumXX = 0.0;
  double sumYY = 0.0;

  for (size_t i = 0; i < x.size(); ++i) {
    double dx = x[i] - mx;
    double dy = y[i] - my;
    numerator += dx * dy;
    sumXX += dx * dx;
    sumYY += dy * dy;
  }

  double denominator = std::sqrt(sumXX * sumYY);
  if (denominator < 1e-15) {
    throw std::runtime_error("Cannot compute correlation: zero variance");
  }

  return numerator / denominator;
}

std::pair<double, double> NumericalMethods::linearRegression(
    const std::vector<double>& x, const std::vector<double>& y)
{
  if (x.size() != y.size() || x.size() < 2) {
    throw std::invalid_argument("x and y must have the same size (at least 2)");
  }

  double mx = mean(x);
  double my = mean(y);

  double numerator = 0.0;
  double denominator = 0.0;

  for (size_t i = 0; i < x.size(); ++i) {
    double dx = x[i] - mx;
    numerator += dx * (y[i] - my);
    denominator += dx * dx;
  }

  if (std::abs(denominator) < 1e-15) {
    throw std::runtime_error(
        "Cannot perform linear regression: zero variance in x");
  }

  double slope = numerator / denominator;
  double intercept = my - slope * mx;

  return {slope, intercept};
}

// Special Functions

double NumericalMethods::gamma(double x)
{
  if (x <= 0) {
    throw std::invalid_argument(
        "Gamma function undefined for non-positive values");
  }

  // Stirling's approximation for large x
  if (x > 10) {
    return std::sqrt(2 * M_PI / x) * std::pow(x / M_E, x);
  }

  // Use recurrence relation: Γ(x+1) = x * Γ(x)
  if (x > 1) {
    return (x - 1) * gamma(x - 1);
  }

  // For 0 < x <= 1, use Lanczos approximation (simplified)
  const double g = 7;
  const std::vector<double> c = {0.99999999999980993,
                                 676.5203681218851,
                                 -1259.1392167224028,
                                 771.32342877765313,
                                 -176.61502916214059,
                                 12.507343278686905,
                                 -0.13857109526572012,
                                 9.9843695780195716e-6,
                                 1.5056327351493116e-7};

  x -= 1;
  double a = c[0];
  for (size_t i = 1; i < c.size(); ++i) {
    a += c[i] / (x + i);
  }

  double t = x + g + 0.5;
  return std::sqrt(2 * M_PI) * std::pow(t, x + 0.5) * std::exp(-t) * a;
}

double NumericalMethods::beta(double a, double b)
{
  return gamma(a) * gamma(b) / gamma(a + b);
}

double NumericalMethods::erf(double x)
{
  // Abramowitz and Stegun approximation
  const double a1 = 0.254829592;
  const double a2 = -0.284496736;
  const double a3 = 1.421413741;
  const double a4 = -1.453152027;
  const double a5 = 1.061405429;
  const double p = 0.3275911;

  int sign = (x >= 0) ? 1 : -1;
  x = std::abs(x);

  double t = 1.0 / (1.0 + p * x);
  double y = 1.0
      - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);

  return sign * y;
}

double NumericalMethods::besselJ(int n, double x)
{
  if (n < 0) {
    return ((n % 2) == 0) ? besselJ(-n, x) : -besselJ(-n, x);
  }

  if (x == 0) {
    return (n == 0) ? 1.0 : 0.0;
  }

  // For small x, use series expansion
  if (std::abs(x) < 10) {
    double result = 0.0;
    double term = std::pow(x / 2, n) / gamma(n + 1);

    for (int k = 0; k < 100; ++k) {
      result += term;
      term *= -x * x / (4 * (k + 1) * (k + n + 1));
      if (std::abs(term) < 1e-15) {
        break;
      }
    }

    return result;
  }

  // For large x, use asymptotic expansion
  double phase = x - (n + 0.5) * M_PI / 2;
  return std::sqrt(2 / (M_PI * x)) * std::cos(phase);
}

double NumericalMethods::legendreP(int n, double x)
{
  if (n < 0 || std::abs(x) > 1) {
    throw std::invalid_argument("Invalid arguments for Legendre polynomial");
  }

  if (n == 0) {
    return 1.0;
  }
  if (n == 1) {
    return x;
  }

  // Use recurrence relation: (n+1)P_{n+1}(x) = (2n+1)xP_n(x) - nP_{n-1}(x)
  double P0 = 1.0;
  double P1 = x;

  for (int k = 2; k <= n; ++k) {
    double P2 = ((2 * k - 1) * x * P1 - (k - 1) * P0) / k;
    P0 = P1;
    P1 = P2;
  }

  return P1;
}

double NumericalMethods::hermiteH(int n, double x)
{
  if (n < 0) {
    throw std::invalid_argument(
        "Hermite polynomial order must be non-negative");
  }

  if (n == 0) {
    return 1.0;
  }
  if (n == 1) {
    return 2 * x;
  }

  // Use recurrence relation: H_{n+1}(x) = 2xH_n(x) - 2nH_{n-1}(x)
  double H0 = 1.0;
  double H1 = 2 * x;

  for (int k = 2; k <= n; ++k) {
    double H2 = 2 * x * H1 - 2 * (k - 1) * H0;
    H0 = H1;
    H1 = H2;
  }

  return H1;
}
