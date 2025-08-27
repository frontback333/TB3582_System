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
#include <QtWidgets/QFrame>
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
    QFrame *frame;
    QWidget *widget;
    QLabel *label;
    QFrame *frame_2;
    QWidget *widget_3;
    QLabel *label_3;
    QFrame *frame_3;
    QSlider *BLDC_Speed_Bar;
    QLabel *BLDC_Status_Text;
    QLabel *BLDC_Status_Label;
    QPushButton *BLDC_Toggle_Button;
    QLabel *BLDC_Speed_Label;
    QLabel *BLDC_RPM;
    QFrame *frame_4;
    QWidget *widget_5;
    QLabel *label_5;
    QFrame *frame_5;
    QLabel *label_2;
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
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 10, 400, 235));
        frame->setFrameShape(QFrame::Shape::Box);
        frame->setFrameShadow(QFrame::Shadow::Plain);
        frame->setLineWidth(3);
        widget = new QWidget(frame);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 400, 200));
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(5, 200, 385, 30));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(420, 10, 400, 235));
        frame_2->setFrameShape(QFrame::Shape::Box);
        frame_2->setFrameShadow(QFrame::Shadow::Plain);
        frame_2->setLineWidth(3);
        widget_3 = new QWidget(frame_2);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(0, 0, 400, 200));
        label_3 = new QLabel(frame_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(5, 200, 385, 30));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(830, 10, 185, 235));
        frame_3->setFrameShape(QFrame::Shape::Box);
        frame_3->setLineWidth(3);
        BLDC_Speed_Bar = new QSlider(frame_3);
        BLDC_Speed_Bar->setObjectName("BLDC_Speed_Bar");
        BLDC_Speed_Bar->setGeometry(QRect(150, 50, 22, 141));
        BLDC_Speed_Bar->setOrientation(Qt::Orientation::Vertical);
        BLDC_Status_Text = new QLabel(frame_3);
        BLDC_Status_Text->setObjectName("BLDC_Status_Text");
        BLDC_Status_Text->setGeometry(QRect(5, 30, 150, 35));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Tahoma")});
        font1.setPointSize(12);
        font1.setBold(true);
        BLDC_Status_Text->setFont(font1);
        BLDC_Status_Text->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        BLDC_Status_Text->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BLDC_Status_Label = new QLabel(frame_3);
        BLDC_Status_Label->setObjectName("BLDC_Status_Label");
        BLDC_Status_Label->setGeometry(QRect(40, 130, 75, 35));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        BLDC_Status_Label->setPalette(palette);
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Tahoma")});
        font2.setPointSize(18);
        font2.setBold(true);
        BLDC_Status_Label->setFont(font2);
        BLDC_Status_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BLDC_Toggle_Button = new QPushButton(frame_3);
        BLDC_Toggle_Button->setObjectName("BLDC_Toggle_Button");
        BLDC_Toggle_Button->setEnabled(true);
        BLDC_Toggle_Button->setGeometry(QRect(40, 180, 75, 35));
        BLDC_Speed_Label = new QLabel(frame_3);
        BLDC_Speed_Label->setObjectName("BLDC_Speed_Label");
        BLDC_Speed_Label->setGeometry(QRect(130, 200, 63, 20));
        BLDC_Speed_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BLDC_RPM = new QLabel(frame_3);
        BLDC_RPM->setObjectName("BLDC_RPM");
        BLDC_RPM->setGeometry(QRect(40, 60, 75, 71));
        QPalette palette1;
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush1);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush1);
        BLDC_RPM->setPalette(palette1);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Tahoma")});
        font3.setPointSize(14);
        font3.setBold(true);
        BLDC_RPM->setFont(font3);
        BLDC_RPM->setAlignment(Qt::AlignmentFlag::AlignCenter);
        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(10, 280, 400, 235));
        frame_4->setFrameShape(QFrame::Shape::Box);
        frame_4->setFrameShadow(QFrame::Shadow::Plain);
        frame_4->setLineWidth(3);
        widget_5 = new QWidget(frame_4);
        widget_5->setObjectName("widget_5");
        widget_5->setGeometry(QRect(0, 0, 400, 200));
        label_5 = new QLabel(frame_4);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(5, 200, 385, 30));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        frame_5 = new QFrame(centralwidget);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(420, 250, 595, 290));
        frame_5->setFrameShape(QFrame::Shape::Box);
        frame_5->setFrameShadow(QFrame::Shadow::Plain);
        frame_5->setLineWidth(3);
        label_2 = new QLabel(frame_5);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 10, 200, 20));
        label_2->setFont(font);
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
        label->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        BLDC_Status_Text->setText(QCoreApplication::translate("MainWindow", "BLDC Power", nullptr));
        BLDC_Status_Label->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        BLDC_Toggle_Button->setText(QCoreApplication::translate("MainWindow", "TOGGLE", nullptr));
        BLDC_Speed_Label->setText(QCoreApplication::translate("MainWindow", "0%", nullptr));
        BLDC_RPM->setText(QCoreApplication::translate("MainWindow", "0\n"
"RPM", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "N/A", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
