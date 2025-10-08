#include <cmath>
#include <cstdlib>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "include/expression.h"

// =================== Constant Implementation ===================
Constant::Constant(double val)
    : value(val)
{
}

std::string Constant::toString() const
{
  if (value == static_cast<int>(value)) {
    return std::to_string(static_cast<int>(value));
  }
  return std::to_string(value);
}

std::shared_ptr<Expression> Constant::derivative(const std::string&) const
{
  return std::make_shared<Constant>(0);
}

std::shared_ptr<Expression> Constant::simplify() const
{
  return std::make_shared<Constant>(value);
}

double Constant::evaluate(const std::map<std::string, double>&) const
{
  return value;
}

Expression::Type Constant::getType() const
{
  return CONSTANT;
}

std::shared_ptr<Expression> Constant::clone() const
{
  return std::make_shared<Constant>(value);
}

// =================== Variable Implementation ===================
Variable::Variable(const std::string& n)
    : name(n)
{
}

std::string Variable::toString() const
{
  return name;
}

std::shared_ptr<Expression> Variable::derivative(const std::string& var) const
{
  if (name == var) {
    return std::make_shared<Constant>(1);
  }
  return std::make_shared<Constant>(0);
}

std::shared_ptr<Expression> Variable::simplify() const
{
  return std::make_shared<Variable>(name);
}

double Variable::evaluate(const std::map<std::string, double>& vars) const
{
  auto it = vars.find(name);
  if (it != vars.end()) {
    return it->second;
  }
  throw std::runtime_error("Variable " + name
                           + " not found in evaluation context");
}

Expression::Type Variable::getType() const
{
  return VARIABLE;
}

std::shared_ptr<Expression> Variable::clone() const
{
  return std::make_shared<Variable>(name);
}

// =================== BinaryOp Implementation ===================
BinaryOp::BinaryOp(std::shared_ptr<Expression> l,
                   char operation,
                   std::shared_ptr<Expression> r)
    : left(l)
    , right(r)
    , op(operation)
{

  // Set the enum based on the character operation
  switch (operation) {
    case '+':
      binaryOp = ADD;
      break;
    case '-':
      binaryOp = SUB;
      break;
    case '*':
      binaryOp = MUL;
      break;
    case '/':
      binaryOp = DIV;
      break;
    case '^':
      binaryOp = POW;
      break;
    default:
      throw std::runtime_error("Unknown binary operation: "
                               + std::string(1, operation));
  }
}

auto BinaryOp::toString() const -> std::string
{
  std::string left_str = left->toString();
  std::string right_str = right->toString();

  if (left->getType() == BINARY_OP) {
    left_str = "(" + left_str + ")";
  }
  if (right->getType() == BINARY_OP) {
    right_str = "(" + right_str + ")";
  }

  return left_str + " " + op + " " + right_str;
}

std::shared_ptr<Expression> BinaryOp::derivative(const std::string& var) const
{
  auto dl = left->derivative(var);
  auto dr = right->derivative(var);

  switch (op) {
    case '+':
      return std::make_shared<BinaryOp>(dl, '+', dr);
    case '-':
      return std::make_shared<BinaryOp>(dl, '-', dr);
    case '*':
      return std::make_shared<BinaryOp>(
          std::make_shared<BinaryOp>(dl, '*', right->clone()),
          '+',
          std::make_shared<BinaryOp>(left->clone(), '*', dr));
    case '/':
      return std::make_shared<BinaryOp>(
          std::make_shared<BinaryOp>(
              std::make_shared<BinaryOp>(dl, '*', right->clone()),
              '-',
              std::make_shared<BinaryOp>(left->clone(), '*', dr)),
          '/',
          std::make_shared<BinaryOp>(right->clone(), '*', right->clone()));
    case '^':
      if (right->getType() == CONSTANT) {
        auto const_right = std::static_pointer_cast<Constant>(right);
        double n = const_right->getValue();
        return std::make_shared<BinaryOp>(
            std::make_shared<BinaryOp>(
                std::make_shared<Constant>(n),
                '*',
                std::make_shared<BinaryOp>(
                    left->clone(), '^', std::make_shared<Constant>(n - 1))),
            '*',
            dl);
      }
      break;
  }
  throw std::runtime_error("Unsupported derivative operation");
}

std::shared_ptr<Expression> BinaryOp::simplify() const
{
  auto s_left = left->simplify();
  auto s_right = right->simplify();

  // Check for constant folding
  if (s_left->getType() == CONSTANT && s_right->getType() == CONSTANT) {
    auto c_left = std::static_pointer_cast<Constant>(s_left);
    auto c_right = std::static_pointer_cast<Constant>(s_right);
    double l_val = c_left->getValue();
    double r_val = c_right->getValue();

    switch (op) {
      case '+':
        return std::make_shared<Constant>(l_val + r_val);
      case '-':
        return std::make_shared<Constant>(l_val - r_val);
      case '*':
        return std::make_shared<Constant>(l_val * r_val);
      case '/':
        if (r_val != 0) {
          return std::make_shared<Constant>(l_val / r_val);
        }
        break;
      case '^':
        return std::make_shared<Constant>(std::pow(l_val, r_val));
    }
  }

  // Algebraic simplifications
  if (s_left->getType() == CONSTANT) {
    auto c_left = std::static_pointer_cast<Constant>(s_left);
    double const l_val = c_left->getValue();

    switch (op) {
      case '+':
        if (l_val == 0) {
          return s_right;  // 0 + x = x
        }
        break;
      case '-':
        if (l_val == 0) {
          // 0 - x = -x (need to implement unary minus or use -1 * x)
          return std::make_shared<BinaryOp>(
              std::make_shared<Constant>(-1), '*', s_right);
        }
        break;
      case '*':
        if (l_val == 0) {
          return std::make_shared<Constant>(0);  // 0 * x = 0
        }
        if (l_val == 1) {
          return s_right;  // 1 * x = x
        }
        break;
      case '/':
        if (l_val == 0) {
          return std::make_shared<Constant>(0);  // 0 / x = 0
        }
        break;
      default:;
    }
  }

  if (s_right->getType() == CONSTANT) {
    auto c_right = std::static_pointer_cast<Constant>(s_right);
    double r_val = c_right->getValue();

    switch (op) {
      case '+':
        if (r_val == 0) {
          return s_left;  // x + 0 = x
        }
        break;
      case '-':
        if (r_val == 0) {
          return s_left;  // x - 0 = x
        }
        break;
      case '*':
        if (r_val == 0) {
          return std::make_shared<Constant>(0);  // x * 0 = 0
        }
        if (r_val == 1) {
          return s_left;  // x * 1 = x
        }
        break;
      case '/':
        if (r_val == 1) {
          return s_left;  // x / 1 = x
        }
        break;
      case '^':
        if (r_val == 0) {
          return std::make_shared<Constant>(1);  // x^0 = 1
        }
        if (r_val == 1) {
          return s_left;  // x^1 = x
        }
        break;
      default:;
    }
  }

  return std::make_shared<BinaryOp>(s_left, op, s_right);
}

double BinaryOp::evaluate(const std::map<std::string, double>& vars) const
{
  double const l_val = left->evaluate(vars);
  double const r_val = right->evaluate(vars);

  switch (op) {
    case '+':
      return l_val + r_val;
    case '-':
      return l_val - r_val;
    case '*':
      return l_val * r_val;
    case '/':
      if (r_val == 0) {
        throw std::runtime_error("Division by zero");
      }
      return l_val / r_val;
    case '^':
      return std::pow(l_val, r_val);
    default:;
  }
  throw std::runtime_error("Unknown binary operation");
}

Expression::Type BinaryOp::getType() const
{
  return BINARY_OP;
}

std::shared_ptr<Expression> BinaryOp::clone() const
{
  return std::make_shared<BinaryOp>(left->clone(), op, right->clone());
}

// =================== Function Implementation ===================
Function::Function(const std::string& fname, std::shared_ptr<Expression> arg)
    : name(fname)
    , argument(arg)
{
}

std::string Function::toString() const
{
  return name + "(" + argument->toString() + ")";
}

std::shared_ptr<Expression> Function::derivative(const std::string& var) const
{
  auto argDeriv = argument->derivative(var);

  if (name == "sin") {
    return std::make_shared<BinaryOp>(
        std::make_shared<Function>("cos", argument->clone()), '*', argDeriv);
  } else if (name == "cos") {
    return std::make_shared<BinaryOp>(
        std::make_shared<BinaryOp>(
            std::make_shared<Constant>(-1),
            '*',
            std::make_shared<Function>("sin", argument->clone())),
        '*',
        argDeriv);
  } else if (name == "tan") {
    return std::make_shared<BinaryOp>(
        std::make_shared<BinaryOp>(
            std::make_shared<Function>("sec", argument->clone()),
            '^',
            std::make_shared<Constant>(2)),
        '*',
        argDeriv);
  } else if (name == "ln" || name == "log") {
    return std::make_shared<BinaryOp>(
        std::make_shared<BinaryOp>(
            std::make_shared<Constant>(1), '/', argument->clone()),
        '*',
        argDeriv);
  } else if (name == "exp") {
    return std::make_shared<BinaryOp>(
        std::make_shared<Function>("exp", argument->clone()), '*', argDeriv);
  } else if (name == "sqrt") {
    return std::make_shared<BinaryOp>(
        std::make_shared<BinaryOp>(
            std::make_shared<Constant>(1),
            '/',
            std::make_shared<BinaryOp>(
                std::make_shared<Constant>(2),
                '*',
                std::make_shared<Function>("sqrt", argument->clone()))),
        '*',
        argDeriv);
  }

  throw std::runtime_error("Derivative not implemented for function: " + name);
}

std::shared_ptr<Expression> Function::simplify() const
{
  auto sArg = argument->simplify();

  // If argument is constant, we can evaluate the function
  if (sArg->getType() == CONSTANT) {
    auto constArg = std::static_pointer_cast<Constant>(sArg);
    double argVal = constArg->getValue();

    try {
      if (name == "sin") {
        return std::make_shared<Constant>(std::sin(argVal));
      }
      if (name == "cos") {
        return std::make_shared<Constant>(std::cos(argVal));
      }
      if (name == "tan") {
        return std::make_shared<Constant>(std::tan(argVal));
      }
      if (name == "exp") {
        return std::make_shared<Constant>(std::exp(argVal));
      }
      if (name == "ln" || name == "log") {
        if (argVal > 0) {
          return std::make_shared<Constant>(std::log(argVal));
        }
      }
      if (name == "sqrt") {
        if (argVal >= 0) {
          return std::make_shared<Constant>(std::sqrt(argVal));
        }
      }
      if (name == "abs") {
        return std::make_shared<Constant>(std::abs(argVal));
      }
      if (name == "floor") {
        return std::make_shared<Constant>(std::floor(argVal));
      }
      if (name == "ceil") {
        return std::make_shared<Constant>(std::ceil(argVal));
      }
    } catch (...) {
      // If evaluation fails, return the original function
    }
  }

  return std::make_shared<Function>(name, sArg);
}

double Function::evaluate(const std::map<std::string, double>& vars) const
{
  double argVal = argument->evaluate(vars);

  if (name == "sin") {
    return std::sin(argVal);
  }
  if (name == "cos") {
    return std::cos(argVal);
  }
  if (name == "tan") {
    return std::tan(argVal);
  }
  if (name == "exp") {
    return std::exp(argVal);
  }
  if (name == "ln" || name == "log") {
    if (argVal <= 0) {
      throw std::runtime_error("Cannot take logarithm of non-positive number");
    }
    return std::log(argVal);
  }
  if (name == "sqrt") {
    if (argVal < 0) {
      throw std::runtime_error("Cannot take square root of negative number");
    }
    return std::sqrt(argVal);
  }
  if (name == "abs") {
    return std::abs(argVal);
  }
  if (name == "floor") {
    return std::floor(argVal);
  }
  if (name == "ceil") {
    return std::ceil(argVal);
  }

  throw std::runtime_error("Unknown function: " + name);
}

Expression::Type Function::getType() const
{
  return FUNCTION;
}

std::shared_ptr<Expression> Function::clone() const
{
  return std::make_shared<Function>(name, argument->clone());
}



// =================== UnaryOpExpression Implementation ===================


UnaryOpExpression::UnaryOpExpression(char op,
                                     std::shared_ptr<Expression> operand)
    : operator_(op)
    , operand_(operand)
{
  if (!operand) {
    throw std::invalid_argument("Operand cannot be null");
  }
}

double UnaryOpExpression::evaluate(
    const std::map<std::string, double>& variables) const
{
  if (!operand_) {
    throw std::runtime_error("Invalid operand in unary expression");
  }

  double operandValue = operand_->evaluate(variables);

  switch (operator_) {
    case '+':
      return operandValue;
    case '-':
      return -operandValue;
    case '!':
      // Factorial (assuming integer input)
      if (operandValue < 0 || operandValue != static_cast<int>(operandValue)) {
        throw std::runtime_error("Factorial requires non-negative integer");
      }
      {
        int n = static_cast<int>(operandValue);
        double result = 1.0;
        for (int i = 2; i <= n; ++i) {
          result *= i;
        }
        return result;
      }
    default:
      throw std::runtime_error("Unknown unary operator: "
                               + std::string(1, operator_));
  }
}

std::string UnaryOpExpression::toString() const
{
  if (!operand_) {
    return "null";
  }

  switch (operator_) {
    case '+':
    case '-':
      return std::string(1, operator_) + "(" + operand_->toString() + ")";
    case '!':
      return "(" + operand_->toString() + ")!";
    default:
      return std::string(1, operator_) + "(" + operand_->toString() + ")";
  }
}

Expression::Type UnaryOpExpression::getType() const
{
  return Expression::Type::UNARY_OP;
}

//std::shared_ptr<Expression> UnaryOpExpression::clone() const
//{
//  if (!operand_) {
//    return nullptr;
//  }
//  return std::make_shared<UnaryOpExpression>(operator_, operand_->clone());
//}

char UnaryOpExpression::getOperator() const
{
  return operator_;
}

std::shared_ptr<Expression> UnaryOpExpression::getOperand() const
{
  return operand_;
}



std::shared_ptr<Expression> UnaryOpExpression::derivative(
    const std::string& var) const
{
  if (!operand_) {
    throw std::runtime_error("Invalid operand in unary expression derivative");
  }

  auto operandDeriv = operand_->derivative(var);

  switch (operator_) {
    case '+':
      // d/dx(+u) = du/dx
      return operandDeriv;
    case '-':
      // d/dx(-u) = -du/dx
      return std::make_shared<UnaryOpExpression>('-', operandDeriv);
    case '!':
      // Factorial derivative is complex and typically not implemented
      // for symbolic computation. You might want to throw an error
      // or implement using gamma function derivatives if needed
      throw std::runtime_error("Derivative of factorial not implemented");
    default:
      throw std::runtime_error("Unknown unary operator for derivative: "
                               + std::string(1, operator_));
  }
}



std::shared_ptr<Expression> UnaryOpExpression::simplify() const
{
  if (!operand_) {
    throw std::runtime_error("Invalid operand in unary expression simplify");
  }

  auto simplifiedOperand = operand_->simplify();

  // Handle constant folding
  if (simplifiedOperand->getType() == CONSTANT) {
    auto constOperand = std::static_pointer_cast<Constant>(simplifiedOperand);
    double val = constOperand->getValue();

    switch (operator_) {
      case '+':
        return std::make_shared<Constant>(val);  // +5 = 5
      case '-':
        return std::make_shared<Constant>(-val);  // -5 = -5
      case '!':
        // Only simplify factorial for small non-negative integers
        if (val >= 0 && val == static_cast<int>(val) && val <= 20) {
          int n = static_cast<int>(val);
          double result = 1.0;
          for (int i = 2; i <= n; ++i) {
            result *= i;
          }
          return std::make_shared<Constant>(result);
        }
        break;
    }
  }

  // Handle algebraic simplifications
  switch (operator_) {
    case '+':
      // +x = x (unary plus is redundant)
      return simplifiedOperand;
    case '-':
      // Check for double negation: -(-x) = x
      if (simplifiedOperand->getType() == UNARY_OP) {
        auto unaryOperand =
            std::static_pointer_cast<UnaryOpExpression>(simplifiedOperand);
        if (unaryOperand->getOperator() == '-') {
          return unaryOperand->getOperand()->simplify();
        }
      }
      break;
  }

  return std::make_shared<UnaryOpExpression>(operator_, simplifiedOperand);
}
std::shared_ptr<Expression> UnaryOpExpression::clone() const
{
  if (!operand_) {
    return nullptr;
  }
  return std::make_shared<UnaryOpExpression>(operator_, operand_->clone());
}