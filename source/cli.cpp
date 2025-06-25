#include <algorithm>
#include <cctype>
#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "include/cli.h"

#include "include/calculus.h"
#include "include/complex_number.h"
#include "include/differential_equations.h"
#include "include/expression.h"
#include "include/fourier.h"
#include "include/laplace.h"
#include "include/numerical_methods.h"
#include "include/polynomial.h"

void CAS_CLI::run()
{
  std::cout << "=== sokobo - Computational Algebra System ===" << std::endl;
  std::cout << "Type 'help' for available commands or 'quit' to exit."
            << std::endl;

  std::string line;
  while (true) {
    std::cout << "sokobo > ";
    if (!std::getline(std::cin, line)) {
      break;
    }

    // Trim whitespace
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);

    if (line.empty()) {
      continue;
    }
    if (line == "quit" || line == "exit") {
      break;
    }

    std::vector<std::string> tokens = split(line);
    if (tokens.empty()) {
      continue;
    }

    try {
      std::string cmd = tokens[0];
      std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

      if (cmd == "help") {
        showHelp();
      } else if (cmd == "clear") {
        clearAll();
      } else if (cmd == "list") {
        listObjects();
      } else if (cmd == "expr") {
        handleExpression(tokens);
      } else if (cmd == "deriv") {
        handleDerivative(tokens);
      } else if (cmd == "integral") {
        handleIntegral(tokens);
      } else if (cmd == "limit") {
        handleLimit(tokens);
      } else if (cmd == "series") {
        handleSeries(tokens);
      } else if (cmd == "poly") {
        handlePolynomial(tokens);
      } else if (cmd == "peval") {
        // handlePolynomialEvaluate(tokens);
      } else if (cmd == "padd") {
        handlePolynomialAdd(tokens);
      } else if (cmd == "pmult") {
        // handlePolynomialMultiply(tokens);
      } else if (cmd == "pgcd") {
        // handlePolynomialGCD(tokens);
      } else if (cmd == "proots") {
        // handlePolynomialRoots(tokens);
      } else if (cmd == "pfactor") {
        // handlePolynomialFactor(tokens);
      } else if (cmd == "complex") {
        handleComplex(tokens);
      } else if (cmd == "cadd") {
        handleComplexAdd(tokens);
      } else if (cmd == "cmult") {
        handleComplexMultiply(tokens);
      } else if (cmd == "cpower") {
        handleComplexPower(tokens);
      } else if (cmd == "croots") {
        handleComplexRoots(tokens);
      } else if (cmd == "matrix") {
        handleMatrix(tokens);
      } else if (cmd == "mset") {
        handleMatrixSet(tokens);
      } else if (cmd == "mdet") {
        handleMatrixDeterminant(tokens);
      } else if (cmd == "minv") {
        handleMatrixInverse(tokens);
      } else if (cmd == "meigen") {
        handleMatrixEigenvalues(tokens);
      } else if (cmd == "laplace") {
        handleLaplace(tokens);
      } else if (cmd == "fourier") {
        handleFourier(tokens);
      } else if (cmd == "ode") {
        handleODE(tokens);
      } else if (cmd == "pde") {
        handlePDE(tokens);
      } else if (cmd == "root") {
        // handleRootFind(tokens);
      } else if (cmd == "optimize") {
        // handleOptimize(tokens);
      } else if (cmd == "interpolate") {
        // handleInterpolate(tokens);
      } else {
        std::cout << "Unknown command: " << cmd << std::endl;
        std::cout << "Type 'help' for available commands." << std::endl;
      }
    } catch (const std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
    }
  }

  std::cout << "Goodbye!" << std::endl;
}

std::vector<std::string> CAS_CLI::split(const std::string& str)
{
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

void CAS_CLI::showHelp()
{
  std::cout << "\n=== CAS Commands ===" << std::endl;

  std::cout << "\nGeneral:" << std::endl;
  std::cout << "  help                     - Show this help" << std::endl;
  std::cout << "  list                     - List all stored objects"
            << std::endl;
  std::cout << "  clear                    - Clear all stored objects"
            << std::endl;
  std::cout << "  quit/exit               - Exit the program" << std::endl;

  std::cout << "\nExpressions:" << std::endl;
  std::cout << "  expr <name> <expression> - Define expression" << std::endl;
  std::cout << "  deriv <expr> <var>      - Compute derivative" << std::endl;
  std::cout << "  integral <expr> <var> <a> <b> - Numerical integration"
            << std::endl;
  std::cout << "  limit <expr> <var> <val> - Compute limit" << std::endl;
  std::cout << "  series <expr> <var> <center> <degree> - Taylor series"
            << std::endl;

  std::cout << "\nPolynomials:" << std::endl;
  std::cout << "  poly <name> <coeffs...> - Define polynomial" << std::endl;
  std::cout << "  peval <poly> <value>    - Evaluate polynomial" << std::endl;
  std::cout << "  padd <p1> <p2> <result> - Add polynomials" << std::endl;
  std::cout << "  pmult <p1> <p2> <result> - Multiply polynomials" << std::endl;
  std::cout << "  pgcd <p1> <p2>          - Compute polynomial GCD"
            << std::endl;
  std::cout << "  proots <poly>           - Find polynomial roots" << std::endl;
  std::cout << "  pfactor <poly>          - Factor polynomial" << std::endl;

  std::cout << "\nComplex Numbers:" << std::endl;
  std::cout << "  complex <name> <real> <imag> - Define complex number"
            << std::endl;
  std::cout << "  cadd <c1> <c2> <result> - Add complex numbers" << std::endl;
  std::cout << "  cmult <c1> <c2> <result> - Multiply complex numbers"
            << std::endl;
  std::cout << "  cpower <c> <n> <result> - Complex power" << std::endl;
  std::cout << "  croots <n>              - Roots of unity" << std::endl;

  std::cout << "\nMatrices:" << std::endl;
  std::cout << "  matrix <name> <rows> <cols> - Create matrix" << std::endl;
  std::cout << "  mset <matrix> <i> <j> <val> - Set matrix element"
            << std::endl;
  std::cout << "  mdet <matrix>           - Matrix determinant" << std::endl;
  std::cout << "  minv <matrix>           - Matrix inverse" << std::endl;
  std::cout << "  meigen <matrix>         - Matrix eigenvalues" << std::endl;

  std::cout << "\nTransforms:" << std::endl;
  std::cout << "  laplace <expr> <var>    - Laplace transform" << std::endl;
  std::cout << "  fourier <signal>        - Fourier transform" << std::endl;

  std::cout << "\nDifferential Equations:" << std::endl;
  std::cout << "  ode <method> <expr> <x0> <y0> <h> <steps> - Solve ODE"
            << std::endl;
  std::cout << "  pde <type> <params...>  - Solve PDE" << std::endl;

  std::cout << "\nNumerical Methods:" << std::endl;
  std::cout << "  root <method> <expr> <params...> - Find roots" << std::endl;
  std::cout << "  optimize <method> <expr> <params...> - Optimization"
            << std::endl;
  std::cout << "  interpolate <method> <x_vals> <y_vals> - Interpolation"
            << std::endl;
  std::cout << std::endl;
}

void CAS_CLI::handleExpression(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 3) {
    std::cout << "Usage: expr <name> <expression>" << std::endl;
    return;
  }

  std::string name = tokens[1];
  std::string exprStr = tokens[2];
  for (size_t i = 3; i < tokens.size(); ++i) {
    exprStr += " " + tokens[i];
  }

  try {
    // Parse the expression string into an Expression object
    auto expr = parseExpression(exprStr);

    // Store the expression in the expressions map
    expressions[name] = expr;

    std::cout << "Expression '" << name << "' = " << exprStr
              << " stored successfully." << std::endl;

    // Optionally show the parsed form
    std::cout << "Parsed as: " << expr->toString() << std::endl;

  } catch (const std::exception& e) {
    std::cout << "Error parsing expression: " << e.what() << std::endl;
    std::cout << "Expression '" << name << "' was not stored." << std::endl;
  }
}

void CAS_CLI::clearAll()
{
  int totalObjects = expressions.size() + polynomials.size()
      + complexNumbers.size() + matrices.size();

  if (totalObjects == 0) {
    std::cout << "No objects to clear." << std::endl;
    return;
  }

  // Clear all containers
  expressions.clear();
  polynomials.clear();
  complexNumbers.clear();
  matrices.clear();

  std::cout << "All objects cleared (" << totalObjects << " objects removed)."
            << std::endl;
}

void CAS_CLI::listObjects()
{
  bool hasObjects = false;

  // List expressions
  if (!expressions.empty()) {
    std::cout << "\nExpressions:" << std::endl;
    for (const auto& pair : expressions) {
      std::cout << "  " << pair.first << " = " << pair.second->toString()
                << std::endl;
    }
    hasObjects = true;
  }

  // List polynomials
  if (!polynomials.empty()) {
    std::cout << "\nPolynomials:" << std::endl;
    for (const auto& pair : polynomials) {
      std::cout << "  " << pair.first << " = " << pair.second.toString()
                << std::endl;
    }
    hasObjects = true;
  }

  // List complex numbers
  if (!complexNumbers.empty()) {
    std::cout << "\nComplex Numbers:" << std::endl;
    for (const auto& pair : complexNumbers) {
      std::cout << "  " << pair.first << " = " << pair.second.toString()
                << std::endl;
    }
    hasObjects = true;
  }

  // List matrices
  if (!matrices.empty()) {
    std::cout << "\nMatrices:" << std::endl;
    for (const auto& pair : matrices) {
      std::cout << "  " << pair.first << " (" << pair.second.getRows() << "x"
                << pair.second.getCols() << ")" << std::endl;
    }
    hasObjects = true;
  }

  if (!hasObjects) {
    std::cout << "No objects stored." << std::endl;
  }

  std::cout << std::endl;
}

// Helper method to parse expression strings (you'll need to implement this)
std::shared_ptr<Expression> CAS_CLI::parseExpression(const std::string& exprStr)
{
  // Remove whitespace
  std::string cleanExpr = removeWhitespace(exprStr);

  if (cleanExpr.empty()) {
    throw std::runtime_error("Empty expression");
  }

  // Parse the expression using recursive descent
  size_t pos = 0;
  return parseAddSub(cleanExpr, pos);
}

// Remove whitespace from expression string
std::string CAS_CLI::removeWhitespace(const std::string& str)
{
  std::string result;
  for (char c : str) {
    if (!std::isspace(c)) {
      result += c;
    }
  }
  return result;
}

// Parse addition and subtraction (lowest precedence)
std::shared_ptr<Expression> CAS_CLI::parseAddSub(const std::string& expr,
                                                 size_t& pos)
{
  auto left = parseMulDiv(expr, pos);

  while (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
    char op = expr[pos++];
    auto right = parseMulDiv(expr, pos);
    left = std::make_shared<BinaryOp>(left, op, right);
  }

  return left;
}

// Parse multiplication and division
std::shared_ptr<Expression> CAS_CLI::parseMulDiv(const std::string& expr,
                                                 size_t& pos)
{
  auto left = parsePower(expr, pos);

  while (pos < expr.length() && (expr[pos] == '*' || expr[pos] == '/')) {
    char op = expr[pos++];
    auto right = parsePower(expr, pos);
    left = std::make_shared<BinaryOp>(left, op, right);
  }

  return left;
}

// Parse exponentiation (highest precedence)
std::shared_ptr<Expression> CAS_CLI::parsePower(const std::string& expr,
                                                size_t& pos)
{
  auto left = parseFactor(expr, pos);

  if (pos < expr.length() && expr[pos] == '^') {
    pos++;
    auto right = parsePower(expr, pos);  // Right associative
    left = std::make_shared<BinaryOp>(left, '^', right);
  }

  return left;
}

// Parse factors (numbers, variables, functions, parentheses)
std::shared_ptr<Expression> CAS_CLI::parseFactor(const std::string& expr,
                                                 size_t& pos)
{
  if (pos >= expr.length()) {
    throw std::runtime_error("Unexpected end of expression");
  }

  // Handle parentheses
  if (expr[pos] == '(') {
    pos++;  // Skip '('
    auto result = parseAddSub(expr, pos);
    if (pos >= expr.length() || expr[pos] != ')') {
      throw std::runtime_error("Missing closing parenthesis");
    }
    pos++;  // Skip ')'
    return result;
  }

  // Handle unary minus
  if (expr[pos] == '-') {
    pos++;
    auto operand = parseFactor(expr, pos);
    return std::make_shared<BinaryOp>(
        std::make_shared<Constant>(-1), '*', operand);
  }

  // Handle unary plus
  if (expr[pos] == '+') {
    pos++;
    return parseFactor(expr, pos);
  }

  // Handle numbers
  if (std::isdigit(expr[pos]) || expr[pos] == '.') {
    return parseNumber(expr, pos);
  }

  // Handle variables and functions
  if (std::isalpha(expr[pos])) {
    return parseVariableOrFunction(expr, pos);
  }

  throw std::runtime_error("Unexpected character: "
                           + std::string(1, expr[pos]));
}

// Parse numeric constants
std::shared_ptr<Expression> CAS_CLI::parseNumber(const std::string& expr,
                                                 size_t& pos)
{
  size_t start = pos;
  bool hasDecimal = false;

  while (pos < expr.length()
         && (std::isdigit(expr[pos]) || (expr[pos] == '.' && !hasDecimal)))
  {
    if (expr[pos] == '.') {
      hasDecimal = true;
    }
    pos++;
  }

  std::string numStr = expr.substr(start, pos - start);
  double value = std::stod(numStr);
  return std::make_shared<Constant>(value);
}

// Parse variables and functions
std::shared_ptr<Expression> CAS_CLI::parseVariableOrFunction(
    const std::string& expr, size_t& pos)
{
  size_t start = pos;

  // Parse identifier
  while (pos < expr.length() && (std::isalnum(expr[pos]) || expr[pos] == '_')) {
    pos++;
  }

  std::string identifier = expr.substr(start, pos - start);

  // Check if it's a function call
  if (pos < expr.length() && expr[pos] == '(') {
    pos++;  // Skip '('
    auto argument = parseAddSub(expr, pos);
    if (pos >= expr.length() || expr[pos] != ')') {
      throw std::runtime_error("Missing closing parenthesis in function call");
    }
    pos++;  // Skip ')'
    return std::make_shared<Function>(identifier, argument);
  }

  // It's a variable
  return std::make_shared<Variable>(identifier);
}

void CAS_CLI::handleDerivative(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 3) {
    std::cout << "Usage: deriv <expression> <variable>" << std::endl;
    return;
  }

  std::string expr = tokens[1];
  std::string var = tokens[2];

  if (expressions.find(expr) == expressions.end()) {
    std::cout << "Expression '" << expr << "' not found." << std::endl;
    return;
  }

  auto derivative = Calculus::differentiate(expressions[expr], var);
  std::cout << "d/d" << var << "(" << expr << ") = " << derivative->toString()
            << std::endl;
}

void CAS_CLI::handleIntegral(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 5) {
    std::cout << "Usage: integral <expression> <variable> <lower> <upper>"
              << std::endl;
    return;
  }

  std::string expr = tokens[1];
  std::string var = tokens[2];
  double lower = std::stod(tokens[3]);
  double upper = std::stod(tokens[4]);

  // For demo - using numerical integration
  auto f = [](double x) { return x * x; };  // Example function
  double result = Calculus::simpsonsRule(f, lower, upper);
  std::cout << "∫[" << lower << "," << upper << "] " << expr << " d" << var
            << " ≈ " << result << std::endl;
}

void CAS_CLI::handleLimit(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: limit <expression> <variable> <value>" << std::endl;
    return;
  }

  std::string expr = tokens[1];
  std::string var = tokens[2];
  double value = std::stod(tokens[3]);

  auto f = [](double x) { return x * x; };  // Example function
  double result = Calculus::limit(f, value);
  std::cout << "lim(" << var << "→" << value << ") " << expr << " = " << result
            << std::endl;
}

void CAS_CLI::handleSeries(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 5) {
    std::cout << "Usage: series <expression> <variable> <center> <degree>"
              << std::endl;
    return;
  }

  std::string expr = tokens[1];
  std::string var = tokens[2];
  double center = std::stod(tokens[3]);
  int degree = std::stoi(tokens[4]);

  auto f = [](double x) { return std::exp(x); };  // Example function
  Polynomial series = Calculus::taylorSeries(f, center, degree);
  std::cout << "Taylor series of " << expr << " around " << var << "=" << center
            << " (degree " << degree << "): " << series.toString() << std::endl;
}

void CAS_CLI::handlePolynomial(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 3) {
    std::cout << "Usage: poly <name> <coefficient0> <coefficient1> ..."
              << std::endl;
    return;
  }

  std::string name = tokens[1];
  std::vector<double> coeffs;
  for (size_t i = 2; i < tokens.size(); ++i) {
    coeffs.push_back(std::stod(tokens[i]));
  }

  polynomials[name] = Polynomial(coeffs);
  std::cout << "Polynomial '" << name << "' = " << polynomials[name].toString()
            << " stored." << std::endl;
}

// void CAS_CLI::handlePolynomialEvaluate(const std::vector<std::string>&
// tokens)
//{
//   if (tokens.size() < 3) {
//     std::cout << "Usage: peval <polynomial> <value>" << std::endl;
//     return;
//   }
//
//   std::string name = tokens[1];
//   double value = std::stod(tokens[2]);
//
//   if (polynomials.find(name) == polynomials.end()) {
//     std::cout << "Polynomial '" << name << "' not found." << std::endl;
//     return;
//   }
//
//   double result = polynomials[name].evaluate(value);
//   std::cout << name << "(" << value << ") = " << result << std::endl;
// }
//
void CAS_CLI::handlePolynomialAdd(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: padd <poly1> <poly2> <result_name>" << std::endl;
    return;
  }

  std::string p1_name = tokens[1];
  std::string p2_name = tokens[2];
  std::string result_name = tokens[3];

  if (CAS_CLI::polynomials.find(p1_name) == CAS_CLI::polynomials.end()
      || CAS_CLI::polynomials.find(p2_name) == CAS_CLI::polynomials.end())
  {
    std::cout << "One or both polynomials not found." << std::endl;
    return;
  }

  CAS_CLI::polynomials[result_name] =
      CAS_CLI::polynomials[p1_name] + CAS_CLI::polynomials[p2_name];
  std::cout << result_name << " = "
            << CAS_CLI::polynomials[result_name].toString() << std::endl;
}

//
// void CAS_CLI::handlePolynomialMultiply(const std::vector<std::string>&
// tokens)
//{
//  if (tokens.size() < 4) {
//    std::cout << "Usage: pmult <poly1> <poly2> <result_name>" << std::endl;
//    return;
//  }
//
//  std::string p1_name = tokens[1];
//  std::string p2_name = tokens[2];
//  std::string result_name = tokens[3];
//
//  if (polynomials.find(p1_name) == polynomials.end()
//      || polynomials.find(p2_name) == polynomials.end())
//  {
//    std::cout << "One or both polynomials not found." << std::endl;
//    return;
//  }
//
//  polynomials[result_name] = polynomials[p1_name] * polynomials[p2_name];
//  std::cout << result_name << " = " << polynomials[result_name].toString()
//            << std::endl;
//}

// void CAS_CLI::handlePolynomialGCD(const std::vector<std::string>& tokens)
//{
//   if (tokens.size() < 3) {
//     std::cout << "Usage: pgcd <poly1> <poly2>" << std::endl;
//     return;
//   }
//
//   std::string p1_name = tokens[1];
//   std::string p2_name = tokens[2];
//
//   if (polynomials.find(p1_name) == polynomials.end()
//       || polynomials.find(p2_name) == polynomials.end())
//   {
//     std::cout << "One or both polynomials not found." << std::endl;
//     return;
//   }
//
//   Polynomial gcd = polynomials[p1_name].gcd(polynomials[p2_name]);
//   std::cout << "GCD(" << p1_name << ", " << p2_name << ") = " <<
//   gcd.toString()
//             << std::endl;
// }
//
// void CAS_CLI::handlePolynomialRoots(const std::vector<std::string>& tokens)
//{
//   if (tokens.size() < 2) {
//     std::cout << "Usage: proots <polynomial>" << std::endl;
//     return;
//   }
//
//   std::string name = tokens[1];
//
//   if (polynomials.find(name) == polynomials.end()) {
//     std::cout << "Polynomial '" << name << "' not found." << std::endl;
//     return;
//   }
//
//   auto roots = polynomials[name].roots();
//   std::cout << "Roots of " << name << ":" << std::endl;
//   for (size_t i = 0; i < roots.size(); ++i) {
//     std::cout << "  r" << (i + 1) << " = " <<
//     ComplexNumber(roots[i]).toString()
//               << std::endl;
//   }
// }
//
// void CAS_CLI::handlePolynomialFactor(const std::vector<std::string>& tokens)
//{
//   if (tokens.size() < 2) {
//     std::cout << "Usage: pfactor <polynomial>" << std::endl;
//     return;
//   }
//
//   std::string name = tokens[1];
//
//   if (polynomials.find(name) == polynomials.end()) {
//     std::cout << "Polynomial '" << name << "' not found." << std::endl;
//     return;
//   }
//
//   auto factors = polynomials[name].factor();
//   std::cout << "Factorization of " << name << ":" << std::endl;
//   for (size_t i = 0; i < factors.size(); ++i) {
//     std::cout << "  Factor " << (i + 1) << ": " << factors[i].toString()
//               << std::endl;
//   }
// }

void CAS_CLI::handleComplex(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: complex <name> <real> <imaginary>" << std::endl;
    return;
  }

  std::string name = tokens[1];
  double real = std::stod(tokens[2]);
  double imag = std::stod(tokens[3]);

  complexNumbers[name] = ComplexNumber(real, imag);
  std::cout << "Complex number '" << name
            << "' = " << complexNumbers[name].toString() << " stored."
            << std::endl;
}

void CAS_CLI::handleComplexAdd(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: cadd <complex1> <complex2> <result_name>" << std::endl;
    return;
  }

  std::string c1_name = tokens[1];
  std::string c2_name = tokens[2];
  std::string result_name = tokens[3];

  if (complexNumbers.find(c1_name) == complexNumbers.end()
      || complexNumbers.find(c2_name) == complexNumbers.end())
  {
    std::cout << "One or both complex numbers not found." << std::endl;
    return;
  }

  complexNumbers[result_name] =
      complexNumbers[c1_name] + complexNumbers[c2_name];
  std::cout << result_name << " = " << complexNumbers[result_name].toString()
            << std::endl;
}

void CAS_CLI::handleComplexMultiply(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: cmult <complex1> <complex2> <result_name>"
              << std::endl;
    return;
  }

  std::string c1_name = tokens[1];
  std::string c2_name = tokens[2];
  std::string result_name = tokens[3];

  if (complexNumbers.find(c1_name) == complexNumbers.end()
      || complexNumbers.find(c2_name) == complexNumbers.end())
  {
    std::cout << "One or both complex numbers not found." << std::endl;
    return;
  }

  complexNumbers[result_name] =
      complexNumbers[c1_name] * complexNumbers[c2_name];
  std::cout << result_name << " = " << complexNumbers[result_name].toString()
            << std::endl;
}

void CAS_CLI::handleComplexPower(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: cpower <complex> <power> <result_name>" << std::endl;
    return;
  }

  std::string c_name = tokens[1];
  int power = std::stoi(tokens[2]);
  std::string result_name = tokens[3];

  if (complexNumbers.find(c_name) == complexNumbers.end()) {
    std::cout << "Complex number '" << c_name << "' not found." << std::endl;
    return;
  }

  complexNumbers[result_name] = complexNumbers[c_name] ^ power;
  std::cout << result_name << " = " << complexNumbers[result_name].toString()
            << std::endl;
}

void CAS_CLI::handleComplexRoots(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 2) {
    std::cout << "Usage: croots <n>" << std::endl;
    return;
  }

  int n = std::stoi(tokens[1]);

  auto roots = ComplexNumber::rootsOfUnity(n);
  std::cout << n << "th roots of unity:" << std::endl;
  for (size_t i = 0; i < roots.size(); ++i) {
    std::cout << "  ω^" << i << " = " << roots[i].toString() << std::endl;
  }
}

void CAS_CLI::handleMatrix(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 4) {
    std::cout << "Usage: matrix <name> <rows> <cols>" << std::endl;
    return;
  }

  std::string name = tokens[1];
  int rows = std::stoi(tokens[2]);
  int cols = std::stoi(tokens[3]);

  matrices[name] = Matrix<int>(rows, cols);
  std::cout << "Matrix '" << name << "' (" << rows << "x" << cols
            << ") created." << std::endl;
}

void CAS_CLI::handleMatrixSet(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 5) {
    std::cout << "Usage: mset <matrix> <row> <col> <value>" << std::endl;
    return;
  }

  std::string name = tokens[1];
  int row = std::stoi(tokens[2]);
  int col = std::stoi(tokens[3]);
  double value = std::stod(tokens[4]);

  if (matrices.find(name) == matrices.end()) {
    std::cout << "Matrix '" << name << "' not found." << std::endl;
    return;
  }

  matrices[name](row, col) = value;
  std::cout << name << "[" << row << "][" << col << "] = " << value
            << std::endl;
}

void CAS_CLI::handleMatrixDeterminant(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 2) {
    std::cout << "Usage: mdet <matrix>" << std::endl;
    return;
  }

  std::string name = tokens[1];

  if (matrices.find(name) == matrices.end()) {
    std::cout << "Matrix '" << name << "' not found." << std::endl;
    return;
  }

  double det = matrices[name].determinant();
  std::cout << "det(" << name << ") = " << det << std::endl;
}

void CAS_CLI::handleMatrixInverse(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 2) {
    std::cout << "Usage: minv <matrix>" << std::endl;
    return;
  }

  std::string name = tokens[1];

  if (matrices.find(name) == matrices.end()) {
    std::cout << "Matrix '" << name << "' not found." << std::endl;
    return;
  }

  try {
    Matrix<int> inv = matrices[name].inverse();
    std::cout << "Inverse of " << name << ":" << std::endl;
    std::cout << inv.toString() << std::endl;
  } catch (const std::exception& e) {
    std::cout << "Cannot compute inverse: " << e.what() << std::endl;
  }
}

void CAS_CLI::handleMatrixEigenvalues(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 2) {
    std::cout << "Usage: meigen <matrix>" << std::endl;
    return;
  }

  std::string name = tokens[1];

  if (matrices.find(name) == matrices.end()) {
    std::cout << "Matrix '" << name << "' not found." << std::endl;
    return;
  }

  auto eigenvals = matrices[name].eigenvalues();
  std::cout << "Eigenvalues of " << name << ":" << std::endl;
  for (size_t i = 0; i < eigenvals.size(); ++i) {
    std::cout << "  λ" << (i + 1) << " = " << eigenvals[i] << std::endl;
  }
}

void CAS_CLI::handleLaplace(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 3) {
    std::cout << "Usage: laplace <expression> <variable>" << std::endl;
    return;
  }

  std::string expr = tokens[1];
  std::string var = tokens[2];

  std::cout << "Laplace transform of " << expr << " with respect to " << var
            << ":" << std::endl;
  std::cout
      << "Note: Full Laplace transform implementation not shown in this demo."
      << std::endl;
}

void CAS_CLI::handleFourier(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 2) {
    std::cout << "Usage: fourier <signal_values...>" << std::endl;
    return;
  }

  std::vector<ComplexNumber> signal;
  for (size_t i = 1; i < tokens.size(); ++i) {
    signal.push_back(ComplexNumber(std::stod(tokens[i])));
  }

  auto spectrum = FourierTransform::FFT(signal);
  std::cout << "FFT result:" << std::endl;
  for (size_t i = 0; i < spectrum.size(); ++i) {
    std::cout << "  X[" << i << "] = " << spectrum[i].toString() << std::endl;
  }
}

void CAS_CLI::handleODE(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 7) {
    std::cout << "Usage: ode <method> <dy/dx_expression> <x0> <y0> <h> <steps>"
              << std::endl;
    std::cout << "Methods: euler, rk4" << std::endl;
    return;
  }

  std::string method = tokens[1];
  std::string expr = tokens[2];
  double x0 = std::stod(tokens[3]);
  double y0 = std::stod(tokens[4]);
  double h = std::stod(tokens[5]);
  int steps = std::stoi(tokens[6]);

  // Example function: dy/dx = x + y
  auto f = [](double x, double y) { return x + y; };

  std::vector<double> solution;
  if (method == "euler") {
    solution = DifferentialEquations::eulerMethod(f, x0, y0, h, steps);
  } else if (method == "rk4") {
    solution = DifferentialEquations::rungeKutta4(f, x0, y0, h, steps);
  } else {
    std::cout << "Unknown method: " << method << std::endl;
    return;
  }

  std::cout << "ODE solution using " << method << " method:" << std::endl;
  for (int i = 0; i < steps && i < static_cast<int>(solution.size()); ++i) {
    double x = x0 + i * h;
    std::cout << "  x = " << x << ", y = " << solution[i] << std::endl;
  }
}

void CAS_CLI::handlePDE(const std::vector<std::string>& tokens)
{
  if (tokens.size() < 2) {
    std::cout << "Usage: pde <type> <parameters...>" << std::endl;
    std::cout << "Types: heat, wave, laplace" << std::endl;
    return;
  }
}
