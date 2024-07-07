#include <gtest/gtest.h>

#include <iostream>
#include <limits>

#include "../controller/calc_controller.h"
#include "../controller/credit_controller.h"
#include "../model/calc_model.h"
#include "../model/credit_model.h"

TEST(Model, X) {
  s21::CalcModel str;
  str.SetX(6.00005);
  EXPECT_DOUBLE_EQ(6.00005, 6.00005);
}

TEST(Model, Get_Status_and_Result) {
  s21::CalcModel str;
  s21::Status code;
  code = s21::kOk;
  EXPECT_EQ(code, str.GetStatus());
  long double res = str.GetResult();
  EXPECT_DOUBLE_EQ(res, 0.0);
}

TEST(Model, Infix) {
  s21::CalcModel str;
  str.SetInfix("test");
  EXPECT_EQ("test", "test");
}

TEST(Model, Errorcode) {
  s21::CalcModel str;
  str.SetInfix("x+2");
  EXPECT_EQ(str.ErrorPrint(), "Вычисление успешно завершено");

  str.SetInfix(")x+2");
  EXPECT_EQ(str.ErrorPrint(), "Неправильный баланс скобок");

  str.SetInfix("");
  EXPECT_EQ(str.ErrorPrint(), "Выражение пустое");

  str.SetInfix(
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "fffffffffffff");
  EXPECT_EQ(str.ErrorPrint(), "Выражение слишком длинное");

  str.SetInfix("3");
  EXPECT_EQ(str.ErrorPrint(), "Выражение слишком короткое");

  str.SetInfix(".3");
  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");

  str.SetInfix("..3");
  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
  str.SetInfix("5..3:");
  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
  str.SetInfix("5.3.3a.a");
  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Controller, GetFromModel) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  EXPECT_DOUBLE_EQ(contr.ModelGetStatus(), str.GetStatus());
  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), str.GetResult());
}

TEST(Controller, SetToModel) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  contr.SetInput("(test 1e-17  )");
  contr.ModelSetExpression(3.13);
  contr.SetInput(")(test 1e-17  )");
  contr.ModelSetExpression(3.13);
  contr.SetInput(" ");
  contr.ModelSetExpression(3.13);
  contr.SetInput(
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "fffffffffffff");
  contr.ModelSetExpression(3.13);
  EXPECT_EQ("test", "test");
}

// Тесты на вычисления
TEST(Calculate, test1) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  std::string input_str = "(8+2*5)/(1+3*2-4)";
  long double answer = (8 + 2 * 5) / (1 + 3 * 2 - 4);

  contr.SetInput(input_str);
  long double x = 3.14159265359;
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test2) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  std::string input_str = "2/(3+2)*5";
  long double answer = 2.0 / (3 + 2) * 5;

  contr.SetInput(input_str);
  long double x = 3.14159265359;
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test3) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  std::string input_str = "2/(3+2)*5^0";
  long double answer = 2.0 / (3 + 2) * powf(5, 0);

  contr.SetInput(input_str);
  long double x = 3.14159265359;
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test4) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  std::string input_str = "2/(3+2)*5mod1";
  long double answer = 2.0 / (3 + 2) * fmod(5, 1);

  contr.SetInput(input_str);
  long double x = 3.14159265359;
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test5) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  std::string input_str = "9.99999+9.999999*9.999";
  long double answer = 9.99999 + 9.999999 * 9.999;

  contr.SetInput(input_str);
  long double x = 3.14159265359;
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test6) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  std::string input_str = "((1) - 2 - ((-3) - (4))) - 5 - (-6)";
  long double answer = ((1) - 2 - ((-3) - (4))) - 5 - (-6);

  contr.SetInput(input_str);
  long double x = 3.14159265359;
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test7) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 4.0;
  std::string input_str = "+5-(+4)-x+((+x)-x-(-x))";
  long double answer = +5.0 - (+4.0) - x + ((+x) - x - (-x));

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test8) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -1234567.1234567;
  std::string input_str = "-x-(-x)";
  long double answer = -x - (-x);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test9) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 10.1;
  std::string input_str = "(-5) mod (-3)";
  long double answer = fmod(-5, -3);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test10) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 10.1;
  std::string input_str = "2^3^4";
  long double answer = powl(2.0, 81.0);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test11) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 10.1;
  std::string input_str = "1.0/2.0*(2.0-1.0)";
  long double answer = 1.0 / 2.0 * (2.0 - 1.0);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test12) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 10.1;
  std::string input_str = "1.0/2.0*(2.0-1.0)(x)";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Calculate, test13) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 10.1;
  std::string input_str = "1.1 + 2.0 + (3 * 4.0) + (5 + 6.78)";
  long double answer = 1.1 + 2.0 + (3 * 4.0) + (5 + 6.78);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test14) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -3;
  std::string input_str = "8^(1/(-x))";
  long double answer = 2.0;

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test15) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 1;
  std::string input_str = "asin(-x+1.5)+acos(-x)-atan(x+1)";
  long double answer = asin(-x + 1.5) + acos(-x) - atan(x + 1);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test16) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 100;
  std::string input_str = "log(x)+ln(-2.0+4.0)";
  long double answer = log10(x) + log(-2.0 + 4.0);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test17) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "cos(x)";
  long double answer = cos(x);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test18) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI / 2.0;
  std::string input_str = "-1*sin(cos(tan(-x^2)^3)^4)";
  long double answer = -1.0 * sinf(powf(cosf(powf(tanf(powf(-x, 2)), 3)), 4));

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_NEAR(contr.ModelGetResult(), answer, 0.000001);
}

TEST(Calculate, test19) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "+x";
  long double answer = +x;

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test20) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "-x";
  long double answer = -x;

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test21) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "xmod(x+1.0)";
  long double answer = fmod(x, x + 1.0);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test22) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "(-1 + (-1 - (1 + 1)) - (-1 + 1) - (-1 + (-1)))";
  long double answer = (-1 + (-1 - (1 + 1)) - (-1 + 1) - (-1 + (-1)));

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test23) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "sqrt(25)";
  long double answer = sqrt(25);

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(Calculate, test24) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "sqrt(x)";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Ошибка вычислений");
}

TEST(Calculate, test25) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 0;
  std::string input_str = "sqrt(x) / x";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Ошибка деления на 0");
}

TEST(Calculate, test26) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = 0;
  std::string input_str = "log(x)";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Ошибка вычислений");
}

TEST(Calculate, test27) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "ln(x)";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Ошибка вычислений");
}

TEST(Calculate, test28) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "55+55@(4+4)";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Calculate, test29) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "654+";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Calculate, test30) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "=1e+06";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Calculate, test31) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "1--1";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Calculate, test32) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Выражение пустое");
}

TEST(Calculate, test33) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = -2;
  std::string input_str = "+()";

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_EQ(str.ErrorPrint(), "Некорректное выражение");
}

TEST(Calculate, test34) {
  s21::CalcModel str;
  s21::CalcController contr(&str);
  long double x = M_PI;
  std::string input_str = "sqrt(25)      + 2    -2";
  long double answer = sqrt(25) + 2 - 2;

  contr.SetInput(input_str);
  contr.ModelSetExpression(x);
  contr.ModelCalculate();

  EXPECT_DOUBLE_EQ(contr.ModelGetResult(), answer);
}

TEST(CreditCalc, test1) {
  s21::CreditInputData input;
  input.credit_amount = 1000000;
  input.credit_term = 13;
  input.credit_term_type = 1;
  input.credit_rate = 23.13;
  input.credit_payment_type = "Annuities";

  s21::CreditController controller;
  controller.SetInputData(input);
  controller.Calculate();

  EXPECT_NEAR(controller.GetOutputData().month_payment[0], 87697.81, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().over_payment, 140071.55, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().total_payment, 1140071.55, 1e-2);
}

TEST(CreditCalc, test2) {
  s21::CreditInputData input;
  input.credit_amount = 123456;
  input.credit_term = 6;
  input.credit_term_type = 1;
  input.credit_rate = 14.32;
  input.credit_payment_type = "Differentiated";

  s21::CreditController controller;
  controller.SetInputData(input);
  controller.Calculate();

  EXPECT_NEAR(controller.GetOutputData().month_payment[0], 22049.24, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().month_payment[1], 21803.70, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().month_payment[2], 21558.16, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().month_payment[3], 21312.62, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().month_payment[4], 21067.08, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().month_payment[5], 20821.54, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().over_payment, 5156.35, 1e-2);
  EXPECT_NEAR(controller.GetOutputData().total_payment, 128612.35, 1e-2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
