#pragma once
#include "mainwindow.h"
#include <QDebug>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

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
              << "ThrustV,CombustTempV,InletPressureV,"
              << "InletTempV,PressureRatioV,"
              << "FuelPressureV,FuelPumpPowerV,"
              << "CoolantTempV,CoolantPressureV,CoolantPumpPowerV,"
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
              << s.inletTemp << ',' << s.compressRatio << ',' << s.fuelPressure << ',' << s.fuelPumpPower << ','
              << s.coolantTemp << ',' << s.coolantPressure << ',' << s.coolantPumpPower << ','
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

    ui->InletTempV->setText(QString("Inlet Temperature : %1 °C").arg(s.inletPressure,0,'f',2));
    ui->PressureRatioV->setText(QString("Pressure Ratio : %1 ").arg(s.inletPressure,0,'f',2));

    ui->FuelPressureV->setText(QString("Fuel Pressure : %1 bar").arg(s.inletPressure,0,'f',2));
    ui->FuelPumpPowerV->setText(QString("Fuel Pump Power : %1 %").arg(s.inletPressure,0,'f',2));

    ui->CoolantTempV->setText(QString("Cooling Oil Temp : %1 °C").arg(s.inletPressure,0,'f',2));
    ui->CoolantPressureV->setText(QString("Cooling Oil Pressure : %1 bar").arg(s.inletPressure,0,'f',2));
    ui->CoolantPumpPowerV->setText(QString("Cooling Oil Pump Power : %1 %").arg(s.inletPressure,0,'f',2));

    ui->SparkPlugPower->setText(s.SparkPlugStatus ? "ON":"OFF");
    ui->SparkPlugPower->setStyleSheet(
        s.SparkPlugStatus ? "QLabel{color: #ff0000;}"
                          :  "QLabel{color: #00ff00;}");
    ui->DateTime->setText(s.D_T.toString("yyyy/M/d, HH:mm:ss.zzz"));
}

//밑 코드 교체하기

FullData MainWindow::readSensors(){
    FullData s;
    s.D_T = QDateTime::currentDateTime();
#ifdef Q_OS_UNIX
    // ---------- 1) 최초 1회: 채널/장치 오픈 ----------
    static bool once = false;
    if (!once) {
        // HX711 (DT=GPIO26, SCK=GPIO22)
        hw.hx.begin(HW::Pins::HX_DT, HW::Pins::HX_SCK);
        hw.hx.tare(16); // 필요 없으면 삭제

        // ADS1115 (IIO, 주소 고정 방식)
        //   0x4B (ADDR→ID_SC/GPIO1), 0x49 (ADDR→3V3)
        //   채널 매핑은 상황에 맞게 수정 가능
        hw.iioOpenAddr(1, 0x4B, 0); // inletPressure  → ADS@0x4B CH0
        hw.iioOpenAddr(1, 0x49, 1); // fuelPressure   → ADS@0x49 CH1
        hw.iioOpenAddr(1, 0x49, 2); // coolantPressure→ ADS@0x49 CH2
        once = true;
    }

    // ---------- 2) 실제 읽기 ----------
    // 2-1) 추력(kg): HX711
    {
        double kg = hw::HX.readKg(8);              // 8샘플 평균
        s.thrust = std::isfinite(kg) ? kg : 0.0;
    }

    // 2-2) 온도(°C): MAX6675 3채널 사용 예
    {
        // CS 핀 매핑: 7=CS0(연소가스), 8=CS1(흡입), 24=CS2(쿨런트)
        double tComb = hw.readMAX6675C(HW::Pins::SPI_CS_0);
        double tInlt = hw.readMAX6675C(HW::Pins::SPI_CS_1);
        double tCool = hw.readMAX6675C(HW::Pins::SPI_CS_2);

        if (!std::isfinite(tComb)) tComb = 0.0;
        if (!std::isfinite(tInlt)) tInlt = 0.0;
        if (!std::isfinite(tCool)) tCool = 0.0;

        s.combustTemp  = tComb;
        s.inletTemp    = tInlt;
        s.coolantTemp  = tCool;
    }

    // 2-3) 압력(bar): ADS1115 (전압→압력 선형 환산)
    auto map_lin = [](double x, double xa, double xb, double ya, double yb){
        if (!std::isfinite(x)) return std::numeric_limits<double>::quiet_NaN();
        if (xb == xa) return std::numeric_limits<double>::quiet_NaN();
        double y = ya + (x - xa) * (yb - ya) / (xb - xa);
        return y;
    };
    auto clamp = [](double v, double lo, double hi){
        if (!std::isfinite(v)) return v;
        if (v < lo) v = lo; if (v > hi) v = hi; return v;
    };

    // TODO: 센서 스펙에 맞게 보정하세요.
    // 예시(아주 흔함): 0.5–4.5 V → 0–10 bar (래티오메트릭 트랜스듀서)
    static constexpr double V_MIN = 0.5;
    static constexpr double V_MAX = 4.5;
    static constexpr double P_MIN = 0.0;
    static constexpr double P_MAX = 10.0;

    {
        double v_inlet = hw.iioReadVAddr(1, 0x4B, 0); // V
        double v_fuel  = hw.iioReadVAddr(1, 0x49, 1); // V
        double v_cool  = hw.iioReadVAddr(1, 0x49, 2); // V

        double p_inlet = map_lin(v_inlet, V_MIN, V_MAX, P_MIN, P_MAX);
        double p_fuel  = map_lin(v_fuel,  V_MIN, V_MAX, P_MIN, P_MAX);
        double p_cool  = map_lin(v_cool,  V_MIN, V_MAX, P_MIN, P_MAX);

        if (std::isfinite(p_inlet)) s.inletPressure   = clamp(p_inlet, P_MIN, P_MAX);
        if (std::isfinite(p_fuel )) s.fuelPressure    = clamp(p_fuel , P_MIN, P_MAX);
        if (std::isfinite(p_cool )) s.coolantPressure = clamp(p_cool , P_MIN, P_MAX);
    }

    // 2-4) 파생값/상태
    s.compressRatio = (s.inletPressure > 0) ? (s.inletPressure / 1.01325) : 0.0; // bar/atm
    s.fuelPumpPower    = BLDC_Power;  // 실제 펌프 PWM을 쓰면 그 값으로 대체
    s.coolantPumpPower = 0.0;         // TODO: 쿨런트 펌프 제어값/센서로 연결
    s.SparkPlugStatus  = hw.readSparkPower();

#else
    auto rnd = QRandomGenerator::global();
    static double t=0, ct=20, ip=2.0, it=20, pr=1.0, fp=0.3, fpp=50, coT=35, coP=1.2, coPP=60;
    auto jitter = [](double v, double step, double minv, double maxv){
        auto* rng = QRandomGenerator::global();
        double u = rng->generateDouble();
        v += (u - 0.5) * step;

        if (v<minv) v=minv; if (v>maxv) v=maxv; return v;
    };
    t   = jitter(t,   1.0,   0.0, 200.0);
    ct  = jitter(ct,  2.0,  20.0,1200.0);
    ip  = jitter(ip,  0.02, 0.80,  3.00);
    it  = jitter(it,  0.50,  0.0, 200.0);
    pr  = jitter(pr,  0.01, 0.80,  2.50);
    fp  = jitter(fp,  0.02, 0.00, 10.00);
    fpp = jitter(fpp,  1.0, 0.00,100.00);
    coT = jitter(coT,  0.8,  0.0, 200.0);
    coP = jitter(coP, 0.02, 0.50,  3.00);
    coPP= jitter(coPP, 1.0,  0.0,100.0);

    s.thrust = t;
    s.combustTemp = ct;
    s.inletPressure = ip;
    s.inletTemp = it;
    s.compressRatio = ip/1.01325;
    s.fuelPressure = fp;
    s.fuelPumpPower = fpp;
    s.coolantTemp = coT;
    s.coolantPressure = coP;
    s.coolantPumpPower = coPP;
    s.SparkPlugStatus = (rnd->bounded(100) < 5) ? !s.SparkPlugStatus : s.SparkPlugStatus;
    return s;
#endif
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
    if(!BLDC_Status){
        ui->BLDC_Status_Label->setText("OFF");
        ui->BLDC_Status_Label->setStyleSheet("QLabel{color: #00ff00}");
        BLDC_Power = 0;
        ui->BLDC_Speed_Bar->setValue(BLDC_Power);
        ui->BLDC_Power_Label->setText(QString("%1%\nPOWER").arg(BLDC_Power));
    }else{
        ui->BLDC_Status_Label->setText("ON");
        ui->BLDC_Status_Label->setStyleSheet("QLabel{color: #ff0000;}");
    }
}

void MainWindow::on_BLDC_Speed_Bar_valueChanged(int value){
    if(!BLDC_Status)value = 0;
    BLDC_Power = value;
    ui->BLDC_Speed_Bar->setValue(value);
    ui->BLDC_Power_Label->setText(QString("%1%\nPOWER").arg(BLDC_Power));
}
