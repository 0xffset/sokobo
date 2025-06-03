#pragma once
#include <vector>
#include <functional>
#include "matrix.h"
#include "polynomial.h"

class NumericalMethods {
public:
    // Root finding
    static double bisectionMethod(std::function<double(double)> f, double a, double b, double tolerance = 1e-10);
    static double newtonRaphson(std::function<double(double)> f, std::function<double(double)> df, 
                               double x0, double tolerance = 1e-10);
    static double secantMethod(std::function<double(double)> f,
                               double x0,
                               double x1,
                               double tolerance = 1e-10);
    static std::vector<double> polynomialRoots(const Polynomial& p);
    
    // Linear systems
    static std::vector<double> gaussianElimination(Matrix<double> A, std::vector<double> b);
    static std::vector<double> LUDecomposition(const Matrix<double>& A, const std::vector<double>& b);
    static std::vector<double> jacobiMethod(const Matrix<double>& A, const std::vector<double>& b, 
                                           double tolerance = 1e-10, int maxIterations = 1000);
    static std::vector<double> gaussSeidelMethod(const Matrix<double>& A, const std::vector<double>& b, 
                                                 double tolerance = 1e-10, int maxIterations = 1000);
    
    // Interpolation
    static Polynomial lagrangeInterpolation(const std::vector<double>& x, const std::vector<double>& y);
    static Polynomial newtonInterpolation(const std::vector<double>& x, const std::vector<double>& y);
    static double splineInterpolation(const std::vector<double>& x, const std::vector<double>& y, double xi);
    
    // Optimization
    static double goldenSectionSearch(std::function<double(double)> f, double a, double b, double tolerance = 1e-10);
    static std::vector<double> gradientDescent(std::function<double(const std::vector<double>&)> f,
                                              std::function<std::vector<double>(const std::vector<double>&)> grad,
                                              std::vector<double> x0, double learningRate = 0.01, 
                                              double tolerance = 1e-10);
    static std::vector<double> newtonOptimization(std::function<double(const std::vector<double>&)> f,
                                                  std::function<std::vector<double>(const std::vector<double>&)> grad,
                                                  std::function<Matrix<double>(const std::vector<double>&)> hessian,
                                                  std::vector<double> x0, double tolerance = 1e-10);
    
    // Statistical methods
    static double mean(const std::vector<double>& data);
    static double variance(const std::vector<double>& data);
    static double standardDeviation(const std::vector<double>& data);
    static double correlation(const std::vector<double>& x, const std::vector<double>& y);
    static std::pair<double, double> linearRegression(const std::vector<double>& x, const std::vector<double>& y);
    
    // Special functions
    static double gamma(double x);
    static double beta(double a, double b);
    static double erf(double x);
    static double besselJ(int n, double x);
    static double legendreP(int n, double x);
    static double hermiteH(int n, double x);
};
