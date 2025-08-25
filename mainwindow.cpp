#include "mainwindow.h"

#include <QDebug>

#include "./ui_mainwindow.h"

bool BLDC_Status = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_BLDC_Toggle_Button_clicked() {
    BLDC_Status ? ui->BLDC_Status_Label->setText("OFF") : ui->BLDC_Status_Label->setText("ON");
    BLDC_Status = !BLDC_Status;
}
