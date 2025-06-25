#include "include/expression.h"
#include "include/polynomial.h"
#include <functional>
#include <vector>

class Calculus {
public:
    // Symbolic differentiation
    static std::shared_ptr<Expression> differentiate(std::shared_ptr<Expression> expr, const std::string& var);
    static std::shared_ptr<Expression> partialDerivative(std::shared_ptr<Expression> expr, const std::string& var);
    
    // Symbolic integration (limited cases)
    static std::shared_ptr<Expression> integrate(std::shared_ptr<Expression> expr, const std::string& var);
    
    // Numerical integration
    static double trapezoidalRule(std::function<double(double)> f, double a, double b, int n = 1000);
    static double simpsonsRule(std::function<double(double)> f, double a, double b, int n = 1000);
    static double gaussianQuadrature(std::function<double(double)> f, double a, double b, int n = 10);
    static double monteCarloIntegration(std::function<double(double)> f, double a, double b, int samples = 100000);
    
    // Multiple integrals
    static double doubleIntegral(std::function<double(double, double)> f, 
                                double x1, double x2, double y1, double y2, int nx = 100, int ny = 100);
    
    // Series expansions
    static Polynomial taylorSeries(std::function<double(double)> f, double center, int degree);
    static Polynomial chebyshevSeries(std::function<double(double)> f, double a, double b, int degree);
    
    // Limits
    static double limit(std::function<double(double)> f, double x, double h = 1e-10);
};