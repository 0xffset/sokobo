#include "include/differential_equations.h"
#include "include/expression.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

// First-order ODE: Euler's method
std::vector<double> DifferentialEquations::eulerMethod(std::function<double(double, double)> f, 
                                                      double x0, double y0, double h, int steps) {
    if (steps <= 0) throw std::invalid_argument("Number of steps must be positive");
    
    std::vector<double> solution;
    solution.reserve(steps + 1);
    solution.push_back(y0);
    
    double x = x0;
    double y = y0;
    
    for (int i = 0; i < steps; i++) {
        y = y + h * f(x, y);
        x = x0 + (i + 1) * h;
        solution.push_back(y);
    }
    
    return solution;
}

// First-order ODE: 4th-order Runge-Kutta method
std::vector<double> DifferentialEquations::rungeKutta4(std::function<double(double, double)> f, 
                                                      double x0, double y0, double h, int steps) {
    if (steps <= 0) throw std::invalid_argument("Number of steps must be positive");
    
    std::vector<double> solution;
    solution.reserve(steps + 1);
    solution.push_back(y0);
    
    double x = x0;
    double y = y0;
    
    for (int i = 0; i < steps; i++) {
        double k1 = h * f(x, y);
        double k2 = h * f(x + h/2, y + k1/2);
        double k3 = h * f(x + h/2, y + k2/2);
        double k4 = h * f(x + h, y + k3);
        
        y = y + (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        x = x0 + (i + 1) * h;
        solution.push_back(y);
    }
    
    return solution;
}

// Adams-Bashforth method (4th order)
std::vector<double> DifferentialEquations::adamsBashforth(std::function<double(double, double)> f, 
                                                         double x0, double y0, double h, int steps) {
    if (steps <= 0) throw std::invalid_argument("Number of steps must be positive");
    
    std::vector<double> solution;
    solution.reserve(steps + 1);
    solution.push_back(y0);
    
    // Use Runge-Kutta to get first few points
    std::vector<double> f_values;
    double x = x0;
    double y = y0;
    
    f_values.push_back(f(x, y));
    
    // Get first 3 points using RK4
    for (int i = 0; i < std::min(3, steps); i++) {
        double k1 = h * f(x, y);
        double k2 = h * f(x + h/2, y + k1/2);
        double k3 = h * f(x + h/2, y + k2/2);
        double k4 = h * f(x + h, y + k3);
        
        y = y + (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        x = x0 + (i + 1) * h;
        solution.push_back(y);
        f_values.push_back(f(x, y));
    }
    
    // Continue with Adams-Bashforth
    for (int i = 3; i < steps; i++) {
        // 4th order Adams-Bashforth formula
        double y_new = y + h * (55.0*f_values[i] - 59.0*f_values[i-1] + 
                                37.0*f_values[i-2] - 9.0*f_values[i-3]) / 24.0;
        
        x = x0 + (i + 1) * h;
        y = y_new;
        solution.push_back(y);
        f_values.push_back(f(x, y));
    }
    
    return solution;
}

// System of ODEs using Runge-Kutta 4th order
std::vector<std::vector<double>> DifferentialEquations::systemRungeKutta4(
    std::vector<std::function<double(double, const std::vector<double>&)>> f,
    double x0, const std::vector<double>& y0, double h, int steps) {
    
    if (steps <= 0) throw std::invalid_argument("Number of steps must be positive");
    if (f.size() != y0.size()) throw std::invalid_argument("Number of functions must match initial conditions");
    
    int n = f.size();
    std::vector<std::vector<double>> solution(n);
    
    for (int i = 0; i < n; i++) {
        solution[i].reserve(steps + 1);
        solution[i].push_back(y0[i]);
    }
    
    double x = x0;
    std::vector<double> y = y0;
    
    for (int step = 0; step < steps; step++) {
        std::vector<double> k1(n), k2(n), k3(n), k4(n);
        std::vector<double> temp(n);
        
        // Calculate k1
        for (int i = 0; i < n; i++) {
            k1[i] = h * f[i](x, y);
        }
        
        // Calculate k2
        for (int i = 0; i < n; i++) {
            temp[i] = y[i] + k1[i]/2;
        }
        for (int i = 0; i < n; i++) {
            k2[i] = h * f[i](x + h/2, temp);
        }
        
        // Calculate k3
        for (int i = 0; i < n; i++) {
            temp[i] = y[i] + k2[i]/2;
        }
        for (int i = 0; i < n; i++) {
            k3[i] = h * f[i](x + h/2, temp);
        }
        
        // Calculate k4
        for (int i = 0; i < n; i++) {
            temp[i] = y[i] + k3[i];
        }
        for (int i = 0; i < n; i++) {
            k4[i] = h * f[i](x + h, temp);
        }
        
        // Update solution
        for (int i = 0; i < n; i++) {
            y[i] = y[i] + (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]) / 6.0;
            solution[i].push_back(y[i]);
        }
        
        x = x0 + (step + 1) * h;
    }
    
    return solution;
}

// Shooting method for boundary value problems
std::vector<double> DifferentialEquations::shootingMethod(
    std::function<double(double, double, double)> f,
    double a, double b, double alpha, double beta, int n) {
    
    if (n <= 0) throw std::invalid_argument("Number of points must be positive");
    
    double h = (b - a) / n;
    
    // Define the system for shooting method
    auto system = [f](double x, const std::vector<double>& y) -> std::vector<double> {
        return {y[1], f(x, y[0], y[1])};
    };
    
    // Shooting method using secant method to find correct initial slope
    double s1 = 0.0;  // First guess for initial slope
    double s2 = 1.0;  // Second guess for initial slope
    
    auto shoot = [&](double slope) -> double {
        std::vector<double> initial = {alpha, slope};
        std::vector<std::function<double(double, const std::vector<double>&)>> funcs = {
            [](double x, const std::vector<double>& y) { return y[1]; },
            [f](double x, const std::vector<double>& y) { return f(x, y[0], y[1]); }
        };
        
        auto result = systemRungeKutta4(funcs, a, initial, h, n);
        return result[0].back() - beta;  // Error at right boundary
    };
    
    double f1 = shoot(s1);
    double f2 = shoot(s2);
    
    // Secant method iterations
    for (int iter = 0; iter < 50; iter++) {
        if (std::abs(f2) < 1e-10) break;
        
        double s_new = s2 - f2 * (s2 - s1) / (f2 - f1);
        s1 = s2;
        f1 = f2;
        s2 = s_new;
        f2 = shoot(s2);
    }
    
    // Final solution with correct initial slope
    std::vector<double> initial = {alpha, s2};
    std::vector<std::function<double(double, const std::vector<double>&)>> funcs = {
        [](double x, const std::vector<double>& y) { return y[1]; },
        [f](double x, const std::vector<double>& y) { return f(x, y[0], y[1]); }
    };
    
    auto result = systemRungeKutta4(funcs, a, initial, h, n);
    return result[0];
}

// 1D Heat equation using finite differences
std::vector<std::vector<double>> DifferentialEquations::heatEquation1D(
    double alpha, double L, double T, int nx, int nt,
    std::function<double(double)> initialCondition,
    std::function<double(double)> boundaryLeft,
    std::function<double(double)> boundaryRight) {
    
    if (nx <= 0 || nt <= 0) throw std::invalid_argument("Grid dimensions must be positive");
    
    double dx = L / (nx - 1);
    double dt = T / (nt - 1);
    double r = alpha * dt / (dx * dx);
    
    // Stability check for explicit method
    if (r > 0.5) {
        throw std::runtime_error("Unstable parameters: r = α*dt/dx² must be ≤ 0.5");
    }
    
    std::vector<std::vector<double>> u(nt, std::vector<double>(nx));
    
    // Initial conditions
    for (int i = 0; i < nx; i++) {
        double x = i * dx;
        u[0][i] = initialCondition(x);
    }
    
    // Time stepping
    for (int t = 1; t < nt; t++) {
        double time = t * dt;
        
        // Boundary conditions
        u[t][0] = boundaryLeft(time);
        u[t][nx-1] = boundaryRight(time);
        
        // Interior points using explicit finite difference
        for (int i = 1; i < nx - 1; i++) {
            u[t][i] = u[t-1][i] + r * (u[t-1][i+1] - 2*u[t-1][i] + u[t-1][i-1]);
        }
    }
    
    return u;
}

// 1D Wave equation using finite differences
std::vector<std::vector<double>> DifferentialEquations::waveEquation1D(
    double c, double L, double T, int nx, int nt,
    std::function<double(double)> initialPosition,
    std::function<double(double)> initialVelocity) {
    
    if (nx <= 0 || nt <= 0) throw std::invalid_argument("Grid dimensions must be positive");
    
    double dx = L / (nx - 1);
    double dt = T / (nt - 1);
    double r = c * dt / dx;
    
    // Stability check (CFL condition)
    if (r > 1.0) {
        throw std::runtime_error("Unstable parameters: c*dt/dx must be ≤ 1");
    }
    
    std::vector<std::vector<double>> u(nt, std::vector<double>(nx));
    
    // Initial position
    for (int i = 0; i < nx; i++) {
        double x = i * dx;
        u[0][i] = initialPosition(x);
    }
    
    // First time step using initial velocity
    for (int i = 1; i < nx - 1; i++) {
        double x = i * dx;
        u[1][i] = u[0][i] + dt * initialVelocity(x) + 
                  0.5 * r * r * (u[0][i+1] - 2*u[0][i] + u[0][i-1]);
    }
    
    // Boundary conditions (fixed ends)
    for (int t = 0; t < nt; t++) {
        u[t][0] = 0.0;
        u[t][nx-1] = 0.0;
    }
    
    // Time stepping for t ≥ 2
    for (int t = 2; t < nt; t++) {
        for (int i = 1; i < nx - 1; i++) {
            u[t][i] = 2*u[t-1][i] - u[t-2][i] + 
                      r*r * (u[t-1][i+1] - 2*u[t-1][i] + u[t-1][i-1]);
        }
    }
    
    return u;
}

// 2D Laplace equation using iterative method (Gauss-Seidel)
std::vector<std::vector<double>> DifferentialEquations::laplaceEquation2D(
    int nx, int ny, double tolerance,
    std::function<double(double, double)> boundaryCondition) {
    
    if (nx <= 0 || ny <= 0) throw std::invalid_argument("Grid dimensions must be positive");
    
    std::vector<std::vector<double>> u(ny, std::vector<double>(nx, 0.0));
    std::vector<std::vector<double>> u_old(ny, std::vector<double>(nx, 0.0));
    
    // Set boundary conditions if provided
    if (boundaryCondition) {
        for (int i = 0; i < nx; i++) {
            double x = static_cast<double>(i) / (nx - 1);
            u[0][i] = boundaryCondition(x, 0.0);        // Bottom
            u[ny-1][i] = boundaryCondition(x, 1.0);     // Top
        }
        for (int j = 0; j < ny; j++) {
            double y = static_cast<double>(j) / (ny - 1);
            u[j][0] = boundaryCondition(0.0, y);        // Left
            u[j][nx-1] = boundaryCondition(1.0, y);     // Right
        }
    }
    
    // Iterative solution using Gauss-Seidel method
    int max_iterations = 10000;
    for (int iter = 0; iter < max_iterations; iter++) {
        u_old = u;
        
        // Update interior points
        for (int j = 1; j < ny - 1; j++) {
            for (int i = 1; i < nx - 1; i++) {
                u[j][i] = 0.25 * (u[j][i+1] + u[j][i-1] + u[j+1][i] + u[j-1][i]);
            }
        }
        
        // Check convergence
        double max_error = 0.0;
        for (int j = 1; j < ny - 1; j++) {
            for (int i = 1; i < nx - 1; i++) {
                max_error = std::max(max_error, std::abs(u[j][i] - u_old[j][i]));
            }
        }
        
        if (max_error < tolerance) {
            break;
        }
    }
    
    return u;
}

// Symbolic solution for linear ODEs (placeholder)
std::shared_ptr<Expression> DifferentialEquations::solveLinearODE(const std::vector<double>& coeffs, 
                                                                 std::shared_ptr<Expression> rhs) {
    // This would require full symbolic computation capabilities
    // For now, return placeholder indicating symbolic solution not implemented
    throw std::runtime_error("Symbolic ODE solution not implemented - requires complete expression tree system");
}

// Symbolic solution for separable ODEs (placeholder)
std::shared_ptr<Expression> DifferentialEquations::solveSeparableODE(std::shared_ptr<Expression> expr) {
    // This would require symbolic integration and algebraic manipulation
    // For now, return placeholder indicating symbolic solution not implemented
    throw std::runtime_error("Symbolic separable ODE solution not implemented - requires complete expression tree system");
}