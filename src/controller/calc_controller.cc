#include "calc_controller.h"

#include <algorithm>
#include <iostream>

/// @brief Контсруктор
/// @param объект модели
s21::CalcController::CalcController(s21::CalcModel* m)
    : model_(m), input_(""){};

/// @brief Деструктор
s21::CalcController::~CalcController(){};

/// @brief Получить статус из модели
/// @return Код статуса
s21::Status s21::CalcController::ModelGetStatus() const {
  return model_->GetStatus();
}

/// @brief Получить результат из модели
/// @return результат вычисления
long double s21::CalcController::ModelGetResult() const {
  return model_->GetResult();
}

/// @brief Перевод входящей сроки в нижний регистр
void s21::CalcController::InputToLower() {
  std::transform(input_.begin(), input_.end(), input_.begin(), tolower);
}

/// @brief Удаление пробелов из входящей строки
void s21::CalcController::DeleteteSpaces() {
  input_.erase(std::remove(input_.begin(), input_.end(), ' '), input_.end());
}

/// @brief Установка значения входящей строки и первичная нормализация
/// @param input - входящая строка
void s21::CalcController::SetInput(const std::string& input) noexcept {
  input_ = input;
  s21::CalcController::InputToLower();
  s21::CalcController::DeleteteSpaces();
}

/// @brief установка в модель обработанной входящей строки
/// @param x - значение x
void s21::CalcController::ModelSetExpression(const long double x) noexcept {
  model_->SetInfix(input_);
  model_->SetX(x);
}

/// @brief Вычисление
void s21::CalcController::ModelCalculate() { model_->Calculate(); }