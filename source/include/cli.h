#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "complex_number.h"
#include "expression.h"
#include "matrix.h"
#include "polynomial.h"

class CAS_CLI
{
private:
  // Command handlers
  void showHelp();
  void showHelpMatrices();
  std::vector<std::string> split(const std::string& str);

  // Expression commands
  void handleExpression(const std::vector<std::string>& tokens);
  void handleDerivative(const std::vector<std::string>& tokens);
  void handleIntegral(const std::vector<std::string>& tokens);
  void handleLimit(const std::vector<std::string>& tokens);
  void handleSeries(const std::vector<std::string>& tokens);

  void handlePolynomial(const std::vector<std::string>& tokens);

  void handlePolynomialAdd(const std::vector<std::string>& tokens);

  // Polynomial commands
  // void handlePolynomial(const std::vector<std::string>& tokens);
  void handlePolynomialEvaluate(const std::vector<std::string>& tokens);
  // void handlePolynomialAdd(const std::vector<std::string>& tokens);
  // void handlePolynomialMultiply(const std::vector<std::string>& tokens);
  // void handlePolynomialGCD(const std::vector<std::string>& tokens);
  void handlePolynomialRoots(const std::vector<std::string>& tokens);
  // void handlePolynomialFactor(const std::vector<std::string>& tokens);

  // Complex number commands
  void handleComplex(const std::vector<std::string>& tokens);
  void handleComplexAdd(const std::vector<std::string>& tokens);
  void handleComplexMultiply(const std::vector<std::string>& tokens);
  void handleComplexPower(const std::vector<std::string>& tokens);
  void handleComplexRoots(const std::vector<std::string>& tokens);

  // Matrix commands
  void handleMatrix(const std::vector<std::string>& tokens);
  void handleMatrixRaw(const std::vector<std::string>& tokens);
  void handleMatrixAdjoint(const std::vector<std::string>& tokens);
  void handleMatrixDisplay(const std::vector<std::string>& tokens);
  void handleMatrixRank(const std::vector<std::string>& tokens);
  void handleMatrixSet(const std::vector<std::string>& tokens);
  void handleMatrixDeterminant(const std::vector<std::string>& tokens);
  void handleMatrixInverse(const std::vector<std::string>& tokens);
  void handleMatrixEigenvalues(const std::vector<std::string>& tokens);

  // Transform commands
  void handleLaplace(const std::vector<std::string>& tokens);
  void handleFourier(const std::vector<std::string>& tokens);

  // Differential equation commands
  void handleODE(const std::vector<std::string>& tokens);
  void handlePDE(const std::vector<std::string>& tokens);

  // Numerical methods
  /*  void handleRootFind(const std::vector<std::string>& tokens);
    void handleOptimize(const std::vector<std::string>& tokens);
    void handleInterpolate(const std::vector<std::string>& tokens);
    */
  // Utility commands
  void listObjects();
  void clearAll();

  // std::map<std::string, std::shared_ptr<Expression>> expressions;

  // Parser helper methods
  std::shared_ptr<Expression> parseExpression(const std::string& exprStr);
  std::string removeWhitespace(const std::string& str);
  std::shared_ptr<Expression> parseAddSub(const std::string& expr, size_t& pos);
  std::shared_ptr<Expression> parseMulDiv(const std::string& expr, size_t& pos);
  std::shared_ptr<Expression> parsePower(const std::string& expr, size_t& pos);
  std::shared_ptr<Expression> parseFactor(const std::string& expr, size_t& pos);
  std::shared_ptr<Expression> parseNumber(const std::string& expr, size_t& pos);
  std::shared_ptr<Expression> parseVariableOrFunction(const std::string& expr,
                                                      size_t& pos);

public:
  std::map<std::string, Polynomial> polynomials;
  std::map<std::string, ComplexNumber> complexNumbers;
  std::map<std::string, Matrix<float>> matrices;
  std::map<std::string, std::shared_ptr<Expression>> expressions;
  std::map<std::string, double> variables;
  void run();
};
