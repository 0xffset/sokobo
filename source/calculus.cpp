#include "include/calculus.h"
#include "include/expression.h"
#include "include/polynomial.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>
// #include <corecrt_math_defines.h>

constexpr double M_PI = 3.14159265358979323846;

// Symbolic differentiation
std::shared_ptr<Expression> Calculus::differentiate(
    std::shared_ptr<Expression> expr, const std::string& var)
{
  return partialDerivative(expr, var);
}

std::shared_ptr<Expression> Calculus::partialDerivative(
    std::shared_ptr<Expression> expr, const std::string& var)
{
  if (!expr) {
    return nullptr;
  }

  // Get the expression type
Expression::Type
  type = expr->getType();

  switch (type) {
  case Expression::Type::CONSTANT: {
      // Derivative of constant is 0
      return std::make_shared<Constant>(0.0);
    }

    case Expression::
      Type::
    VARIABLE: {
      auto varExpr = std::dynamic_pointer_cast<Variable>(expr);
      if (varExpr && varExpr->getName() == var) {
        // d/dx (x) = 1
        return std::make_shared<Constant>(1.0);
      } else {
        // d/dx (y) = 0 where y != x
        return std::make_shared<Constant>(0.0);
      }
    }

    case Expression::Type::BINARY_OP: {
      auto binExpr = std::dynamic_pointer_cast<BinaryOp>(expr);
      if (!binExpr) {
        break;
      }

      auto left = binExpr->getLeft();
      auto right = binExpr->getRight();
      char op = binExpr->getOperator();

      switch (op) {
        case '+':
        case '-': {
          // d/dx (f � g) = df/dx � dg/dx
          auto leftDeriv = partialDerivative(left, var);
          auto rightDeriv = partialDerivative(right, var);
          return std::make_shared<BinaryOp>(
              leftDeriv, op, rightDeriv);
        }

        case '*': {
          // Product rule: d/dx (f * g) = f' * g + f * g'
          auto leftDeriv = partialDerivative(left, var);
          auto rightDeriv = partialDerivative(right, var);

          auto term1 = std::make_shared<BinaryOp>(leftDeriv, '*', right);
          auto term2 = std::make_shared<BinaryOp>(left, '*', rightDeriv);

          return std::make_shared<BinaryOp>(term1, '+', term2);
        }

        case '/': {
          // Quotient rule: d/dx (f/g) = (f'*g - f*g') / g^2
          auto leftDeriv = partialDerivative(left, var);
          auto rightDeriv = partialDerivative(right, var);

          auto numerator1 = std::make_shared<BinaryOp>(leftDeriv, '*', right);
          auto numerator2 = std::make_shared<BinaryOp>(left, '*', rightDeriv);
          auto numerator =
              std::make_shared<BinaryOp>(numerator1, '-', numerator2);

          auto denominator = std::make_shared<BinaryOp>(right, '*', right);

          return std::make_shared<BinaryOp>(
              numerator, '/', denominator);
        }

        case '^': {
          // Power rule: d/dx (f^g) = f^g * (g'*ln(f) + g*f'/f)
          // For simple case f^n where n is constant: d/dx (f^n) = n * f^(n-1) *
          // f'

          // Check if right side is constant
          if (right->getType() == Expression::Type::CONSTANT) {
            auto constExpr =
                std::dynamic_pointer_cast<Constant>(right);
            if (constExpr) {
              double n = constExpr->getValue();
              if (n == 0) {
                return std::make_shared<Constant>(0.0);
              }

              auto leftDeriv = partialDerivative(left, var);
              auto nMinus1 = std::make_shared<Constant>(n - 1);
              auto nConst = std::make_shared<Constant>(n);

              // n * f^(n-1) * f'
              auto power = std::make_shared<BinaryOp>(left, '^', nMinus1);
              auto temp = std::make_shared<BinaryOp>(nConst, '*', power);
              return std::make_shared<BinaryOp>(temp, '*', leftDeriv);
            }
          }

          // General case would require logarithmic differentiation
          // For now, throw error for complex power rule cases
          throw std::runtime_error(
              "Complex power rule differentiation not implemented");
        }
      }
      break;
    }

    case Expression::Type::
    UNARY_OP: {
      auto unaryExpr = std::dynamic_pointer_cast<UnaryOpExpression>(expr);
      if (!unaryExpr) {
        break;
      }

      auto operand = unaryExpr->getOperand();
      char op = unaryExpr->getOperator();
     

      switch (op) {
        case '-': {
          // d/dx (-f) = -df/dx
          auto operandDeriv = partialDerivative(expr, var);
          return std::make_shared<UnaryOpExpression>('-', operandDeriv);
        }
        case '+': {
          // d/dx (+f) = df/dx
          return partialDerivative(expr, var);
        }
      }
      break;
    }

    case Expression::Type::
    FUNCTION: {
      auto funcExpr = std::dynamic_pointer_cast<Function>(expr);
      if (!funcExpr) {
        break;
      }

      std::string funcName = funcExpr->getFunctionName();
      auto arg = funcExpr->getArgument();
      auto argDeriv = partialDerivative(arg, var);

      // Chain rule: d/dx f(g(x)) = f'(g(x)) * g'(x)
      if (funcName == "sin") {
        auto cosFunc = std::make_shared<Function>("cos", arg);
        return std::make_shared<BinaryOp>(cosFunc, '*', argDeriv);
      } else if (funcName == "cos") {
        auto sinFunc = std::make_shared<Function>("sin", arg);
        auto negSin = std::make_shared<UnaryOpExpression>('-', sinFunc);
        return std::make_shared<BinaryOp>(negSin, '*', argDeriv);
      } else if (funcName == "tan") {
        auto cosFunc = std::make_shared<Function>("cos", arg);
        auto cos2 = std::make_shared<BinaryOp>(cosFunc, '*', cosFunc);
        auto one = std::make_shared<Constant>(1.0);
        auto sec2 = std::make_shared<BinaryOp>(one, '/', cos2);
        return std::make_shared<BinaryOp>(sec2, '*', argDeriv);
      } else if (funcName == "ln" || funcName == "log") {
        auto reciprocal = std::make_shared<BinaryOp>(
            std::make_shared<Constant>(1.0), '/', arg);
        return std::make_shared<BinaryOp>(reciprocal, '*', argDeriv);
      } else if (funcName == "exp") {
        auto expFunc = std::make_shared<Function>("exp", arg);
        return std::make_shared<BinaryOp>(expFunc, '*', argDeriv);
      } else if (funcName == "sqrt") {
        // d/dx sqrt(f) = 1/(2*sqrt(f)) * f'
        auto two = std::make_shared<Constant>(2.0);
        auto sqrtFunc = std::make_shared<Function>("sqrt", arg);
        auto denominator = std::make_shared<BinaryOp>(two, '*', sqrtFunc);
        auto fraction = std::make_shared<BinaryOp>(
            std::make_shared<Constant>(1.0), '/', denominator);
        return std::make_shared<BinaryOp>(fraction, '*', argDeriv);
      }

      // For unknown functions, we can't differentiate
      throw std::runtime_error("Differentiation of function '" + funcName
                               + "' not implemented");
    }

    default:
      break;
  }

  throw std::runtime_error(
      "Unable to differentiate expression of unknown type");
}

// Symbolic integration (limited cases)
std::shared_ptr<Expression> Calculus::integrate(std::shared_ptr<Expression> expr, const std::string& var) {
    // Similar to differentiation, this would need full expression analysis
    // Would implement basic integration rules:
    // - Power rule: x^n -> x^(n+1)/(n+1)
    // - Sum rule: integral of sum = sum of integrals
    // - Basic trigonometric and exponential integrals
    
    throw std::runtime_error("Symbolic integration not fully implemented - requires complete expression tree structure");
}

// Numerical integration using trapezoidal rule
double Calculus::trapezoidalRule(std::function<double(double)> f, double a, double b, int n) {
    if (n <= 0) throw std::invalid_argument("Number of intervals must be positive");
    
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }
    
    return sum * h;
}

// Numerical integration using Simpson's rule
double Calculus::simpsonsRule(std::function<double(double)> f, double a, double b, int n)  {
    if (n <= 0 || n % 2 != 0) throw std::invalid_argument("Number of intervals must be positive and even");
    
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    
    // Add odd-indexed terms (coefficient 4)
    for (int i = 1; i < n; i += 2) {
        sum += 4 * f(a + i * h);
    }
    
    // Add even-indexed terms (coefficient 2)
    for (int i = 2; i < n; i += 2) {
        sum += 2 * f(a + i * h);
    }
    
    return sum * h / 3.0;
}

// Gaussian quadrature integration
double Calculus::gaussianQuadrature(std::function<double(double)> f, double a, double b, int n) {
    // Gauss-Legendre quadrature points and weights for common values of n
    static const std::vector<std::vector<double>> points = {
        {},  // n=0 (unused)
        {0.0},  // n=1
        {-0.5773502691896257, 0.5773502691896257},  // n=2
        {-0.7745966692414834, 0.0, 0.7745966692414834},  // n=3
        {-0.8611363115940526, -0.3399810435848563, 0.3399810435848563, 0.8611363115940526},  // n=4
        {-0.9061798459386640, -0.5384693101056831, 0.0, 0.5384693101056831, 0.9061798459386640}  // n=5
    };
    
    static const std::vector<std::vector<double>> weights = {
        {},  // n=0 (unused)
        {2.0},  // n=1
        {1.0, 1.0},  // n=2
        {0.5555555555555556, 0.8888888888888888, 0.5555555555555556},  // n=3
        {0.3478548451374538, 0.6521451548625461, 0.6521451548625461, 0.3478548451374538},  // n=4
        {0.2369268850561891, 0.4786286704993665, 0.5688888888888889, 0.4786286704993665, 0.2369268850561891}  // n=5
    };
    
    if (n < 1 || n > 5) {
        throw std::invalid_argument("Gaussian quadrature only implemented for n=1 to 5");
    }
    
    double sum = 0.0;
    double mid = (a + b) / 2.0;
    double half_width = (b - a) / 2.0;
    
    for (int i = 0; i < n; i++) {
        double x = mid + half_width * points[n][i];
        sum += weights[n][i] * f(x);
    }
    
    return half_width * sum;
}

// Monte Carlo integration
double Calculus::monteCarloIntegration(std::function<double(double)> f, double a, double b, int samples) {
    if (samples <= 0) throw std::invalid_argument("Number of samples must be positive");
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(a, b);
    
    double sum = 0.0;
    for (int i = 0; i < samples; i++) {
        double x = dis(gen);
        sum += f(x);
    }
    
    return (b - a) * sum / samples;
}

// Double integral using nested trapezoidal rule
double Calculus::doubleIntegral(std::function<double(double, double)> f, 
                               double x1, double x2, double y1, double y2, int nx, int ny) {
    if (nx <= 0 || ny <= 0) throw std::invalid_argument("Number of intervals must be positive");
    
    double hx = (x2 - x1) / nx;
    double hy = (y2 - y1) / ny;
    double sum = 0.0;
    
    // Apply trapezoidal rule in both dimensions
    for (int i = 0; i <= nx; i++) {
        double x = x1 + i * hx;
        double weight_x = (i == 0 || i == nx) ? 0.5 : 1.0;
        
        for (int j = 0; j <= ny; j++) {
            double y = y1 + j * hy;
            double weight_y = (j == 0 || j == ny) ? 0.5 : 1.0;
            
            sum += weight_x * weight_y * f(x, y);
        }
    }
    
    return sum * hx * hy;
}

// Taylor series expansion
Polynomial Calculus::taylorSeries(std::function<double(double)> f, double center, int degree) {
    if (degree < 0) throw std::invalid_argument("Degree must be non-negative");
    
    std::vector<double> coefficients(degree + 1);
    double h = 1e-8;  // Small step for numerical differentiation
    
    // Calculate derivatives numerically and build Taylor series
    coefficients[0] = f(center);  // f(center)
    
    for (int n = 1; n <= degree; n++) {
        // Calculate nth derivative using finite differences
        double derivative = 0.0;
        
        // Use central difference formula for higher accuracy
        if (n == 1) {
            derivative = (f(center + h) - f(center - h)) / (2.0 * h);
        } else {
            // For higher derivatives, use recursive finite differences
            // This is a simplified approach - more sophisticated methods exist
            std::function<double(double)> fn = f;
            for (int i = 0; i < n; i++) {
                auto prev_fn = fn;
                fn = [prev_fn, h](double x) {
                    return (prev_fn(x + h) - prev_fn(x - h)) / (2.0 * h);
                };
            }
            derivative = fn(center);
        }
        
        // Calculate factorial
        double factorial = 1.0;
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }
        
        coefficients[n] = derivative / factorial;
    }
    
    return Polynomial(coefficients);
}

// Chebyshev series expansion
Polynomial Calculus::chebyshevSeries(std::function<double(double)> f, double a, double b, int degree) {
    if (degree < 0) throw std::invalid_argument("Degree must be non-negative");
    
    std::vector<double> coefficients(degree + 1, 0.0);
    
    // Transform interval [a,b] to [-1,1] for Chebyshev polynomials
    auto transform = [a, b](double t) { return 0.5 * ((b - a) * t + a + b); };
    
    // Calculate Chebyshev coefficients
    for (int k = 0; k <= degree; k++) {
        double sum = 0.0;
        int n_points = std::max(degree + 1, 50);  // Use enough points for accuracy
        
        for (int j = 0; j < n_points; j++) {
            double t = std::cos(std::acos(-1.0) * (j + 0.5) / n_points);  // Chebyshev points
            double x = transform(t);
            double T_k = 1.0;  // T_0(t) = 1
            
            if (k == 1) {
                T_k = t;  // T_1(t) = t
            } else if (k > 1) {
                // Use recurrence relation: T_{k+1}(t) = 2t*T_k(t) - T_{k-1}(t)
                double T_prev = 1.0;  // T_0
                double T_curr = t;    // T_1
                
                for (int m = 2; m <= k; m++) {
                    double T_next = 2.0 * t * T_curr - T_prev;
                    T_prev = T_curr;
                    T_curr = T_next;
                }
                T_k = T_curr;
            }
            
            sum += f(x) * T_k;
        }
        
        coefficients[k] = (k == 0 ? 1.0 : 2.0) * sum / n_points;
    }
    
    // Convert Chebyshev coefficients to standard polynomial form
    // This is a simplified conversion - full implementation would use proper basis transformation
    return Polynomial(coefficients);
}

// Numerical limit calculation
double Calculus::limit(std::function<double(double)> f, double x, double h) {
    if (h <= 0) throw std::invalid_argument("Step size must be positive");
    
    // Approach the limit from both sides
    double left_limit = f(x - h);
    double right_limit = f(x + h);
    
    // Refine the estimate by reducing h
    double prev_avg = (left_limit + right_limit) / 2.0;
    
    for (int i = 0; i < 10; i++) {  // Iterate to improve accuracy
        h *= 0.1;
        left_limit = f(x - h);
        right_limit = f(x + h);
        double current_avg = (left_limit + right_limit) / 2.0;
        
        // Check for convergence
        if (std::abs(current_avg - prev_avg) < 1e-12) {
            break;
        }
        prev_avg = current_avg;
    }
    
    return prev_avg;
}
