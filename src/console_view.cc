#include <iostream>
#include <regex>
#include <string_view>

#include "controller/calc_controller.h"
#include "controller/credit_controller.h"
#include "model/calc_model.h"
#include "model/credit_model.h"

int main() {
  std::cout << "---- Console view for Calc----\n";
  s21::CalcModel str;
  s21::CalcController controller(&str);

  // установить значения выражения
  long double x = 1;
  std::string input_str = "+()";

  // Ответ для сравнения
  long double answer = x - 2 + sin(x);

  // вывод результатов
  std::cout << "Origin input str = " << input_str << std::endl;

  controller.SetInput(input_str);
  controller.ModelSetExpression(x);
  std::cout << "Infix from Model = " << str.GetInfix() << std::endl;

  controller.ModelCalculate();
  std::cout << "Result from model = " << str.GetResult() << std::endl;
  std::cout << "Etalon answer = " << answer << std::endl;
  std::cout << "model status from controller = " << controller.ModelGetStatus()
            << " (" << str.ErrorPrint() << ")" << std::endl;
  std::cout << std::endl;

  std::cout << "---- Console view for CreditCalc----\n";
  s21::CreditInputData input;

  // установка значений
  input.credit_amount = 1000000;
  input.credit_term = 13;
  input.credit_term_type = 1;
  input.credit_rate = 23.13;
  input.credit_payment_type = "Annuities";

  // вычисления
  s21::CreditController credit_controller;
  credit_controller.SetInputData(input);
  credit_controller.Calculate();

  // вывод результатов
  std::cout << "month payment = "
            << credit_controller.GetOutputData().month_payment[0] << std::endl;
  std::cout << "over payment = "
            << credit_controller.GetOutputData().over_payment << std::endl;
  std::cout << "total payment = "
            << credit_controller.GetOutputData().total_payment << std::endl;
  return 0;
}