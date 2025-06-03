#pragma once
#include <map>
#include <memory>
#include <string>

class Expression
{
public:
  enum Type
  {
    CONSTANT,
    VARIABLE,
    BINARY_OP,
    UNARY_OP,
    FUNCTION
  };

  virtual std::string toString() const = 0;
  virtual std::shared_ptr<Expression> derivative(
      const std::string& var) const = 0;
  virtual std::shared_ptr<Expression> simplify() const = 0;
  virtual double evaluate(
      const std::map<std::string, double>& vars = {}) const = 0;
  virtual Type getType() const = 0;
  virtual std::shared_ptr<Expression> clone() const = 0;

  virtual ~Expression() = default;
};

class Constant : public Expression
{
private:
  double value;

public:
  Constant(double val);
  std::string toString() const override;
  std::shared_ptr<Expression> derivative(const std::string& var) const override;
  std::shared_ptr<Expression> simplify() const override;
  double evaluate(
      const std::map<std::string, double>& vars = {}) const override;
  Type getType() const override;
  std::shared_ptr<Expression> clone() const override;

  double getValue() const { return value; }
};

class Variable : public Expression
{
private:
  std::string name;

public:
  Variable(const std::string& n);
  std::string toString() const override;
  std::shared_ptr<Expression> derivative(const std::string& var) const override;
  std::shared_ptr<Expression> simplify() const override;
  double evaluate(
      const std::map<std::string, double>& vars = {}) const override;
  Type getType() const override;
  std::shared_ptr<Expression> clone() const override;

  std::string getName() const { return name; }
};

class BinaryOp : public Expression
{
public:
  enum TypeBinaryOp
  {
    ADD,
    SUB,
    MUL,
    DIV,
    POW
  };

private:
  std::shared_ptr<Expression> left;
  std::shared_ptr<Expression> right;
  char op;
 TypeBinaryOp binaryOp; 

public:
  

  BinaryOp(std::shared_ptr<Expression> l,
           char operation,
           std::shared_ptr<Expression>r);
  std::string toString() const override;
  std::shared_ptr<Expression> derivative(const std::string& var) const override;

 
  std::shared_ptr<Expression> simplify() const override;
  double evaluate(
      const std::map<std::string, double>& vars = {}) const override;
  Type getType() const override;
  std::shared_ptr<Expression> clone() const override;
  std::shared_ptr<Expression> getLeft() const { return left; }
  std::shared_ptr<Expression> getRight() const { return right; }

  char getOperator() const { return op; }


};

class Function : public Expression
{
private:
  std::string name;
  std::shared_ptr<Expression> argument;

public:
  Function(const std::string& fname, std::shared_ptr<Expression> arg);
  std::string toString() const override;
  std::shared_ptr<Expression> derivative(const std::string& var) const override;
  std::shared_ptr<Expression> simplify() const override;
  double evaluate(
      const std::map<std::string, double>& vars = {}) const override;
  Type getType() const override;
  std::shared_ptr<Expression> clone() const override;

  std::string getFunctionName() const { return name; }

  std::shared_ptr<Expression> getArgument() const { return argument; }
};




class UnaryOpExpression : public Expression
{
private:
  char operator_;
  std::shared_ptr<Expression> operand_;

public:
  // Constructor
  UnaryOpExpression(char op, std::shared_ptr<Expression> operand);

  // Destructor
  virtual ~UnaryOpExpression() = default;

  // Override pure virtual methods from Expression
  virtual double evaluate(
      const std::map<std::string, double>& variables) const override;
  virtual std::string toString() const override;
  virtual Expression::Type getType() const override;
  virtual std::shared_ptr<Expression> clone() const override;
  virtual std::shared_ptr<Expression> derivative(const std::string& var) const override;
  virtual std::shared_ptr<Expression> simplify() const override;
  // Getters
  char getOperator() const;
  std::shared_ptr<Expression> getOperand() const;

};
