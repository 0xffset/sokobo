#pragma once
#include "expression.h"
#include <vector>
#include <functional>

class DifferentialEquations {
public:
    // Ordinary Differential Equations
    
    // First-order ODEs
    static std::vector<double> eulerMethod(std::function<double(double, double)> f, 
                                          double x0, double y0, double h, int steps);
    static std::vector<double> rungeKutta4(std::function<double(double, double)> f, 
                                          double x0, double y0, double h, int steps);
    static std::vector<double> adamsBashforth(std::function<double(double, double)> f, 
                                             double x0, double y0, double h, int steps);
    
    // Higher-order ODEs (converted to system of first-order)
    static std::vector<std::vector<double>> systemRungeKutta4(
        std::vector<std::function<double(double, const std::vector<double>&)>> f,
        double x0, const std::vector<double>& y0, double h, int steps);
    
    // Boundary value problems
    static std::vector<double> shootingMethod(
        std::function<double(double, double, double)> f,
        double a, double b, double alpha, double beta, int n = 100);
    
    // Partial Differential Equations
    
    // Heat equation (1D)
    static std::vector<std::vector<double>> heatEquation1D(
        double alpha, double L, double T, int nx, int nt,
        std::function<double(double)> initialCondition,
        std::function<double(double)> boundaryLeft,
        std::function<double(double)> boundaryRight);
    
    // Wave equation (1D)
    static std::vector<std::vector<double>> waveEquation1D(
        double c, double L, double T, int nx, int nt,
        std::function<double(double)> initialPosition,
        std::function<double(double)> initialVelocity);
    
    // Laplace equation (2D) - using finite differences
    static std::vector<std::vector<double>> laplaceEquation2D(
        int nx, int ny, double tolerance = 1e-6,
        std::function<double(double, double)> boundaryCondition = nullptr);
    
    // Symbolic solutions for special cases
    static std::shared_ptr<Expression> solveLinearODE(const std::vector<double>& coeffs, 
                                                     std::shared_ptr<Expression> rhs);
    static std::shared_ptr<Expression> solveSeparableODE(std::shared_ptr<Expression> expr);
};