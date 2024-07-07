#ifndef DEPOZIT_CALC_H
#define DEPOZIT_CALC_H

#include <QDialog>

namespace Ui {
class depozit_calc;
}

class depozit_calc : public QDialog {
  Q_OBJECT

 public:
  explicit depozit_calc(QWidget *parent = nullptr);
  ~depozit_calc();

 private slots:
  void on_pushButton_clicked();

 private:
  Ui::depozit_calc *ui;
};

#endif  // DEPOZIT_CALC_H
