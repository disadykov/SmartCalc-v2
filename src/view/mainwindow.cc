#include "mainwindow.h"

#include <QLineEdit>
#include <QVector>

#include "credit_calc.h"
#include "depozit_calc.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->pushButton_zero, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_one, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_two, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_three, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_four, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_five, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_six, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_seven, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_eight, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_nine, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_left, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_right, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_mult, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(digits_numbers()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::digits_numbers() {
  QPushButton *button = (QPushButton *)sender();
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() +
                              button->text());
  ui->textEdit_input->setAlignment(Qt::AlignRight);
}

void MainWindow::on_pushButton_clean_clicked() {
  ui->textEdit_input->clear();
  ui->textEdit_input->setAlignment(Qt::AlignRight);
  ui->doubleSpinBox_editX->setValue(0.0);
}

void MainWindow::on_pushButton_dot_clicked() {
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() + '.');
  ui->textEdit_input->setAlignment(Qt::AlignRight);
}

void MainWindow::on_pushButton_back_clicked() {
  QLineEdit tmp_str;
  tmp_str.setText(ui->textEdit_input->toPlainText());
  tmp_str.backspace();
  ui->textEdit_input->setText(tmp_str.text());
  ui->textEdit_input->setAlignment(Qt::AlignRight);
}

void MainWindow::on_pushButton_pi_clicked() {
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() +
                              "3.14159265358979323846");
  ui->textEdit_input->setAlignment(Qt::AlignRight);
}

void MainWindow::on_pushButton_equals_clicked() {
  // setlocale(LC_NUMERIC, "C");
  ui->widget->removeGraph(0);

  s21::CalcModel str;
  s21::CalcController controller(&str);
  QString input_str = ui->textEdit_input->toPlainText();
  controller.SetInput(input_str.toStdString());
  controller.ModelSetExpression(ui->doubleSpinBox_editX->value());
  controller.ModelCalculate();
  long double result = controller.ModelGetResult();
  QString res_string;

  if (str.GetStatus() != s21::kOk) {
    ui->textEdit_input->setText(QString::fromStdString(str.ErrorPrint()));
  } else {
    if (ui->checkBox_2->isChecked()) {
      double minX, maxX, minY, maxY;
      QVector<double> xx, yy;

      minX = ui->doubleSpinBox_minX->value();
      maxX = ui->doubleSpinBox_maxX->value();
      minY = ui->doubleSpinBox_minY->value();
      maxY = ui->doubleSpinBox_maxY->value();

      double X = minX;
      double step = (maxX - minX) / 10000;
      double min = 1000000;
      double max = -10000000;
      long double result_graf;
      QString tmp_expression;
      tmp_expression = ui->textEdit_input->toPlainText();

      while (X < maxX) {
        xx.push_back(X);

        controller.ModelSetExpression(X);
        controller.ModelCalculate();
        result_graf = controller.ModelGetResult();
        yy.push_back(result_graf);

        if (min > result_graf && !std::isinf(result_graf)) min = result_graf;
        if (max < result_graf && !std::isinf(result_graf)) max = result_graf;
        X += step;
      }

      if (min < -1000000) min = -1000000;
      if (max > 1000000) max = 1000000;

      ui->widget->xAxis->setRange(minX * 1.005, maxX * 1.005);
      ui->widget->yAxis->setRange(minY * 1.005, maxY * 1.005);

      if (ui->checkBox_3->isChecked()) {
        ui->widget->yAxis->setRange(min * 1.005, max * 1.005);
        ui->doubleSpinBox_maxY->setValue(max * 1.005);
        ui->doubleSpinBox_minY->setValue(min * 1.005);
      } else {
        ui->doubleSpinBox_maxY->update();
        ui->doubleSpinBox_minY->update();
        ui->widget->yAxis->setRange(minY * 1.005, maxY * 1.005);
      }

      ui->widget->addGraph();
      ui->widget->graph(0)->addData(xx, yy);
      ui->widget->replot();

    } else {
      ui->widget->replot();
      ui->widget->removeGraph(0);
      if (ui->checkBox->isChecked()) {
        res_string = QString::number(result, 'f', 6);
      } else {
        res_string = QString::number(result, 'f', 15);
      }
      ui->textEdit_input->setText(res_string);
    }
  }
  ui->textEdit_input->setAlignment(Qt::AlignRight);
}

void MainWindow::on_checkBox_3_stateChanged(int arg1) {
  if (arg1) {
    ui->doubleSpinBox_maxY->setEnabled(false);
    ui->doubleSpinBox_minY->setEnabled(false);
  } else {
    ui->doubleSpinBox_maxY->setEnabled(true);
    ui->doubleSpinBox_minY->setEnabled(true);
  }
}

void MainWindow::on_pushButton_clicked() {
  credit_calc credit_window;
  credit_window.setModal(true);
  credit_window.exec();
}

void MainWindow::on_pushButton_depozit_clicked() {
  depozit_calc depozit_window;
  depozit_window.setModal(true);
  depozit_window.exec();
}

void MainWindow::on_pushButton_e_clicked() {
  ui->textEdit_input->setText(ui->textEdit_input->toPlainText() + "e");
  ui->textEdit_input->setAlignment(Qt::AlignRight);
}
