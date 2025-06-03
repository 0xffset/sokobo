#pragma once
#include <vector>
#include <string>
#include <complex>

class Polynomial {
private:
    std::vector<double> coefficients;
    void removeLeadingZeros();
    
public:
    Polynomial(const std::vector<double>& coeffs);
    Polynomial(double constant = 0);
    
    int degree() const;
    double getCoeff(int i) const;
    void setCoeff(int i, double val);
    int getDegree() const; 
    
    // Basic operations
    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;
    
    // Advanced operations
    Polynomial gcd(const Polynomial& other) const;
    Polynomial derivative() const;
    Polynomial integral() const;
    std::vector<std::complex<double>> roots() const;
    
    // Evaluation
    double evaluate(double x) const;
    std::complex<double> evaluate(std::complex<double> x) const;
    
    // Factorization
    std::vector<Polynomial> factor() const;
    
    // String representation
    std::string toString() const;
    
    // Static methods for polynomial interpolation
    static Polynomial lagrangeInterpolation(const std::vector<double>& x, const std::vector<double>& y);
    static Polynomial newtonInterpolation(const std::vector<double>& x, const std::vector<double>& y);
};