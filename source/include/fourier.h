#pragma once
#include "include/complex_number.h"
#include <vector>
#include <functional>

class FourierTransform {
public:
    // Discrete Fourier Transform
    static std::vector<ComplexNumber> DFT(const std::vector<ComplexNumber>& signal);
    static std::vector<ComplexNumber> IDFT(const std::vector<ComplexNumber>& spectrum);
    
    // Fast Fourier Transform
    static std::vector<ComplexNumber> FFT(const std::vector<ComplexNumber>& signal);
    static std::vector<ComplexNumber> IFFT(const std::vector<ComplexNumber>& spectrum);
    
    // Continuous Fourier Transform (numerical)
    static ComplexNumber continuousFT(std::function<double(double)> f, double omega, double T = 10.0);
    static std::function<double(double)> inverseFT(std::function<ComplexNumber(double)> F, double t_max = 10.0);
    
    // Fourier Series
    static std::vector<ComplexNumber> fourierSeries(std::function<double(double)> f, double period, int harmonics);
    
    // Spectral analysis
    static std::vector<double> powerSpectrum(const std::vector<double>& signal);
    static std::vector<double> magnitude(const std::vector<ComplexNumber>& spectrum);
    static std::vector<double> phase(const std::vector<ComplexNumber>& spectrum);
    
    // Window functions
    static std::vector<double> hammingWindow(int N);
    static std::vector<double> hanningWindow(int N);
    static std::vector<double> blackmanWindow(int N);
};