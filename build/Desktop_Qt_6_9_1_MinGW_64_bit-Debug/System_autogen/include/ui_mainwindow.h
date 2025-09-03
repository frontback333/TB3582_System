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
    QFrame *ThrustFrame;
    QWidget *ThrustGraph;
    QLabel *ThrustV;
    QFrame *CombustTempFrame;
    QWidget *CombustTempGraph;
    QLabel *CombustTempV;
    QFrame *BLDCFrame;
    QSlider *BLDC_Speed_Bar;
    QLabel *BLDC_Status_Text;
    QLabel *BLDC_Status_Label;
    QPushButton *BLDC_Toggle_Button;
    QLabel *BLDC_Power_Label;
    QFrame *InletPressureFrame;
    QWidget *InletPressureGraph;
    QLabel *InletPressureV;
    QFrame *LabelFrame;
    QLabel *InletTempV;
    QLabel *FuelPressureV;
    QLabel *PressureRatioV;
    QLabel *CoolantTempV;
    QLabel *CoolantPressureV;
    QLabel *FuelPumpPowerV;
    QLabel *CoolantPumpPowerV;
    QLabel *SparkPlugLabel;
    QLabel *SparkPlugPower;
    QLabel *DateTime;
    QLabel *LatestLog;
    QLabel *OpenedFile;
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
        ThrustFrame = new QFrame(centralwidget);
        ThrustFrame->setObjectName("ThrustFrame");
        ThrustFrame->setGeometry(QRect(10, 10, 400, 235));
        ThrustFrame->setFrameShape(QFrame::Shape::Box);
        ThrustFrame->setFrameShadow(QFrame::Shadow::Plain);
        ThrustFrame->setLineWidth(3);
        ThrustGraph = new QWidget(ThrustFrame);
        ThrustGraph->setObjectName("ThrustGraph");
        ThrustGraph->setGeometry(QRect(3, 3, 394, 202));
        ThrustV = new QLabel(ThrustFrame);
        ThrustV->setObjectName("ThrustV");
        ThrustV->setGeometry(QRect(5, 205, 385, 25));
        QFont font;
        font.setPointSize(10);
        ThrustV->setFont(font);
        ThrustV->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        CombustTempFrame = new QFrame(centralwidget);
        CombustTempFrame->setObjectName("CombustTempFrame");
        CombustTempFrame->setGeometry(QRect(420, 10, 400, 235));
        CombustTempFrame->setFrameShape(QFrame::Shape::Box);
        CombustTempFrame->setFrameShadow(QFrame::Shadow::Plain);
        CombustTempFrame->setLineWidth(3);
        CombustTempGraph = new QWidget(CombustTempFrame);
        CombustTempGraph->setObjectName("CombustTempGraph");
        CombustTempGraph->setGeometry(QRect(3, 3, 394, 202));
        CombustTempV = new QLabel(CombustTempFrame);
        CombustTempV->setObjectName("CombustTempV");
        CombustTempV->setGeometry(QRect(5, 205, 385, 25));
        CombustTempV->setFont(font);
        CombustTempV->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        BLDCFrame = new QFrame(centralwidget);
        BLDCFrame->setObjectName("BLDCFrame");
        BLDCFrame->setGeometry(QRect(830, 10, 185, 235));
        BLDCFrame->setFrameShape(QFrame::Shape::Box);
        BLDCFrame->setLineWidth(3);
        BLDC_Speed_Bar = new QSlider(BLDCFrame);
        BLDC_Speed_Bar->setObjectName("BLDC_Speed_Bar");
        BLDC_Speed_Bar->setGeometry(QRect(150, 50, 22, 141));
        BLDC_Speed_Bar->setAutoFillBackground(false);
        BLDC_Speed_Bar->setMaximum(100);
        BLDC_Speed_Bar->setOrientation(Qt::Orientation::Vertical);
        BLDC_Status_Text = new QLabel(BLDCFrame);
        BLDC_Status_Text->setObjectName("BLDC_Status_Text");
        BLDC_Status_Text->setGeometry(QRect(5, 30, 150, 35));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Tahoma")});
        font1.setPointSize(12);
        font1.setBold(true);
        BLDC_Status_Text->setFont(font1);
        BLDC_Status_Text->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        BLDC_Status_Text->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BLDC_Status_Label = new QLabel(BLDCFrame);
        BLDC_Status_Label->setObjectName("BLDC_Status_Label");
        BLDC_Status_Label->setGeometry(QRect(40, 130, 75, 35));
        QPalette palette;
        QBrush brush(QColor(0, 255, 0, 255));
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
        BLDC_Toggle_Button = new QPushButton(BLDCFrame);
        BLDC_Toggle_Button->setObjectName("BLDC_Toggle_Button");
        BLDC_Toggle_Button->setEnabled(true);
        BLDC_Toggle_Button->setGeometry(QRect(40, 180, 75, 35));
        BLDC_Power_Label = new QLabel(BLDCFrame);
        BLDC_Power_Label->setObjectName("BLDC_Power_Label");
        BLDC_Power_Label->setGeometry(QRect(27, 60, 100, 71));
        QPalette palette1;
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush1);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush1);
        BLDC_Power_Label->setPalette(palette1);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Tahoma")});
        font3.setPointSize(14);
        font3.setBold(true);
        BLDC_Power_Label->setFont(font3);
        BLDC_Power_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        InletPressureFrame = new QFrame(centralwidget);
        InletPressureFrame->setObjectName("InletPressureFrame");
        InletPressureFrame->setGeometry(QRect(10, 250, 400, 235));
        InletPressureFrame->setFrameShape(QFrame::Shape::Box);
        InletPressureFrame->setFrameShadow(QFrame::Shadow::Plain);
        InletPressureFrame->setLineWidth(3);
        InletPressureGraph = new QWidget(InletPressureFrame);
        InletPressureGraph->setObjectName("InletPressureGraph");
        InletPressureGraph->setGeometry(QRect(3, 3, 394, 202));
        InletPressureV = new QLabel(InletPressureFrame);
        InletPressureV->setObjectName("InletPressureV");
        InletPressureV->setGeometry(QRect(5, 205, 385, 25));
        InletPressureV->setFont(font);
        InletPressureV->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        LabelFrame = new QFrame(centralwidget);
        LabelFrame->setObjectName("LabelFrame");
        LabelFrame->setGeometry(QRect(420, 250, 595, 290));
        LabelFrame->setFrameShape(QFrame::Shape::Box);
        LabelFrame->setFrameShadow(QFrame::Shadow::Plain);
        LabelFrame->setLineWidth(3);
        InletTempV = new QLabel(LabelFrame);
        InletTempV->setObjectName("InletTempV");
        InletTempV->setGeometry(QRect(15, 10, 250, 25));
        InletTempV->setFont(font);
        InletTempV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        FuelPressureV = new QLabel(LabelFrame);
        FuelPressureV->setObjectName("FuelPressureV");
        FuelPressureV->setGeometry(QRect(15, 100, 250, 25));
        FuelPressureV->setFont(font);
        FuelPressureV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        PressureRatioV = new QLabel(LabelFrame);
        PressureRatioV->setObjectName("PressureRatioV");
        PressureRatioV->setGeometry(QRect(15, 40, 250, 25));
        PressureRatioV->setFont(font);
        PressureRatioV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        CoolantTempV = new QLabel(LabelFrame);
        CoolantTempV->setObjectName("CoolantTempV");
        CoolantTempV->setGeometry(QRect(15, 190, 250, 25));
        CoolantTempV->setFont(font);
        CoolantTempV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        CoolantPressureV = new QLabel(LabelFrame);
        CoolantPressureV->setObjectName("CoolantPressureV");
        CoolantPressureV->setGeometry(QRect(15, 220, 250, 25));
        CoolantPressureV->setFont(font);
        CoolantPressureV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        FuelPumpPowerV = new QLabel(LabelFrame);
        FuelPumpPowerV->setObjectName("FuelPumpPowerV");
        FuelPumpPowerV->setGeometry(QRect(15, 130, 250, 25));
        FuelPumpPowerV->setFont(font);
        FuelPumpPowerV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        CoolantPumpPowerV = new QLabel(LabelFrame);
        CoolantPumpPowerV->setObjectName("CoolantPumpPowerV");
        CoolantPumpPowerV->setGeometry(QRect(15, 250, 250, 25));
        CoolantPumpPowerV->setFont(font);
        CoolantPumpPowerV->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        SparkPlugLabel = new QLabel(LabelFrame);
        SparkPlugLabel->setObjectName("SparkPlugLabel");
        SparkPlugLabel->setGeometry(QRect(300, 20, 140, 25));
        SparkPlugLabel->setFont(font);
        SparkPlugLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        SparkPlugPower = new QLabel(LabelFrame);
        SparkPlugPower->setObjectName("SparkPlugPower");
        SparkPlugPower->setGeometry(QRect(450, 20, 50, 25));
        QPalette palette2;
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        SparkPlugPower->setPalette(palette2);
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(true);
        font4.setItalic(true);
        SparkPlugPower->setFont(font4);
        SparkPlugPower->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        DateTime = new QLabel(LabelFrame);
        DateTime->setObjectName("DateTime");
        DateTime->setGeometry(QRect(330, 250, 250, 25));
        DateTime->setFont(font);
        DateTime->setText(QString::fromUtf8("2025/9/1, 05:44:14"));
        DateTime->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        LatestLog = new QLabel(centralwidget);
        LatestLog->setObjectName("LatestLog");
        LatestLog->setGeometry(QRect(10, 490, 400, 25));
        QFont font5;
        font5.setPointSize(8);
        LatestLog->setFont(font5);
        LatestLog->setText(QString::fromUtf8("N/A"));
        LatestLog->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        OpenedFile = new QLabel(centralwidget);
        OpenedFile->setObjectName("OpenedFile");
        OpenedFile->setGeometry(QRect(10, 520, 400, 25));
        OpenedFile->setFont(font5);
        OpenedFile->setText(QString::fromUtf8("N/A"));
        OpenedFile->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
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
        ThrustV->setText(QCoreApplication::translate("MainWindow", "Thrust : 0 kg", nullptr));
        CombustTempV->setText(QCoreApplication::translate("MainWindow", "Combusted Gas Temperature : 0 \302\260C", nullptr));
        BLDC_Status_Text->setText(QCoreApplication::translate("MainWindow", "BLDC Power", nullptr));
        BLDC_Status_Label->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        BLDC_Toggle_Button->setText(QCoreApplication::translate("MainWindow", "TOGGLE", nullptr));
        BLDC_Power_Label->setText(QCoreApplication::translate("MainWindow", "0%\n"
"POWER", nullptr));
        InletPressureV->setText(QCoreApplication::translate("MainWindow", "Inlet Pressure : 0 bar", nullptr));
        InletTempV->setText(QCoreApplication::translate("MainWindow", "Inlet Temperature :", nullptr));
        FuelPressureV->setText(QCoreApplication::translate("MainWindow", "Fuel Pressure : ", nullptr));
        PressureRatioV->setText(QCoreApplication::translate("MainWindow", "Pressure Ratio : ", nullptr));
        CoolantTempV->setText(QCoreApplication::translate("MainWindow", "Cooling Oil Temp :", nullptr));
        CoolantPressureV->setText(QCoreApplication::translate("MainWindow", "Cooling Oil Pressure : ", nullptr));
        FuelPumpPowerV->setText(QCoreApplication::translate("MainWindow", "Fuel Pump Power :", nullptr));
        CoolantPumpPowerV->setText(QCoreApplication::translate("MainWindow", "Cooling Oil Pump Power : ", nullptr));
        SparkPlugLabel->setText(QCoreApplication::translate("MainWindow", "Spark Plug Power :", nullptr));
        SparkPlugPower->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
