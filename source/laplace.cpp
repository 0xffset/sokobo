#include "include/laplace.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <numeric>

// Initialize static member
std::map<std::string, std::function<ComplexNumber(ComplexNumber)>> LaplaceTransform::transformTable;

void LaplaceTransform::initializeTransformTable() {
    if (!transformTable.empty()) return; // Already initialized
    
    // Basic transforms
    transformTable["1"] = [](ComplexNumber s) -> ComplexNumber {
        return ComplexNumber(1.0, 0.0) / s;
    };
    
    transformTable["t"] = [](ComplexNumber s) -> ComplexNumber {
        return ComplexNumber(1.0, 0.0) / (s * s);
    };
    
    transformTable["t^2"] = [](ComplexNumber s) -> ComplexNumber {
        return ComplexNumber(2.0, 0.0) / (s * s * s);
    };
    
    // Exponentials
    transformTable["exp(at)"] = [](ComplexNumber s) -> ComplexNumber {
        // This is a placeholder - actual 'a' value would need to be extracted from expression
        double a = 1.0; // Default value
        return ComplexNumber(1.0, 0.0) / (s - ComplexNumber(a, 0.0));
    };
    
    // Trigonometric functions
    transformTable["sin(wt)"] = [](ComplexNumber s) -> ComplexNumber {
        double w = 1.0; // Default value - would need to be extracted from expression
        ComplexNumber w_complex(w, 0.0);
        return w_complex / (s * s + w_complex * w_complex);
    };
    
    transformTable["cos(wt)"] = [](ComplexNumber s) -> ComplexNumber {
        double w = 1.0; // Default value - would need to be extracted from expression
        ComplexNumber w_complex(w, 0.0);
        return s / (s * s + w_complex * w_complex);
    };
    
    // Hyperbolic functions
    transformTable["sinh(at)"] = [](ComplexNumber s) -> ComplexNumber {
        double a = 1.0; // Default value
        ComplexNumber a_complex(a, 0.0);
        return a_complex / (s * s - a_complex * a_complex);
    };
    
    transformTable["cosh(at)"] = [](ComplexNumber s) -> ComplexNumber {
        double a = 1.0; // Default value
        ComplexNumber a_complex(a, 0.0);
        return s / (s * s - a_complex * a_complex);
    };
}

// Symbolic Laplace transforms
std::shared_ptr<Expression> LaplaceTransform::transform(std::shared_ptr<Expression> expr, const std::string& var) {
    initializeTransformTable();
    
    if (!expr) {
        throw std::invalid_argument("Expression cannot be null");
    }
    
    // Get string representation of the expression
    std::string exprStr = expr->toString();
    
    // Simple pattern matching for basic transforms
    // Note: This is a simplified implementation. A full implementation would need
    // sophisticated expression parsing and pattern matching
    
    if (exprStr == "1") {
        // L{1} = 1/s
      return std::make_shared<Variable>("1/s");
    }
    else if (exprStr == var) {
        // L{t} = 1/s^2
      return std::make_shared<Variable>("1/s^2");
    }
    else if (exprStr.find("exp") != std::string::npos) {
        // Handle exponential functions
        // This is simplified - would need proper parsing
      return std::make_shared<Variable>("1/(s-a)");
    }
    else if (exprStr.find("sin") != std::string::npos) {
        // Handle sine functions
      return std::make_shared<Variable>("w/(s^2+w^2)");
    }
    else if (exprStr.find("cos") != std::string::npos) {
        // Handle cosine functions
      return std::make_shared<Variable>("s/(s^2+w^2)");
    }
    
    // Default case - return a placeholder
    return std::make_shared<Variable>("L{" + exprStr + "}");
}

std::shared_ptr<Expression> LaplaceTransform::inverseTransform(std::shared_ptr<Expression> expr, const std::string& var) {
    if (!expr) {
        throw std::invalid_argument("Expression cannot be null");
    }
    
    std::string exprStr = expr->toString();
    
    // Simple pattern matching for basic inverse transforms
    if (exprStr == "1/s") {
      return std::make_shared<Variable>("1");
    }
    else if (exprStr == "1/s^2") {
      return std::make_shared<Variable>(var);
    }
    else if (exprStr.find("1/(s-") != std::string::npos) {
        // Handle 1/(s-a) -> exp(at)
      return std::make_shared<Variable>("exp(a*" + var + ")");
    }
    else if (exprStr.find("/(s^2+") != std::string::npos) {
        if (exprStr.find("s/(s^2+") != std::string::npos) {
            // s/(s^2+w^2) -> cos(wt)
          return std::make_shared<Variable>("cos(w*" + var + ")");
        } else {
            // w/(s^2+w^2) -> sin(wt)
          return std::make_shared<Variable>("sin(w*" + var + ")");
        }
    }
    
    // Default case
    return std::make_shared<Variable>("L^{-1}{" + exprStr + "}");
}

// Numerical Laplace transforms
ComplexNumber LaplaceTransform::numericalTransform(std::function<double(double)> f, ComplexNumber s, double T) {
  if (s.getValue().real() <= 0) {
        throw std::invalid_argument("Real part of s must be positive for convergence");
    }
    
    const int samples = 1000;
    double dt = T / samples;
    ComplexNumber result(0, 0);
    
    for (int n = 0; n < samples; ++n) {
        double t = n * dt;
        double ft = f(t);
        
        // Calculate e^(-st)
        ComplexNumber minus_st =
            ComplexNumber(-s.getValue().real() * t, -s.getValue().imag() * t);
        ComplexNumber exponential = minus_st.exp();
        
        result = result + ComplexNumber(ft, 0) * exponential * dt;
    }
    
    return result;
}

// Helper function for factorial
double factorial(int n)
{
  if (n <= 1) {
    return 1.0;
  }
  double result = 1.0;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

std::function<double(double)> LaplaceTransform::numericalInverseTransform(
    std::function<ComplexNumber(ComplexNumber)> F, double t_max) {
    
    return [F, t_max](double t) -> double {
        if (t < 0) return 0.0; // Laplace transform is causal
        
        // Use Bromwich integral approximation
        // This is a simplified implementation using the Gaver-Stehfest algorithm
        const int N = 10; // Number of terms in the approximation
        
        double result = 0.0;
        double ln2_t = log(2.0) / t;
        
        // Gaver-Stehfest coefficients
        std::vector<double> V(N + 1);
        for (int i = 1; i <= N; ++i) {
            double sum = 0.0;
            int k_min = (i + 1) / 2;
            int k_max = std::min(i, N / 2);
            
            for (int k = k_min; k <= k_max; ++k) {
                double term = pow(k, N / 2) * factorial(2 * k);
                term /= (factorial(N / 2 - k) * factorial(k) * factorial(k - 1) * factorial(i - k) * factorial(2 * k - i));
                sum += term;
            }
            
            V[i] = pow(-1, i + N / 2) * sum;
        }
        
        // Compute the approximation
        for (int i = 1; i <= N; ++i) {
            ComplexNumber s(i * ln2_t, 0.0);
            ComplexNumber F_s = F(s);
            result += V[i] * F_s.getValue().real();
        }
        
        return ln2_t * result;
    };
}



// Common transforms
ComplexNumber LaplaceTransform::exponentialTransform(double a, ComplexNumber s) {
    // L{e^(at)} = 1/(s-a)
    ComplexNumber a_complex(a, 0.0);
    return ComplexNumber(1.0, 0.0) / (s - a_complex);
}

ComplexNumber LaplaceTransform::sinusoidalTransform(double omega, ComplexNumber s) {
    // L{sin(ωt)} = ω/(s²+ω²)
    ComplexNumber omega_complex(omega, 0.0);
    return omega_complex / (s * s + omega_complex * omega_complex);
}

ComplexNumber LaplaceTransform::polynomialTransform(const Polynomial& p, ComplexNumber s) {
    // L{t^n} = n!/s^(n+1)
    // For a polynomial, we sum the transforms of individual terms
    
    ComplexNumber result(0, 0);
    
    // Assuming Polynomial has getDegree() and getCoefficient(int) methods
    for (int i = 0; i <= p.getDegree(); ++i) {
        double coeff = p.getCoeff(i);
        if (std::abs(coeff) < 1e-15) continue; // Skip zero coefficients
        
        // Calculate i! / s^(i+1)
        double factorial_i = factorial(i);
        ComplexNumber s_power = s;
        for (int j = 1; j < i + 1; ++j) {
            s_power = s_power * s;
        }
        
        ComplexNumber term = ComplexNumber(coeff * factorial_i, 0.0) / s_power;
        result = result + term;
    }
    
    return result;
}

// Properties
std::shared_ptr<Expression> LaplaceTransform::convolution(std::shared_ptr<Expression> f1, std::shared_ptr<Expression> f2) {
    if (!f1 || !f2) {
        throw std::invalid_argument("Expressions cannot be null");
    }
    
    // L{f1 * f2} = F1(s) * F2(s) where * denotes convolution
    auto F1 = transform(f1);
    auto F2 = transform(f2);
    
    // Create expression representing the product
    std::string result_str = "(" + F1->toString() + ") * (" + F2->toString() + ")";
    return std::make_shared<Variable>(result_str);
}

std::shared_ptr<Expression> LaplaceTransform::timeShift(std::shared_ptr<Expression> expr, double a) {
    if (!expr) {
        throw std::invalid_argument("Expression cannot be null");
    }
    
    if (a < 0) {
        throw std::invalid_argument("Time shift must be non-negative");
    }
    
    // L{f(t-a)u(t-a)} = e^(-as) * F(s) where u is the unit step function
    auto F = transform(expr);
    std::string result_str = "exp(-" + std::to_string(a) + "*s) * (" + F->toString() + ")";
    return std::make_shared<Variable>(result_str);
}

std::shared_ptr<Expression> LaplaceTransform::frequencyShift(std::shared_ptr<Expression> expr, double a) {
    if (!expr) {
        throw std::invalid_argument("Expression cannot be null");
    }
    
    // L{e^(at) * f(t)} = F(s-a)
    auto F = transform(expr);
    std::string original = F->toString();
    
    // Replace 's' with '(s-a)' in the transformed expression
    // This is a simplified implementation
    std::string result_str = original;
    size_t pos = 0;
    while ((pos = result_str.find('s', pos)) != std::string::npos) {
        result_str.replace(pos, 1, "(s-" + std::to_string(a) + ")");
        pos += 4 + std::to_string(a).length();
    }
    
    return std::make_shared<Variable>(result_str);
}