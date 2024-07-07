#include "credit_calc.h"

#include "ui_credit_calc.h"

credit_calc::credit_calc(QWidget *parent)
    : QDialog(parent), ui(new Ui::credit_calc) {
  ui->setupUi(this);
  ui->spinBox_term->setMaximum(1188);
}

credit_calc::~credit_calc() { delete ui; }

void add_table(double d, QStandardItemModel *model, int i, int j) {
  QStandardItem *item = new QStandardItem(QString::number(d, 'f', 2));
  model->setItem(i, j, item);
}

void credit_calc::on_comboBox_term_currentIndexChanged(int index) {
  if (index == 0) {
    ui->spinBox_term->setMaximum(1188);
  } else {
    ui->spinBox_term->setMaximum(99);
  }
}

void credit_calc::on_pushButton_clicked() {
  QStandardItemModel *model = new QStandardItemModel();
  QStringList labels = QObject::tr(
                           "Ежемесячный платеж;Оплата основного долга;Оплата "
                           "процентов;Остаток задолженности")
                           .simplified()
                           .split(";");
  model->setHorizontalHeaderLabels(labels);

  s21::CreditInputData input;
  input.credit_amount = ui->doubleSpinBox_credit_sum->value();
  input.credit_term = ui->spinBox_term->value();
  input.credit_term_type = 1;
  input.credit_rate = ui->doubleSpinBox_bid->value();

  if (ui->comboBox_term->currentText() != "В месяцах") {
    input.credit_term = input.credit_term * 12;
  }
  s21::CreditController credit_controller;

  double debt_balance = input.credit_amount;
  double bid = (input.credit_rate / 12) / 100;

  if (ui->radioButton_annuity->isChecked()) {
    input.credit_payment_type = "Annuities";

    credit_controller.SetInputData(input);
    credit_controller.Calculate();

    double sum_payment =
        roundf(input.credit_amount *
               (bid / (1 - pow(1 + bid, -input.credit_term))) * 100) /
        100;

    for (int i = 0; i < input.credit_term; i++) {
      double overpayment = debt_balance * bid;
      double payment = roundf((sum_payment - overpayment) * 100) / 100;
      debt_balance -= payment;
      add_table(credit_controller.GetOutputData().month_payment[0], model, i,
                0);
      add_table(payment, model, i, 1);
      add_table(overpayment, model, i, 2);
      if (i == (input.credit_term - 1)) {
        add_table(roundf(fabs(debt_balance)), model, i, 3);
      } else {
        add_table(fabs(debt_balance), model, i, 3);
      }
    }
  }
  if (ui->radioButton_different->isChecked()) {
    input.credit_payment_type = "Differentiated";

    credit_controller.SetInputData(input);
    credit_controller.Calculate();

    double payment = input.credit_amount / input.credit_term;

    for (int i = 0; i < input.credit_term; i++) {
      double overpayment = debt_balance * bid;
      debt_balance = debt_balance - payment;
      add_table(credit_controller.GetOutputData().month_payment[i], model, i,
                0);
      add_table(payment, model, i, 1);
      add_table(overpayment, model, i, 2);
      if (i == (input.credit_term - 1)) {
        add_table(roundf(fabs(debt_balance)), model, i, 3);
      } else {
        add_table(fabs(debt_balance), model, i, 3);
      }
    }
  }

  ui->label_overpayment_result->setText(
      QString::number(credit_controller.GetOutputData().over_payment, 'f', 2) +
      " \u20BD");
  ui->label_total_result->setText(
      QString::number(credit_controller.GetOutputData().total_payment, 'f', 2) +
      " \u20BD");

  ui->tableView->setModel(model);
  ui->tableView->resizeColumnsToContents();
  ui->tableView->show();
}
