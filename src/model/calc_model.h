#ifndef CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALC_MODEL_H_
#define CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALC_MODEL_H_

#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

namespace s21 {
typedef enum {
  kOk,
  kBadExpression,
  kBadBalanceBrackets,
  kEmpty,
  kTooShirt,
  kTooLong,
  kDivisionByZero,
  kCalculationError
} Status;

class CalcModel {
 private:
  Status status_;
  long double x_;
  long double result_;
  std::string infix_;
  std::string postfix_;
  std::stack<long double> stack_;

  std::string StringReplacer(const std::string& inputStr,
                             const std::string& src, const std::string& dst);

  bool CheckBalanceBrackets(const std::string& inputStr);
  bool StrIsEmpty(const std::string& inputStr);
  void CheckSizeExpression(const std::string& inputStr);
  void CorrectPoint(const std::string& inputStr);
  void LastSymbol(const std::string& inputStr);
  bool isNumber(const std::string::iterator& it) noexcept;
  bool isUnar(const std::string::iterator& it) noexcept;
  bool isBrackets(const std::string::iterator& it) noexcept;
  bool isOperators(const std::string::iterator& it) noexcept;
  bool isFunction(std::string::iterator& it) noexcept;
  bool isPow(const std::string::iterator& it) noexcept;
  char ChangeFunctions(std::string::iterator& it, bool iter);
  bool isValid();
  int Priority(char ch) noexcept;
  void PushBackPostfix();
  void NumberToPostfix(std::string::iterator& it);
  void UnarToPostfix(const std::string::iterator& it);
  void BracketsToPostfix(const std::string::iterator& it);
  void PowToPostfix(const std::string::iterator& it);
  void FunctionToPostfix(std::string::iterator& it);
  void SetToDefault();
  Status InfixToPostfix();
  void NumberToStack(std::string::iterator& it);
  void CalcOperand(const std::string::iterator& it);
  void CalcFunc(const std::string::iterator& it);

 public:
  CalcModel();
  ~CalcModel();

  Status GetStatus() const;
  long double GetResult() const;
  void SetX(const long double x) noexcept;
  void SetInfix(const std::string& infix) noexcept;
  void Calculate();
  std::string ErrorPrint();

  // tmp func for testing
  std::string GetInfix() const { return infix_; };
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_MODEL_CALC_MODEL_H_