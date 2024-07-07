#include "depozit_calc.h"

#include "ui_depozit_calc.h"

depozit_calc::depozit_calc(QWidget *parent)
    : QDialog(parent), ui(new Ui::depozit_calc) {
  ui->setupUi(this);
}

depozit_calc::~depozit_calc() { delete ui; }

double depozit_rate(double rate) { return rate * 0.01; }
int depozit_period(int term, int term_month) { return term * term_month; }

double depozit_total(double amount, int term, int term_month,
                     double interest_rate, int frequency, bool capitalization,
                     double replenishments, double withdraws) {
  double interest = depozit_rate(interest_rate), amount_tmp = amount,
         result = 0.0;
  int period = depozit_period(term, term_month);
  if (period >= frequency) {
    for (int i = 1; i <= period; i++) {
      double tmp_earn = 0.0;
      if (frequency == 1) {
        tmp_earn = amount_tmp * interest / 12;
      } else if (frequency == 12 && i % 12 == 0) {
        tmp_earn = amount_tmp * interest;
      }
      amount_tmp = amount_tmp + replenishments - withdraws;
      if (capitalization) amount_tmp += tmp_earn;
      result += tmp_earn;
    }
  }
  return result;
}

double depozit_tax_amount(double total_amount, double tax_rate) {
  double tax = depozit_rate(tax_rate);
  return total_amount * tax;
}

double depozit_end_balance(double total_amount, double amount,
                           double tax_amount, int term, int term_month,
                           double replenishments, double withdraws) {
  int period = depozit_period(term, term_month);
  double repl_and_wtdrw = (replenishments - withdraws) * period;
  return amount + total_amount - tax_amount + repl_and_wtdrw;
}

void depozit_calc::on_pushButton_clicked() {
  double Amount = ui->doubleSpinBox_sum_depozit->value();
  int Term = ui->spinBox_term->value();
  int TermMonth = 0;
  ui->comboBox_term->currentText() == "В годах" ? TermMonth = 12
                                                : TermMonth = 1;
  double InterestRate = ui->doubleSpinBox_interest_rate->value();
  double TaxRate = ui->doubleSpinBox_tax_rate->value();
  int Frequency = 0;
  ui->comboBox_frequency->currentText() == "Раз в год" ? Frequency = 12
                                                       : Frequency = 1;
  bool Capitalization = ui->checkBox_capitalization->isChecked();
  double Replenishment = ui->doubleSpinBox_replenishment->value();
  double Withdrawal = ui->doubleSpinBox_withdrawal->value();

  double totalInterest =
      depozit_total(Amount, Term, TermMonth, InterestRate, Frequency,
                    Capitalization, Replenishment, Withdrawal);
  QString totalInterestStr = QString::number(totalInterest, 'f', 2);
  ui->label_Interest_total->setText(totalInterestStr + " \u20BD");

  double totalTax = depozit_tax_amount(totalInterest, TaxRate);
  QString totalTaxStr = QString::number(totalTax, 'f', 2);
  ui->label_tax_total->setText(totalTaxStr + " \u20BD");

  double endBalance = depozit_end_balance(totalInterest, Amount, totalTax, Term,
                                          TermMonth, Replenishment, Withdrawal);
  QString endBalanceStr = QString::number(endBalance, 'f', 2);
  ui->label_total_balance->setText(endBalanceStr + " \u20BD");
}
