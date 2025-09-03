#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QDir>
#include <QFile>
#include <QLineSeries>
#include <QMainWindow>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>
#include <QtCharts>
#if defined(Q_OS_UNIX)
    #include<unistd.h>
#endif
#if defined(Q_OS_WIN)
    #include<io.h>
    #include<windows.h>
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct FullData {
    QDateTime   D_T;
    double      thrust = 0.0;         // kg
    double      combustTemp = 0.0;    // °C
    double      inletPressure = 0.0;  // bar

    double      inletTemp = 0.0;         // °C
    double      compressRatio = 0.0;     // number
    double      fuelPressure = 0.0;      // bar
    double      fuelPumpPower = 0.0;     // percentage
    double      coolantTemp = 0.0;       // °C
    double      coolantPressure = 0.0;   // bar
    double      coolantPumpPower = 0.0;  // percentage

    bool        SparkPlugStatus = false;
};

struct GraphComponent {
    QLineSeries     *series = nullptr;
    QChart          *chart = nullptr;
    QChartView      *view = nullptr;
    QDateTimeAxis   *axisX = nullptr;
    QValueAxis      *axisY = nullptr;
    QVBoxLayout     *layout = nullptr;

    void init(QWidget *targetWidget, const QString &title, const QString &yUnit = QString()) {
        chart = new QChart();
        chart->legend()->hide();
        chart->setTitle(title);
        chart->setTitleFont(QFont("Tahoma", 12, QFont::Bold));
        chart->setBackgroundVisible(false);
        chart->setMargins(QMargins(0, 0, 0, 0));

        axisX = new QDateTimeAxis();
        axisX->setFormat("HH:mm:ss");
        axisX->setTitleText("Time");
        axisX->setTickCount(6);
        chart->addAxis(axisX, Qt::AlignBottom);

        axisY = new QValueAxis();
        axisY->setLabelFormat("%.2f");
        if (!yUnit.isEmpty()) axisY->setTitleText(yUnit);
        chart->addAxis(axisY, Qt::AlignLeft);

        series = new QLineSeries();
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);

        layout = new QVBoxLayout(targetWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(view);
    }
    void append(qint64 t_ms, double value, qint64 windowMs) {
        const qint64 cutOffTime = t_ms - windowMs;

        auto sPoints = series->points();
        int removePoint = 0;
        while (removePoint < sPoints.size() && sPoints[removePoint].x() < cutOffTime) removePoint++;
        if (removePoint) series->removePoints(0, removePoint);

        series->append(t_ms, value);
        axisX->setRange(QDateTime::fromMSecsSinceEpoch(t_ms - windowMs),
                        QDateTime::fromMSecsSinceEpoch(t_ms));

        sPoints = series->points();
        if (!sPoints.isEmpty()) {
            double minY = sPoints.first().y(), maxY = minY;
            for (const auto &p : sPoints) {
                if (p.y() < minY) minY = p.y();
                if (p.y() > maxY) maxY = p.y();
            }
            if (minY == maxY) {
                minY -= 0.5;
                maxY += 0.5;
            }
            const double pad = (maxY - minY) * 0.1;
            axisY->setRange(minY - pad, maxY + pad);
        }
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void on_BLDC_Toggle_Button_clicked();
    void SamplingTick();

    FullData readSensors();

    void on_BLDC_Speed_Bar_valueChanged(int value);

private:
    Ui::MainWindow  *ui;
    GraphComponent  thrustGraph;
    GraphComponent  combustTempGraph;
    GraphComponent  inletPressureGraph;

    QTimer          renewTimer;
    const int       renewMs = 250;
    const qint64    WindowMs = 30 * 1000;

    QFile           logFile;
    QTextStream     logStream;
    QString         logPath;
    const QString   BaseDir = QString("D:/TB-3582/System/DataLogs");
    int             framesAfterFlush, framesAfterFsync = 0;
    const int       flushFrames = 8;
    const int       fsyncFrames = 20;
    bool            FileOpened = false;

    bool            BLDC_Status;
    int             BLDC_Power;

    void SyncDta(QFile &file, QTextStream *ts = nullptr);
    bool openCsvFile();
    void writeCsv(const FullData& s);
    void closeCsvFile();
    void updLatestLog();
    void updLabels(const FullData& s);
};
#endif  // MAINWINDOW_H
