#include "HW_GPIO.h"

bool HW::GPIO::init(){
#ifdef Q_OS_UNIX
    if (gpioInitialise() < 0) return false;

    gpioSetMode(Pins::SparkPWR, PI_INPUT);

    gpioSetMode(Pins::SPI_CLK, PI_OUTPUT); gpioWrite(Pins::SPI_CLK, 0);
    for (int cs : {Pins::SPI_CS_0, Pins::SPI_CS_1, Pins::SPI_CS_2, Pins::SPI_CS_3}) {
        gpioSetMode(cs, PI_OUTPUT); gpioWrite(cs, 1);
    }
    return true;
#else
    return true;
#endif
}

void HW::GPIO::shutdown(){
#ifdef Q_OS_UNIX
    gpioTerminate();
#endif
}
void HW::GPIO::setBLDCPower(int p){
#ifdef Q_OS_UNIX
    if (p < 0) p = 0; if (p > 100) p = 100;
    gpioHardwarePWM(Pins::BLDC_PWM, 1000, p * 10000);
#endif
}

bool HW::GPIO::readSparkPower(){
#ifdef Q_OS_UNIX
    return gpioRead(Pins::SparkPWR) > 0;
#else
    return false;
#endif
}

// ---------------- MAX6675 ----------------
static constexpr int kClkDelayUs = 1;

unsigned HW::GPIO::readBitsCS(int cs_gpio, int bits){
#ifdef Q_OS_UNIX
    if (bits <= 0) return 0u;
    if (bits > 32) bits = 32;
    unsigned v = 0u;
    gpioWrite(cs_gpio, 0); gpioDelay(kClkDelayUs);
    for (int i=0; i<bits; ++i) {
        gpioWrite(Pins::SPI_CLK, 1); gpioDelay(kClkDelayUs);
        v = (v<<1) | (gpioRead(Pins::SPI_MISO) & 1u);
        gpioWrite(Pins::SPI_CLK, 0); gpioDelay(kClkDelayUs);
    }
    gpioWrite(cs_gpio, 1);
    if (bits < 32) v &= ((1u << bits) - 1u);
    return v;
#else
    return 0u;
#endif
}

double HW::GPIO::decodeMAX6675(unsigned raw16){
    raw16 &= 0xFFFFu;
    if (raw16 & 0x0004u) return std::numeric_limits<double>::quiet_NaN(); // open-circuit
    unsigned t12 = (raw16 >> 3) & 0x0FFFu;
    return t12 * 0.25; // °C
}

double HW::GPIO::readMAX6675C(int cs_gpio){
#ifdef Q_OS_UNIX
    unsigned raw = readBitsCS(cs_gpio, 16);
    return decodeMAX6675(raw);
#else
    return std::numeric_limits<double>::quiet_NaN();
#endif
}

// ---------------- ADS1115 (IIO/sysfs) ----------------
QString HW::GPIO::IioChannel::resolveIioBasePathByAddr(int bus, int addr7){
    const QString node = QString("/sys/bus/i2c/devices/%1-%2")
    .arg(bus)
        .arg(addr7, 4, 16, QLatin1Char('0')).toLower();
    QDir d(node);
    if (!d.exists()) return {};
    const auto lst = d.entryInfoList(QStringList() << "iio:device*",
                                     QDir::System | QDir::NoDotAndDotDot | QDir::AllEntries);
    if (lst.isEmpty()) return {};
    return lst.first().canonicalFilePath(); // /sys/bus/iio/devices/iio:deviceX
}

bool HW::GPIO::IioChannel::openByAddr(int bus, int addr7, int ch){
    const QString base = resolveIioBasePathByAddr(bus, addr7);
    if (base.isEmpty()) return false;

    QFile scl(base + QString("/in_voltage%1_scale").arg(ch));
    if (!scl.open(QIODevice::ReadOnly)) return false;
    bool ok=false;
    scale = QString::fromUtf8(scl.readAll()).trimmed().toDouble(&ok);
    scl.close();
    if (!ok) return false;

    raw.setFileName(base + QString("/in_voltage%1_raw").arg(ch));
    return raw.open(QIODevice::ReadOnly);
}

double HW::GPIO::IioChannel::readV(){
    if (!raw.isOpen()) return std::numeric_limits<double>::quiet_NaN();
    raw.seek(0);
    QByteArray b = raw.read(32);
    bool ok=false;
    int code = QString::fromUtf8(b).trimmed().toInt(&ok);
    if (!ok) return std::numeric_limits<double>::quiet_NaN();
    return code * scale;
}

bool HW::GPIO::iioOpenAddr(int bus, int addr7, int ch){
    Key key{bus, addr7, ch};
    if (iio.contains(key)) return true;

    auto chan = QSharedPointer<IioChannel>::create();
    if (!chan->openByAddr(bus, addr7, ch)) return false;

    iio.insert(key, chan);  // 포인터는 복사 가능 → 에러 없음
    return true;
}

double HW::GPIO::iioReadVAddr(int bus, int addr7, int ch){
    Key key{bus, addr7, ch};
    auto it = iio.find(key);
    if (it == iio.end()) return std::numeric_limits<double>::quiet_NaN();
    return it.value()->readV();  // 포인터 역참조
}

void HW::GPIO::iioCloseAddr(int bus, int addr7, int ch){
    Key key{bus, addr7, ch};
    auto it = iio.find(key);
    if (it != iio.end()){
        it.value()->close();
        iio.erase(it);
    }
}

// ---------------- HX711 (endail/hx711) ----------------
#ifdef Q_OS_UNIX
struct HW::GPIO::HX::Impl {
    HX711::SimpleHX711 dev;
    Impl(int dt, int sck, int ref, long off, HX711::Rate rate)
        : dev(dt, sck, ref, off, rate) {
        dev.setUnit(HX711::Mass::Unit::KG);
    }
};

// "1.23 kg" → 1.23 로 변환 (Mass에 바로 double 반환자가 없어서)
static double toKg(const HX711::Mass& m) {
    std::string s = m.toString(HX711::Mass::Unit::KG);
    auto pos = s.find(' ');
    if (pos != std::string::npos) s.resize(pos);
    try { return std::stod(s); } catch (...) { return std::numeric_limits<double>::quiet_NaN(); }
}
#endif

bool HW::GPIO::HX::begin(int dt, int sck) {
#ifdef Q_OS_UNIX
    _dt = dt; _sck = sck;
    delete impl; impl = nullptr; _ok = false;
    try {
        // 기본 10SPS 보드 가정 (80SPS 보드면 HZ_80로 교체)
        impl = new Impl(_dt, _sck, refUnit, offset, HX711::Rate::HZ_10);
        _ok = true;
    } catch (...) {
        impl = nullptr; _ok = false;
    }
    return _ok;
#else
    Q_UNUSED(dt); Q_UNUSED(sck);
    return false;
#endif
}

void HW::GPIO::HX::setCalibration(int ref, int off){
    refUnit = ref; offset = off;
#ifdef Q_OS_UNIX
    if (_ok) {
        // 가장 단순한 방법: 재생성으로 적용
        begin(_dt, _sck);
    }
#endif
}

void HW::GPIO::HX::zero() {
#ifdef Q_OS_UNIX
    if (_ok && impl) impl->dev.zero();
#endif
}

double HW::GPIO::HX::readKg(int samples) {
#ifdef Q_OS_UNIX
    if (!_ok || !impl) return std::numeric_limits<double>::quiet_NaN();
    return toKg(impl->dev.weight(samples));  // 내부 평균/중앙값 처리
#else
    Q_UNUSED(samples);
    return std::numeric_limits<double>::quiet_NaN();
#endif
}
void HW::GPIO::HX::powerDown(){
#ifdef Q_OS_UNIX
    if (_ok && impl) impl->dev.powerDown();
#endif
}
void HW::GPIO::HX::powerUp(){
#ifdef Q_OS_UNIX
    if (_ok && impl) impl->dev.powerUp();
#endif
}

/*
//#if defined(Q_OS_UNIX)
#include <pigpio.h>
#include <cmath>
struct GPIO{
    bool   init(){
        if (gpioInitialise() < 0) return false;

        gpioSetMode(Pins::SparkPWR, PI_INPUT);

        gpioSetMode(Pins::SPI_CLK, PI_OUTPUT); gpioWrite(Pins::SPI_CLK, 0);
        for (int cs : {Pins::SPI_CS_0, Pins::SPI_CS_1, Pins::SPI_CS_2, Pins::SPI_CS_3}) {
            gpioSetMode(cs, PI_OUTPUT); gpioWrite(cs, 1);
        }
        return true;
    }

    void shutdown(){gpioTerminate();}

    void   setBLDCPower(int p){
        if (p < 0) p = 0; if (p > 100) p = 100;
        gpioHardwarePWM(Pins::BLDC_PWM, 1000, p * 10000);
    }


    bool   readSparkPower(){return gpioRead(Pins::SparkPWR) > 0;}

    unsigned readBitsCS(int cs, int bits){
        unsigned v = 0;
        gpioWrite(cs, 0);      // CS low
        gpioDelay(1);
        for (int i=0;i<bits;i++){
            gpioWrite(Pins::SPI_SCLK, 1);
            gpioDelay(1);
            v = (v<<1) | (gpioRead(Pins::SPI_MISO) & 1);
            gpioWrite(Pins::SPI_SCLK, 0);
            gpioDelay(1);
        }
        gpioWrite(cs, 1);      // CS high
        return v;
    }

    double   decodeMAX6675(unsigned raw){
        if (raw & 0x4) return std::nan(""); // open thermocouple
        unsigned t = (raw >> 3) & 0x0FFF;
        return t * 0.25;
    }

    struct IioChannel {
        QFile  raw;
        double scale = std::numeric_limits<double>::quiet_NaN();

        // iio:deviceX 경로를 주소로부터 해석
        static QString resolveIioBasePathByAddr(int bus, int addr7){
            // ex) "/sys/bus/i2c/devices/1-004b/iio:deviceX" (X는 부팅마다 달라짐)
            const QString node = QString("/sys/bus/i2c/devices/%1-%2")
                                     .arg(bus)
                                     .arg(addr7, 4, 16, QLatin1Char('0'))  // 0x4b -> "004b"
                                     .toLower();
            QDir d(node);
            if (!d.exists()) return {};

            const auto lst = d.entryInfoList(QStringList() << "iio:device*",
                                             QDir::System | QDir::NoDotAndDotDot | QDir::AllEntries);
            if (lst.isEmpty()) return {};
            // canonicalFilePath()로 심링크를 실제 "/sys/bus/iio/devices/iio:deviceX"로 해석
            return lst.first().canonicalFilePath();
        }

        // bus, addr7(0x49/0x4b), ch(0~3)를 지정해서 채널 오픈
        bool openByAddr(int bus, int addr7, int ch){
            const QString base = resolveIioBasePathByAddr(bus, addr7);
            if (base.isEmpty()) return false;

            QFile scl(base + QString("/in_voltage%1_scale").arg(ch));
            if (!scl.open(QIODevice::ReadOnly)) return false;
            bool ok=false;
            scale = QString::fromUtf8(scl.readAll()).trimmed().toDouble(&ok);
            scl.close();
            if (!ok) return false;

            raw.setFileName(base + QString("/in_voltage%1_raw").arg(ch));
            return raw.open(QIODevice::ReadOnly);
        }

        double readV(){
            if (!raw.isOpen()) return std::numeric_limits<double>::quiet_NaN();
            raw.seek(0);                      // 매번 처음부터 읽기(최신 값)
            QByteArray b = raw.read(32);
            bool ok=false;
            const int rawCode = QString::fromUtf8(b).trimmed().toInt(&ok);
            if (!ok) return std::numeric_limits<double>::quiet_NaN();
            return rawCode * scale;           // V = raw * scale
        }

        void close(){ if (raw.isOpen()) raw.close(); }
    };

    // (bus, addr, ch) -> 채널 객체 캐시
    QMap<Key, IioChannel> iio;

    bool iioOpenAddr(int bus, int addr7, int ch){
        Key key{bus,addr7,ch};
        if (iio.contains(key)) return true;   // 이미 열림
        IioChannel c;
        if (!c.openByAddr(bus, addr7, ch)) return false;
        iio.insert(key, std::move(c));
        return true;
    }

    double iioReadVAddr(int bus, int addr7, int ch){
        Key key{bus,addr7,ch};
        auto it = iio.find(key);
        if (it == iio.end()) return std::numeric_limits<double>::quiet_NaN();
        return it->readV();
    }

    void iioCloseAddr(int bus, int addr7, int ch){
        Key key{bus,addr7,ch};
        auto it = iio.find(key);
        if (it != iio.end()){ it->close(); iio.erase(it); }
    }
};
//#else
struct GPIO{
    bool        init(){ return true; }
    void        shutdown(){}
    void        setBLDCPower(int){}
    bool        readSparkPower(){ return false; }
    int         openADS(int){ return -1; }
    void        closeADS(int){}
    int         ads1115_read_single(int, int){ return -1; }
    unsigned    readBitsCS(int, int){ return 0; }
    double      decodeMAX6675(unsigned){ return 0.0; }
};
//#endif
*/
