#include "calc_model.h"

/// @brief Конструктор
s21::CalcModel::CalcModel() : status_(kOk), x_(0.0), result_(0.0), infix_("") {}

/// @brief Деструктор
s21::CalcModel::~CalcModel() {}

/// @brief Получаем значения из модели
s21::Status s21::CalcModel::GetStatus() const { return status_; }
long double s21::CalcModel::GetResult() const { return result_; }

std::string s21::CalcModel::ErrorPrint() {
  switch (status_) {
    case kBadBalanceBrackets:
      return "Неправильный баланс скобок";
    case kEmpty:
      return "Выражение пустое";
    case kTooLong:
      return "Выражение слишком длинное";
    case kTooShirt:
      return "Выражение слишком короткое";
    case kBadExpression:
      return "Некорректное выражение";
    case kDivisionByZero:
      return "Ошибка деления на 0";
    case kCalculationError:
      return "Ошибка вычислений";
    default:
      return "Вычисление успешно завершено";
  }
}

/// @brief Меняем подстроку в строке
/// @param inputStr - входящая строка
/// @param src - подстрока для поиска
/// @param dst - подстрока на которую меняем
/// @return строка с изменениями
std::string s21::CalcModel::StringReplacer(const std::string& inputStr,
                                           const std::string& src,
                                           const std::string& dst) {
  std::string result(inputStr);
  size_t pos = result.find(src);
  while (pos != std::string::npos) {
    result.replace(pos, src.size(), dst);
    pos = result.find(src, pos);
  }
  return result;
}

/// @brief Проверка баланаса скобок
/// @param inputStr проверяемая строка
/// @return истина, если баланс скобок правильный и наоборот
bool s21::CalcModel::CheckBalanceBrackets(const std::string& inputStr) {
  int count = 0;
  bool flag = true;
  for (const auto& c : inputStr) {
    if (c == '(') {
      ++count;
    } else if (c == ')') {
      --count;
    }
    if (count < 0) {
      flag = false;
      break;
    }
  }
  if (count != 0) flag = false;
  return flag;
}

/// @brief Проверка строки на пустоту
/// @param inputStr входная строка
/// @return истина, если строка пуста
bool s21::CalcModel::StrIsEmpty(const std::string& inputStr) {
  return inputStr.empty();
}

/// @brief Проверяем строку на минимальный и максимальный размер
/// @param inputStr входящая строка
void s21::CalcModel::CheckSizeExpression(const std::string& inputStr) {
  if (inputStr.length() < 2) {
    status_ = kTooShirt;
  } else {
    if (inputStr.length() > 255) {
      status_ = kTooLong;
    }
  }
}

/// @brief Проверка корректности точек в выражении
void s21::CalcModel::CorrectPoint(const std::string& inputStr) {
  if (inputStr.length() >= 2) {
    if ((inputStr.at(0) == '.')) {
      status_ = kBadExpression;
    } else {
      for (size_t i = 0; i < inputStr.length() - 1; ++i) {
        if ((inputStr.at(i) == '.') && (inputStr.at(i + 1) == '.')) {
          status_ = kBadExpression;
          break;
        } else {
          if (inputStr.length() > 3) {
            for (size_t j = 1; j < inputStr.length() - 1; ++j) {
              if ((inputStr.at(j) == '.') && (!(isdigit(inputStr.at(j - 1))) ||
                                              !(isdigit(inputStr.at(j + 1))))) {
                status_ = kBadExpression;
                break;
              }
            }
          }
        }
      }
    }
  }
}

/// @brief Проверка последнего символа на скобку или число
/// @param inputStr исходная строка
void s21::CalcModel::LastSymbol(const std::string& inputStr) {
  if (status_ == kOk) {
    char ch = inputStr.at(inputStr.length() - 1);
    if ((ch != ')') && (!isdigit(ch)) && (ch != 'x')) {
      status_ = kBadExpression;
    }
  }
}

/// @brief установливаем значения полей модели
void s21::CalcModel::SetX(const long double x) noexcept { x_ = x; }
void s21::CalcModel::SetInfix(const std::string& infix) noexcept {
  infix_ = infix;

  // Меняем экспонетциальную запись, запятые на точки и mod
  infix_ = s21::CalcModel::StringReplacer(infix_, "e-", "/10^");
  infix_ = s21::CalcModel::StringReplacer(infix_, "e+", "*10^");
  infix_ = s21::CalcModel::StringReplacer(infix_, "e", "*10^");
  infix_ = s21::CalcModel::StringReplacer(infix_, ",", ".");
  infix_ = s21::CalcModel::StringReplacer(infix_, "mod", "%");

  // Проверка баланса скобок
  if (!s21::CalcModel::CheckBalanceBrackets(infix_)) {
    status_ = kBadBalanceBrackets;
  }

  // Проверка на минимальный и максимальный размер
  s21::CalcModel::CheckSizeExpression(infix_);

  // Проверка на пустоту
  if (s21::CalcModel::StrIsEmpty(infix_)) {
    status_ = kEmpty;
  }

  // Проверка точек и последний символ
  s21::CalcModel::CorrectPoint(infix_);
  s21::CalcModel::LastSymbol(infix_);
}

// Блок функций на проверку числа, функции, оператора, степени
bool s21::CalcModel::isNumber(const std::string::iterator& it) noexcept {
  return isdigit(*it) || *it == '.' || *it == ',' || *it == 'e' || *it == 'x' ||
         ((*it == '-' || *it == '+') &&
          (it != infix_.begin() && *(it - 1) == 'e'));
}

bool s21::CalcModel::isUnar(const std::string::iterator& it) noexcept {
  return (*it == '-' || *it == '+') &&
         (it == infix_.begin() || *(it - 1) == '(');
}

bool s21::CalcModel::isBrackets(const std::string::iterator& it) noexcept {
  return *it == '(' || *it == ')';
}

bool s21::CalcModel::isOperators(const std::string::iterator& it) noexcept {
  return *it == '-' || *it == '+' || *it == '*' || *it == '/' || *it == '%' ||
         *it == '^';
}

bool s21::CalcModel::isFunction(std::string::iterator& it) noexcept {
  char ch = ChangeFunctions(it, false);
  return ch >= 65 && ch <= 73 && *it != ch;
}

bool s21::CalcModel::isPow(const std::string::iterator& it) noexcept {
  return *it == '^';
}

char s21::CalcModel::ChangeFunctions(std::string::iterator& it, bool iter) {
  std::vector<std::string> func = {"cos",  "sin",  "tan", "acos", "asin",
                                   "atan", "sqrt", "log", "ln"};
  for (int i = 0; i < (int)func.size(); ++i) {
    if (std::equal(it, it + func[i].size(), func[i].begin(), func[i].end())) {
      if (iter) it += func[i].length() - 1;
      return i + 65;
    }
  }
  return *it;
}

bool s21::CalcModel::isValid() {
  if (status_ == kOk) {
    return true;
  } else {
    return false;
  }
}

// Приоритет операций
int s21::CalcModel::Priority(char ch) noexcept {
  int flag = 0;
  if (ch == '+' || ch == '-')
    flag = 1;
  else if (ch == '*' || ch == '/')
    flag = 2;
  else if (ch == '^')
    flag = 3;
  else if ((ch >= 65 && ch <= 74) || ch == '%')
    flag = 4;
  else if (ch == '~' || ch == '&')
    flag = 5;
  else if (ch == '(')
    flag = -1;
  return flag;
}

void s21::CalcModel::PushBackPostfix() {
  if (!stack_.empty()) {
    postfix_.push_back(stack_.top());
    stack_.pop();
    postfix_.push_back(' ');
  }
}

// Блок функций перевода в посфикс
void s21::CalcModel::NumberToPostfix(std::string::iterator& it) {
  while (isNumber(it)) {
    postfix_.push_back(*it++);
  }
  *it--;
  postfix_.push_back(' ');
}

void s21::CalcModel::UnarToPostfix(const std::string::iterator& it) {
  char ch = *it == '+' ? '&' : '~';
  while (!stack_.empty() && Priority(ch) <= Priority(stack_.top()))
    PushBackPostfix();
  stack_.push(ch);
}

void s21::CalcModel::BracketsToPostfix(const std::string::iterator& it) {
  if (*it == '(')
    stack_.push(*it);
  else {
    while (!stack_.empty() && stack_.top() != '(') {
      PushBackPostfix();
    }
    if (!stack_.empty() && stack_.top() == '(')
      stack_.pop();
    else
      status_ = kBadExpression;
  }
}

void s21::CalcModel::PowToPostfix(const std::string::iterator& it) {
  while (!stack_.empty() && Priority(*it) < Priority(stack_.top()))
    PushBackPostfix();
  stack_.push(*it);
}

void s21::CalcModel::FunctionToPostfix(std::string::iterator& it) {
  char func_name = ChangeFunctions(it, true);
  while (!stack_.empty() && Priority(func_name) <= Priority(stack_.top()))
    PushBackPostfix();
  stack_.push(func_name);
}

// Установка дефолтных значений
void s21::CalcModel::SetToDefault() {
  postfix_ = "";
  status_ = kOk;
  result_ = 0.0;
  while (!stack_.empty()) stack_.pop();
}

// Основные вычисление
s21::Status s21::CalcModel::InfixToPostfix() {
  if (!isValid()) return kBadExpression;
  SetToDefault();
  for (auto it = infix_.begin(); it != infix_.end(); ++it) {
    if (isNumber(it)) {
      NumberToPostfix(it);
    } else if (isUnar(it)) {
      UnarToPostfix(it);
    } else if (isBrackets(it)) {
      BracketsToPostfix(it);
    } else if (isFunction(it)) {
      FunctionToPostfix(it);
    } else if (isPow(it)) {
      PowToPostfix(it);
    } else if (isOperators(it)) {
      while (!stack_.empty() && Priority(*it) <= Priority(stack_.top()))
        PushBackPostfix();
      stack_.push(*it);
    } else
      status_ = kBadExpression;
  }
  while (!stack_.empty()) PushBackPostfix();
  if (status_ != kOk) postfix_ = "";
  return status_;
}

void s21::CalcModel::NumberToStack(std::string::iterator& it) {
  if (*it == 'x') {
    stack_.push(x_);
  } else {
    std::string digit;
    while (isNumber(it)) digit.push_back(*it++);
    *it--;
    try {
      stack_.push(std::stod(digit));
    } catch (const std::out_of_range& e) {
      status_ = kBadExpression;
    }
  }
}

void s21::CalcModel::CalcOperand(const std::string::iterator& it) {
  long double operand_two = 0, operand_one = 0;
  if (stack_.empty()) {
    status_ = kBadExpression;
  } else {
    operand_two = stack_.top();
    stack_.pop();
  }
  if (stack_.empty()) {
    status_ = kBadExpression;
  } else {
    operand_one = stack_.top();
    stack_.pop();
  }
  if (*it == '+') {
    stack_.push(operand_one + operand_two);
  } else if (*it == '-') {
    stack_.push(operand_one - operand_two);
  } else if (*it == '*') {
    stack_.push(operand_one * operand_two);
  } else if (*it == '^') {
    stack_.push(std::pow(operand_one, operand_two));
  } else if (*it == '/') {
    if (operand_two != 0.0)
      stack_.push(operand_one / operand_two);
    else
      status_ = kDivisionByZero;
  } else if (*it == '%') {
    stack_.push(std::fmod(operand_one, operand_two));
  }
}

void s21::CalcModel::CalcFunc(const std::string::iterator& it) {
  long double operand = 0.0;
  if (stack_.empty()) {
    status_ = kBadExpression;
  } else {
    operand = stack_.top();
    stack_.pop();
  }
  if (*it == 'A') {
    stack_.push(std::cos(operand));
  } else if (*it == 'B') {
    stack_.push(std::sin(operand));
  } else if (*it == 'C') {
    stack_.push(std::tan(operand));
  } else if (*it == 'D') {
    stack_.push(std::acos(operand));
  } else if (*it == 'E') {
    stack_.push(std::asin(operand));
  } else if (*it == 'F') {
    stack_.push(std::atan(operand));
  } else if (*it == 'G') {
    if (operand >= 0) {
      stack_.push(std::sqrt(operand));
    } else
      status_ = kCalculationError;
  } else if (*it == 'H') {
    if (operand > 0) {
      stack_.push(std::log10(operand));
    } else
      status_ = kCalculationError;
  } else if (*it == 'I') {
    if (operand > 0) {
      stack_.push(std::log(operand));
    } else
      status_ = kCalculationError;
  } else if (*it == '~') {
    stack_.push(operand * -1);
  } else if (*it == '&') {
    stack_.push(operand);
  }
}

void s21::CalcModel::Calculate() {
  if (InfixToPostfix() != kBadExpression && status_ == kOk) {
    for (auto it = postfix_.begin(); it != postfix_.end(); ++it) {
      if (*it == ' ') {
        continue;
      } else if (isNumber(it)) {
        NumberToStack(it);
      } else if (isOperators(it)) {
        CalcOperand(it);
      } else {
        CalcFunc(it);
      }
      if (status_ == kOk && !stack_.empty()) result_ = stack_.top();
    }
  }
  if (status_ == kOk && !stack_.empty()) {
    stack_.pop();
  }

  if (!stack_.empty()) status_ = kBadExpression;
}