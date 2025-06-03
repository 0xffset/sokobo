# sokobo - Computational Algebra System (CAS)


A comprehensive C++ implementation of a Computer Algebra System with advanced mathematical capabilities.

## Features

### Core Functionality
- **Symbolic Expression Manipulation**: Variables, constants, binary operations, functions
- **Polynomial Arithmetic**: Addition, multiplication, division, GCD, factorization
- **Complex Number Operations**: All basic operations plus advanced functions
- **Matrix Operations**: Determinant, inverse, eigenvalues, decompositions
- **Calculus**: Symbolic differentiation, numerical integration, series expansions
- **Transforms**: Laplace transforms, Fourier transforms (DFT/FFT)
- **Differential Equations**: ODE and PDE solvers
- **Numerical Methods**: Root finding, optimization, interpolation

### Advanced Features
- Taylor and Chebyshev series expansions
- Multiple integration methods (Trapezoidal, Simpson's, Gaussian quadrature, Monte Carlo)
- Various ODE solvers (Euler, Runge-Kutta, Adams-Bashforth)
- PDE solvers for heat, wave, and Laplace equations
- Linear system solvers (Gaussian elimination, LU decomposition, iterative methods)
- Statistical functions and regression analysis
- Special functions (Gamma, Beta, Error, Bessel, Legendre, Hermite)

## Building

### Using Make
```bash
make
```

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the executable:
```bash
./cas
```

### Example Commands

#### Polynomials
```
poly p1 1 2 3        # Define p1 = 1 + 2x + 3x²
poly p2 2 1          # Define p2 = 2 + x
add p1 p2            # Add polynomials
mult p1 p2           # Multiply polynomials
deriv p1             # Derivative of p1
roots p1             # Find roots of p1
factor p1            # Factor polynomial
```

#### Complex Numbers
```
complex c1 3 4       # Define c1 = 3 + 4i
complex c2 1 -2      # Define c2 = 1 - 2i
cadd c1 c2           # Add complex numbers
cmult c1 c2          # Multiply complex numbers
cpower c1 3          # c1^3
croots 8 3           # 3rd roots of unity
```

#### Expressions and Calculus
```
expr f x^2+sin(x)    # Define expression f = x² + sin(x)
deriv f x            # Derivative of f with respect to x
integral f x 0 1     # Integrate f from 0 to 1
series f 0 5         # Taylor series of f around 0, degree 5
limit f 0            # Limit of f as x approaches 0
```

#### Matrices
```
matrix m1 3 3        # Create 3x3 matrix m1
mset m1 0 0 5        # Set m1[0][0] = 5
mdet m1              # Determinant of m1
minv m1              # Inverse of m1
meigen m1            # Eigenvalues of m1
```

#### Transforms
```
laplace f t          # Laplace transform of f(t)
ilaplace F s         # Inverse Laplace transform of F(s)
fourier signal       # Fourier transform of signal
ifourier spectrum    # Inverse Fourier transform
```

#### Differential Equations
```
ode euler y'=x+y 0 1 0.1 10    # Solve y'=x+y using Euler method
ode rk4 y'=x+y 0 1 0.1 10      # Solve using Runge-Kutta 4th order
pde heat 1.0 1.0 1.0 50 50     # Solve 1D heat equation
```

#### Numerical Methods
```
root bisect f -1 1             # Find root using bisection method
root newton f df 1             # Find root using Newton-Raphson
optimize golden f -1 1         # Find minimum using golden section
interpolate lagrange x_vals y_vals  # Lagrange interpolation
```

## Project Structure

```
cas/
├── include/           # Header files
│   ├── expression.h
│   ├── polynomial.h
│   ├── complex_number.h
│   ├── matrix.h
│   ├── calculus.h
│   ├── laplace.h
│   ├── fourier.h
│   ├── differential_equations.h
│   ├── numerical_methods.h
│   └── cli.h
├── src/              # Source

# Building and installing

See the [BUILDING](BUILDING.md) document.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

<!--
Please go to https://choosealicense.com/licenses/ and choose a license that
fits your needs. The recommended license for a project of this type is the
GNU AGPLv3.
-->
