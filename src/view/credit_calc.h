#ifndef CREDIT_CALC_H
#define CREDIT_CALC_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtMath>

#include "../controller/credit_controller.h"
#include "../model/credit_model.h"

namespace Ui {
class credit_calc;
}

class credit_calc : public QDialog {
  Q_OBJECT

 public:
  explicit credit_calc(QWidget *parent = nullptr);
  ~credit_calc();

 private slots:

  void on_comboBox_term_currentIndexChanged(int index);

  void on_pushButton_clicked();

 private:
  Ui::credit_calc *ui;
};

#endif  // CREDIT_CALC_H
