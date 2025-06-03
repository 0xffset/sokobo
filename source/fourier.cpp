#include "include/fourier.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "include/numerical_methods.h"

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#  define M_E 2.71828
#endif

// Discrete Fourier Transform
std::vector<ComplexNumber> FourierTransform::DFT(const std::vector<ComplexNumber>& signal) {
    int N = signal.size();
    std::vector<ComplexNumber> spectrum(N);
    
    for (int k = 0; k < N; ++k) {
        ComplexNumber sum(0, 0);
        for (int n = 0; n < N; ++n) {
            double angle = -2.0 * M_PI * k * n / N;
            ComplexNumber twiddle(cos(angle), sin(angle));
            sum = sum + signal[n] * twiddle;
        }
        spectrum[k] = sum;
    }
    
    return spectrum;
}

std::vector<ComplexNumber> FourierTransform::IDFT(const std::vector<ComplexNumber>& spectrum) {
    int N = spectrum.size();
    std::vector<ComplexNumber> signal(N);
    
    for (int n = 0; n < N; ++n) {
        ComplexNumber sum(0, 0);
        for (int k = 0; k < N; ++k) {
            double angle = 2.0 * M_PI * k * n / N;
            ComplexNumber twiddle(cos(angle), sin(angle));
            sum = sum + spectrum[k] * twiddle;
        }
        signal[n] = sum / N;
    }
    
    return signal;
}

// Helper function for FFT bit-reversal
static int reverseBits(int num, int bits) {
    int result = 0;
    for (int i = 0; i < bits; ++i) {
        result = (result << 1) | (num & 1);
        num >>= 1;
    }
    return result;
}

// Fast Fourier Transform (Cooley-Tukey algorithm)
std::vector<ComplexNumber> FourierTransform::FFT(const std::vector<ComplexNumber>& signal) {
    int N = signal.size();
    
    // Check if N is a power of 2
    if (N == 0 || (N & (N - 1)) != 0) {
        // If not power of 2, fall back to DFT
        return DFT(signal);
    }
    
    int bits = 0;
    int temp = N;
    while (temp > 1) {
        temp >>= 1;
        bits++;
    }
    
    std::vector<ComplexNumber> result(signal);
    
    // Bit-reversal permutation
    for (int i = 0; i < N; ++i) {
        int j = reverseBits(i, bits);
        if (i < j) {
            std::swap(result[i], result[j]);
        }
    }
    
    // FFT computation
    for (int len = 2; len <= N; len <<= 1) {
        double angle = -2.0 * M_PI / len;
        ComplexNumber wlen(cos(angle), sin(angle));
        
        for (int i = 0; i < N; i += len) {
            ComplexNumber w(1, 0);
            for (int j = 0; j < len / 2; ++j) {
                ComplexNumber u = result[i + j];
                ComplexNumber v = result[i + j + len / 2] * w;
                result[i + j] = u + v;
                result[i + j + len / 2] = u - v;
                w = w * wlen;
            }
        }
    }
    
    return result;
}

std::vector<ComplexNumber> FourierTransform::IFFT(const std::vector<ComplexNumber>& spectrum) {
    int N = spectrum.size();
    
    // Conjugate the input
    std::vector<ComplexNumber> conjugated(N);
    for (int i = 0; i < N; ++i) {
      conjugated[i] = ComplexNumber(spectrum[i].getValue().real(),
                                    -spectrum[i].getValue().imag());
    }
    
    // Apply FFT
    std::vector<ComplexNumber> result = FFT(conjugated);
    
    // Conjugate and normalize
    for (int i = 0; i < N; ++i) {
      result[i] = ComplexNumber(result[i].getValue().real() / N,
                                -result[i].getValue().imag() / N);
    }
    
    return result;
}

// Continuous Fourier Transform (numerical integration)
ComplexNumber FourierTransform::continuousFT(std::function<double(double)> f, double omega, double T) {
    const int samples = 1000;
    double dt = 2 * T / samples;
    ComplexNumber result(0, 0);
    
    for (int n = 0; n < samples; ++n) {
        double t = -T + n * dt;
        double angle = -omega * t;
        ComplexNumber exponential(cos(angle), sin(angle));
        result = result + ComplexNumber(f(t), 0) * exponential * dt;
    }
    
    return result;
}

std::function<double(double)> FourierTransform::inverseFT(std::function<ComplexNumber(double)> F, double t_max) {
    return [F, t_max](double t) -> double {
        const int samples = 1000;
        double domega = 2 * t_max / samples;
        double result = 0.0;
        
        for (int n = 0; n < samples; ++n) {
            double omega = -t_max + n * domega;
            ComplexNumber F_omega = F(omega);
            double angle = omega * t;
            ComplexNumber exponential(cos(angle), sin(angle));
            ComplexNumber integrand = F_omega * exponential;
            result += integrand.getValue().real() * domega;
        }
        
        return result / (2 * M_PI);
    };
}

// Fourier Series
std::vector<ComplexNumber> FourierTransform::fourierSeries(std::function<double(double)> f, double period, int harmonics) {
    std::vector<ComplexNumber> coefficients(2 * harmonics + 1);
    double omega0 = 2 * M_PI / period;
    
    for (int n = -harmonics; n <= harmonics; ++n) {
        const int samples = 1000;
        double dt = period / samples;
        ComplexNumber sum(0, 0);
        
        for (int k = 0; k < samples; ++k) {
            double t = k * dt;
            double angle = -n * omega0 * t;
            ComplexNumber exponential(cos(angle), sin(angle));
            sum = sum + ComplexNumber(f(t), 0) * exponential * dt;
        }
        
        coefficients[n + harmonics] = sum / period;
    }
    
    return coefficients;
}

// Spectral analysis
std::vector<double> FourierTransform::powerSpectrum(const std::vector<double>& signal) {
    // Convert real signal to complex
    std::vector<ComplexNumber> complexSignal;
    for (double val : signal) {
        complexSignal.push_back(ComplexNumber(val, 0));
    }
    
    // Compute FFT
    std::vector<ComplexNumber> spectrum = FFT(complexSignal);
    
    // Compute power spectrum
    std::vector<double> power(spectrum.size());
    for (size_t i = 0; i < spectrum.size(); ++i) {
      power[i] = spectrum[i].getValue().real() * spectrum[i].getValue().real()
          + 
                   spectrum[i].getValue().imag() * spectrum[i].getValue().imag();
    }
    
    return power;
}

std::vector<double> FourierTransform::magnitude(const std::vector<ComplexNumber>& spectrum) {
    std::vector<double> mag(spectrum.size());
    for (size_t i = 0; i < spectrum.size(); ++i) {
      mag[i] =
          sqrt(spectrum[i].getValue().real() * spectrum[i].getValue().real()
                    + 
                      spectrum[i].getValue().imag() * spectrum[i].getValue().imag());
    }
    return mag;
}

std::vector<double> FourierTransform::phase(const std::vector<ComplexNumber>& spectrum) {
    std::vector<double> ph(spectrum.size());
    for (size_t i = 0; i < spectrum.size(); ++i) {
      ph[i] =
          atan2(spectrum[i].getValue().imag(), spectrum[i].getValue().real());
    }
    return ph;
}

// Window functions
std::vector<double> FourierTransform::hammingWindow(int N) {
    std::vector<double> window(N);
    for (int n = 0; n < N; ++n) {
        window[n] = 0.54 - 0.46 * cos(2 * M_PI * n / (N - 1));
    }
    return window;
}

std::vector<double> FourierTransform::hanningWindow(int N) {
    std::vector<double> window(N);
    for (int n = 0; n < N; ++n) {
        window[n] = 0.5 * (1 - cos(2 * M_PI * n / (N - 1)));
    }
    return window;
}

std::vector<double> FourierTransform::blackmanWindow(int N) {
    std::vector<double> window(N);
    for (int n = 0; n < N; ++n) {
        double factor = 2 * M_PI * n / (N - 1);
        window[n] = 0.42 - 0.5 * cos(factor) + 0.08 * cos(2 * factor);
    }
    return window;
}