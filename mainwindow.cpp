#include "mainwindow.h"
#include "HW_GPIO.h"
#include <QDebug>
#include <algorithm>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    hwReady = hw.init();

    if(!hwReady){
        qWarning()<<"GPIO INIT FAILED";
        ui->BLDC_Toggle_Button->setEnabled(false);
        ui->BLDC_Speed_Bar->setEnabled(false);
    }
    thrustGraph.init(ui->ThrustGraph, "Thrust", "kg");
    combustTempGraph.init(ui->CombustTempGraph, "Combusted Gas Temperature", "°C");
    inletPressureGraph.init(ui->InletPressureGraph, "Inlet Pressure", "bar");

    if(openCsvFile()){
        FileOpened = true;
        connect(&renewTimer,&QTimer::timeout,this,&MainWindow::SamplingTick);
        renewTimer.start(renewMs);
    }else{
        ui->OpenedFile->setText("FILE NOT OPENED!! RESTART!!");
        ui->LatestLog->setText("FILE NOT OPENED!! RESTART!!");
    }
}

MainWindow::~MainWindow() {
#ifdef Q_OS_UNIX
    if(hwReady){
        hw.setBLDCPower(0);
        hw.hx.powerDown();
        hw.shutdown();
    }
#endif
    if(FileOpened)closeCsvFile();
    delete ui;
}

QString MainWindow::getBaseDir(){
    QDir d(QCoreApplication::applicationDirPath());
    d.cdUp();
    d.cdUp();
    return QDir::cleanPath(d.filePath("DataLogs"));
}

void MainWindow::SamplingTick() {
    FullData s = readSensors();
    const qint64 now = s.D_T.toMSecsSinceEpoch();

    thrustGraph.append(now, s.thrust, WindowMs);
    combustTempGraph.append(now, s.combustTemp, WindowMs);
    inletPressureGraph.append (now, s.inletPressure, WindowMs);

    writeCsv(s);
    updLabels(s);
}

void MainWindow::SyncDta(QFile &file, QTextStream *ts){
    if(ts)ts->flush();
}

bool MainWindow::openCsvFile(){
    const QString Fdir = getBaseDir().isEmpty() ?
                            (QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/DataLogs")
                            : getBaseDir();
    QDir().mkpath(Fdir);

    const QString Fname = "log_"+QDateTime::currentDateTime().toString("yyyyMMdd_HHmmsszz") + ".csv";
    logPath = Fdir + "/" + Fname;

    logFile.setFileName(logPath);

    if(!logFile.open(QIODevice::WriteOnly|QIODevice::Text)) return false;

    logStream.setEncoding(QStringConverter::Utf8);
    logStream.setLocale(QLocale::c());
    logStream.setDevice(&logFile);

    logStream << "timeStamp,"
              << "ThrustV,CombustTempV,InletPressureV,ABPressureV,"
              << "InletTempV,PressureRatioV,"
              << "FuelPressureV,FuelPumpPowerV,ABPumpPowerV,"
              << "CoolantTempV,CoolantPressureV,CoolantPumpPowerV,BatteryVoltage,"
              << "SparkPlugStatus\n";
    logStream.flush();
    framesAfterFlush = 8;
    framesAfterFsync = 20;
    ui->OpenedFile->setText(Fdir+"/"+Fname);
    return true;
}

void MainWindow::writeCsv(const FullData& s){
    const QString D_T = s.D_T.toString(Qt::ISODateWithMs);
    logStream << D_T << ','
              << s.thrust << ',' << s.combustTemp << ',' << s.inletPressure << ','
              << s.afterburnerPressure << ','
              << s.inletTemp << ',' << s.compressRatio << ',' << s.fuelPressure << ',' << s.fuelPumpPower << ',' << s.afterburnerPumpPower << ','
              << s.coolantTemp << ',' << s.coolantPressure << ',' << s.coolantPumpPower << ',' << s.batteryVoltage << ','
              << (s.SparkPlugStatus ? "ON" : "OFF") << '\n';
    if(++framesAfterFlush >= flushFrames){
        logStream.flush();
        framesAfterFlush = 0;
    }
    if(++framesAfterFsync>=fsyncFrames){
        #if defined(Q_OS_UNIX)
        ::fsync(logFile.handle());
        #endif
        #if defined(Q_OS_WIN)
        _commit(logFile.handle());
        intptr_t osfh = _get_osfhandle(logFile.handle());
        if(osfh!=1)::FlushFileBuffers(reinterpret_cast<HANDLE>(osfh));
        #endif
        framesAfterFsync = 0;
        updLatestLog();
    }
}

void MainWindow::closeCsvFile(){
    if(!logFile.isOpen())return;
    logStream.flush();
#if defined(Q_OS_UNIX)
    ::fsync(logFile.handle());
#endif
#if defined(Q_OS_WIN)
    _commit(logFile.handle());
    intptr_t osfh = _get_osfhandle(logFile.handle());
    if(osfh!=1)::FlushFileBuffers(reinterpret_cast<HANDLE>(osfh));
#endif
}

void MainWindow::updLabels(const FullData& s){
    ui->ThrustV->setText(QString("Thrust : %1 kg").arg(s.thrust,0,'f',2));
    ui->CombustTempV->setText(QString("Combusted Gas Temperature : %1 °C").arg(s.combustTemp,0,'f',2));
    ui->InletPressureV->setText(QString("Inlet Pressure : %1 bar").arg(s.inletPressure,0,'f',2));
    ui->ABPressureV->setText(QString("AB Pressure : %1 bar").arg(s.afterburnerPressure,0,'f',2));

    ui->InletTempV->setText(QString("Inlet Temperature : %1 °C").arg(s.inletTemp,0,'f',2));
    ui->PressureRatioV->setText(QString("Pressure Ratio : %1 ").arg(s.compressRatio,0,'f',2));

    ui->FuelPressureV->setText(QString("Fuel Pressure : %1 bar").arg(s.fuelPressure,0,'f',2));
    ui->FuelPumpPowerV->setText(QString("Fuel Pump Power : %1 %").arg(s.fuelPumpPower,0,'f',2));
    ui->ABPumpPowerV->setText(QString("AB Pump Power : %1 %").arg(s.afterburnerPumpPower,0,'f',2));

    ui->CoolantTempV->setText(QString("Oil Temp : %1 °C").arg(s.coolantTemp,0,'f',2));
    ui->CoolantPressureV->setText(QString("Oil Pressure : %1 bar").arg(s.coolantPressure,0,'f',2));
    ui->CoolantPumpPowerV->setText(QString("Oil Pump Power : %1 %").arg(s.coolantPumpPower,0,'f',2));
    ui->BatteryVoltage->setText(QString("Battery Voltage : %1 V").arg(s.batteryVoltage,0,'f',2));

    ui->SparkPlugPower->setText(s.SparkPlugStatus ? "ON":"OFF");
    ui->SparkPlugPower->setStyleSheet(
        s.SparkPlugStatus ? "QLabel{color: #ff0000;}"
                          :  "QLabel{color: #00ff00;}");
    ui->DateTime->setText(s.D_T.toString("yyyy/M/d, HH:mm:ss.zzz"));
}

//밑 코드 교체하기

double MainWindow::pressureFromVoltage(double voltage){
    if (!std::isfinite(voltage))
        return std::numeric_limits<double>::quiet_NaN();

    constexpr double V_MIN = 0.5;
    constexpr double V_MAX = 4.5;
    constexpr double P_MIN = 0.0;
    constexpr double P_MAX = 10.0;

    const double clamped = std::clamp(voltage, V_MIN, V_MAX);

    return P_MIN + (clamped - V_MIN) * (P_MAX - P_MIN) / (V_MAX - V_MIN);
}

double MainWindow::throttleFromVoltage(double voltage){
    if (!std::isfinite(voltage))
        return std::numeric_limits<double>::quiet_NaN();

    constexpr double V_MAX = 3.0;

    return std::clamp(voltage, 0.0, V_MAX) * 100.0 / V_MAX;
}

double MainWindow::batteryVoltageFromVoltage(double voltage){
    if (!std::isfinite(voltage))
        return std::numeric_limits<double>::quiet_NaN();

    constexpr double V_SCALE = 6.1;
    constexpr double V_OFFSET = 0.0;

    return voltage * V_SCALE + V_OFFSET;
}

FullData MainWindow::readSensors(){
    FullData s;
    s.D_T = QDateTime::currentDateTime();

#ifdef Q_OS_UNIX
    if (!hwReady) {
            s.thrust = std::numeric_limits<double>::quiet_NaN();
            s.combustTemp = std::numeric_limits<double>::quiet_NaN();
            s.inletPressure = std::numeric_limits<double>::quiet_NaN();
            s.afterburnerPressure = std::numeric_limits<double>::quiet_NaN();
            s.inletTemp = std::numeric_limits<double>::quiet_NaN();
            s.compressRatio = std::numeric_limits<double>::quiet_NaN();
            s.fuelPressure = std::numeric_limits<double>::quiet_NaN();
            s.fuelPumpPower = BLDC_Power;
            s.afterburnerPumpPower = std::numeric_limits<double>::quiet_NaN();
            s.coolantTemp = std::numeric_limits<double>::quiet_NaN();
            s.coolantPressure = std::numeric_limits<double>::quiet_NaN();
            s.coolantPumpPower = std::numeric_limits<double>::quiet_NaN();
            s.batteryVoltage = std::numeric_limits<double>::quiet_NaN();
            s.SparkPlugStatus = false;
            return s;
        }
    static bool hxInitialized = false;
    static bool adsInitialized = false;

    static QElapsedTimer max6675Timer;
    static double lastCombustTemp = std::numeric_limits<double>::quiet_NaN();
    static double lastInletTemp = std::numeric_limits<double>::quiet_NaN();
    static double lastCoolantTemp = std::numeric_limits<double>::quiet_NaN();

    if (!max6675Timer.isValid())
        max6675Timer.start();

    if (!hxInitialized) {
        hxInitialized = hw.hx.begin();
        if (!hxInitialized)
                qWarning() << "HX711 failed.";
        //if (hxInitialized)
        //    hw.hx.zero();
    }

    if (!adsInitialized) {
        adsInitialized = true;
        for(int i = 0; i < 4; i++){
            adsInitialized &= hw.iioOpenAddr(1, HW::Pins::ADDR_VCC, i);
            adsInitialized &= hw.iioOpenAddr(1, HW::Pins::ADDR_SCL, i);
        }
    }

    s.thrust = hw.hx.readKg();

    // MAX6675 변환 시간이 약 220ms
    if (max6675Timer.elapsed() >= MAX6675Ms) {
        const double combust = hw.readMAX6675C(HW::Pins::SPI_CS_0);
        const double inlet = hw.readMAX6675C(HW::Pins::SPI_CS_1);
        const double coolant = hw.readMAX6675C(HW::Pins::SPI_CS_2);

        if (std::isfinite(combust)) lastCombustTemp = combust;
        if (std::isfinite(inlet)) lastInletTemp = inlet;
        if (std::isfinite(coolant)) lastCoolantTemp = coolant;

        max6675Timer.restart();
    }

    s.combustTemp = lastCombustTemp;
    s.inletTemp = lastInletTemp;
    s.coolantTemp = lastCoolantTemp;

    s.inletPressure = pressureFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_VCC, HW::Pins::Comp_P_Ch));
    s.fuelPressure = pressureFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_VCC, HW::Pins::Fuel_P_Ch));
    s.afterburnerPressure = pressureFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_VCC, HW::Pins::AB_P_Ch));
    s.coolantPressure = pressureFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_VCC, HW::Pins::Oil_P_Ch));

    if (std::isfinite(s.inletPressure)) {
        s.compressRatio = s.inletPressure / 1.01325;
    } else {
        s.compressRatio = std::numeric_limits<double>::quiet_NaN();
    }

    s.batteryVoltage = batteryVoltageFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_SCL, HW::Pins::Battery_V_Ch));
    s.fuelPumpPower = throttleFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_SCL, HW::Pins::FuelPump_Throttle_Ch));
    s.afterburnerPumpPower = throttleFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_SCL, HW::Pins::AB_Throttle_Ch));
    s.coolantPumpPower = throttleFromVoltage(hw.iioReadVAddr(1, HW::Pins::ADDR_SCL, HW::Pins::Oil_Throttle_Ch));
    s.SparkPlugStatus = hw.readSparkPower();
#else
    auto rnd = QRandomGenerator::global();
    static double t=0, ct=20, ip=2.0, abp=1.0, it=20, pr=1.0, fp=0.3, fpp=50, abpp=40, coT=35, coP=1.2, coPP=60, bv=12.0;
    auto jitter = [](double v, double step, double minv, double maxv){
        auto* rng = QRandomGenerator::global();
        double u = rng->generateDouble();
        v += (u - 0.5) * step;

        if (v<minv) v=minv; if (v>maxv) v=maxv; return v;
    };
    t   = jitter(t,   1.0,   0.0, 200.0);
    ct  = jitter(ct,  2.0,  20.0,1200.0);
    ip  = jitter(ip,  0.02, 0.80,  3.00);
    abp = jitter(abp, 0.02, 0.00, 10.00);
    it  = jitter(it,  0.50,  0.0, 200.0);
    pr  = jitter(pr,  0.01, 0.80,  2.50);
    fp  = jitter(fp,  0.02, 0.00, 10.00);
    fpp = jitter(fpp,  1.0, 0.00,100.00);
    abpp= jitter(abpp, 1.0, 0.00,100.00);
    coT = jitter(coT,  0.8,  0.0, 200.0);
    coP = jitter(coP, 0.02, 0.50,  3.00);
    coPP= jitter(coPP, 1.0,  0.0,100.0);
    bv  = jitter(bv,  0.02, 0.00, 20.00);

    s.thrust = t;
    s.combustTemp = ct;
    s.inletPressure = ip;
    s.afterburnerPressure = abp;
    s.inletTemp = it;
    s.compressRatio = ip/1.01325;
    s.fuelPressure = fp;
    s.fuelPumpPower = fpp;
    s.afterburnerPumpPower = abpp;
    s.coolantTemp = coT;
    s.coolantPressure = coP;
    s.coolantPumpPower = coPP;
    s.batteryVoltage = bv;
    s.SparkPlugStatus = (rnd->bounded(100) < 5) ? !s.SparkPlugStatus : s.SparkPlugStatus;
#endif
    return s;
}

void MainWindow::updLatestLog(){
    if(logPath.isEmpty())return;
    QFile f(logPath);
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text)){
        ui->LatestLog->setText("Error While Reading File");
        return;
    }
    const qint64 tailSize = 16 * 1024;
    const qint64 sz = f.size();
    const qint64 start = qMax<qint64>(0, sz - tailSize);
    const qint64 want  = sz - start;
    if (!f.seek(start)) {
        ui->LatestLog->setText("Seek Failed");
        return;
    }
    QByteArray tail = f.read(want);
    f.close();

    tail.replace("\r", "");
    QList<QByteArray> lines = tail.split('\n');
    while (!lines.isEmpty() && lines.last().trimmed().isEmpty())
        lines.removeLast();
    if (lines.isEmpty()) { ui->LatestLog->setText("(empty)"); return; }

    QByteArray last = lines.last().trimmed();
    if (last.startsWith("timeStamp")) {
        if (lines.size() >= 2) last = lines.at(lines.size()-2).trimmed();
    }
    ui->LatestLog->setText(QString::fromUtf8(last));
    qDebug()<<QDateTime::currentDateTime().toString("mm:ss");
}

void MainWindow::on_BLDC_Toggle_Button_clicked() {
    BLDC_Status = !BLDC_Status;
    qDebug()<<BLDC_Status;
    if(!BLDC_Status){
        BLDC_Power = 0;
        ui->BLDC_Speed_Bar->setValue(BLDC_Power);
        ui->BLDC_Status_Label->setText("OFF");
        ui->BLDC_Status_Label->setStyleSheet("QLabel{color: #00ff00}");
        ui->BLDC_Power_Label->setText(QString("%1%\nPOWER").arg(BLDC_Power));
#ifdef Q_OS_UNIX
        if(hwReady){
            hw.setBLDCPower(0);
        }
#endif
    }else{
        ui->BLDC_Status_Label->setText("ON");
        ui->BLDC_Status_Label->setStyleSheet("QLabel{color: #ff0000;}");
#ifdef Q_OS_UNIX
        if(hwReady){
            hw.setBLDCPower(BLDC_Power);
        }
#endif
    }
}

void MainWindow::on_BLDC_Speed_Bar_valueChanged(int value){
    if(!BLDC_Status)value = 0;
    BLDC_Power = value;
#ifdef Q_OS_UNIX
    hw.setBLDCPower(BLDC_Power);
#endif
    ui->BLDC_Speed_Bar->setValue(value);
    ui->BLDC_Power_Label->setText(QString("%1%\nPOWER").arg(BLDC_Power));
}

void MainWindow::on_pushButton_clicked(){
#ifdef Q_OS_UNIX
    if (!hwReady)
        return;
#endif
    qDebug()<<"tare";
    hw.hx.zero();
}

