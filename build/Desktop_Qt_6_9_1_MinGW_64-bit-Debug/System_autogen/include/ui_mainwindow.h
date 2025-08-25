/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QSlider *verticalSlider;
    QPushButton *BLDC_Toggle_Button;
    QLabel *BLDC_Status_Text;
    QLabel *BLDC_Status_Label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(1024, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalSlider = new QSlider(centralwidget);
        verticalSlider->setObjectName("verticalSlider");
        verticalSlider->setGeometry(QRect(990, 220, 22, 141));
        verticalSlider->setOrientation(Qt::Orientation::Vertical);
        BLDC_Toggle_Button = new QPushButton(centralwidget);
        BLDC_Toggle_Button->setObjectName("BLDC_Toggle_Button");
        BLDC_Toggle_Button->setEnabled(true);
        BLDC_Toggle_Button->setGeometry(QRect(880, 320, 75, 35));
        BLDC_Status_Text = new QLabel(centralwidget);
        BLDC_Status_Text->setObjectName("BLDC_Status_Text");
        BLDC_Status_Text->setGeometry(QRect(860, 240, 130, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("Tahoma")});
        font.setPointSize(12);
        font.setBold(true);
        BLDC_Status_Text->setFont(font);
        BLDC_Status_Text->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        BLDC_Status_Text->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BLDC_Status_Label = new QLabel(centralwidget);
        BLDC_Status_Label->setObjectName("BLDC_Status_Label");
        BLDC_Status_Label->setGeometry(QRect(880, 280, 75, 35));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        BLDC_Status_Label->setPalette(palette);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Tahoma")});
        font1.setPointSize(18);
        font1.setBold(true);
        BLDC_Status_Label->setFont(font1);
        BLDC_Status_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1024, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        BLDC_Toggle_Button->setText(QCoreApplication::translate("MainWindow", "TOGGLE", nullptr));
        BLDC_Status_Text->setText(QCoreApplication::translate("MainWindow", "BLDC Power", nullptr));
        BLDC_Status_Label->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
