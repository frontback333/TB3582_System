#include <QApplication>

#include "mainwindow.h"
#include<QtCharts/QtCharts>
#include<QtCharts/QChartView>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}
