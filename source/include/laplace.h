#pragma once
#include "expression.h"
#include "complex_number.h"
#include <map>
#include <functional>
#include "polynomial.h"

class LaplaceTransform {
private:
    static std::map<std::string, std::function<ComplexNumber(ComplexNumber)>> transformTable;
    static void initializeTransformTable();
    
public:
    // Symbolic Laplace transforms
    static std::shared_ptr<Expression> transform(std::shared_ptr<Expression> expr, const std::string& var = "t");
    static std::shared_ptr<Expression> inverseTransform(std::shared_ptr<Expression> expr, const std::string& var = "s");
    
    // Numerical Laplace transforms
    static ComplexNumber numericalTransform(std::function<double(double)> f, ComplexNumber s, double T = 10.0);
    static std::function<double(double)> numericalInverseTransform(std::function<ComplexNumber(ComplexNumber)> F, 
                                                                  double t_max = 10.0);
    
    // Common transforms
    static ComplexNumber exponentialTransform(double a, ComplexNumber s);
    static ComplexNumber sinusoidalTransform(double omega, ComplexNumber s);
    static ComplexNumber polynomialTransform(const Polynomial& p, ComplexNumber s);
    
    // Properties
    static std::shared_ptr<Expression> convolution(std::shared_ptr<Expression> f1, std::shared_ptr<Expression> f2);
    static std::shared_ptr<Expression> timeShift(std::shared_ptr<Expression> expr, double a);
    static std::shared_ptr<Expression> frequencyShift(std::shared_ptr<Expression> expr, double a);
};