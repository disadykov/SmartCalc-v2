#ifndef CPP3_SMARTCALC_V2_0_1_SRC_CONTROLLER_CALC_CONTROLLER_H_
#define CPP3_SMARTCALC_V2_0_1_SRC_CONTROLLER_CALC_CONTROLLER_H_

#include <stack>
#include <string>

#include "../model/calc_model.h"

namespace s21 {
class CalcController {
 public:
  explicit CalcController(s21::CalcModel* m);
  ~CalcController();

  Status ModelGetStatus() const;
  void SetInput(const std::string& input) noexcept;
  void ModelSetExpression(const long double x) noexcept;
  long double ModelGetResult() const;
  void ModelCalculate();

 private:
  CalcModel* model_;
  std::string input_;

  void InputToLower();
  void DeleteteSpaces();
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_SRC_CONTROLLER_CALC_CONTROLLER_H_