#ifndef HW_GPIO_H
#define HW_GPIO_H

#pragma once

#include<QtGlobal>
#include<QFile>
#include<QDir>
#include<QString>
#include <QMap>
#include <QPair>
#include<tuple>
#include <QByteArray>
#include <limits>

#ifdef Q_OS_UNIX
// UNIX 전용 의존성
#include <pigpio.h>
// endail/hx711
#include <hx711/common.h>
#endif

using Key = std::tuple<int,int,int>;

namespace HW{

struct Pins{
    static constexpr int BLDC_PWM       = 12; //12

    static constexpr int SparkPWR       = 27; //27

    static constexpr int I2C_SCL        = 3; //3
    static constexpr int I2C_SDA        = 2; //2

    static constexpr int ADDR_SCL       = 0x4B;
    static constexpr int ADDR_VCC       = 0x49;

    static constexpr int SPI_MISO       = 9; //9
    static constexpr int SPI_CLK        = 11; //11
    static constexpr int SPI_CS_0       = 7; //7
    static constexpr int SPI_CS_1       = 8; //8
    static constexpr int SPI_CS_2       = 24; //24
    static constexpr int SPI_CS_3       = 25; //25

    static constexpr int HX_SCK         = 22;
    static constexpr int HX_DT          =26;
};

struct GPIO {
    // ---------------- 공통(플랫폼 독립) ----------------
    bool   init();
    void   shutdown();
    void   setBLDCPower(int p);
    bool   readSparkPower();

    // MAX6675
    unsigned readBitsCS(int cs_gpio, int bits);
    double   decodeMAX6675(unsigned raw16);
    double   readMAX6675C(int cs_gpio);

    // ADS1115 (IIO/sysfs, 주소 고정)
    struct IioChannel {
        QFile  raw;
        double scale = std::numeric_limits<double>::quiet_NaN();
        static QString resolveIioBasePathByAddr(int bus, int addr7);
        bool openByAddr(int bus, int addr7, int ch);
        double readV();
        void close(){ if (raw.isOpen()) raw.close(); }
    };
    using Key = std::tuple<int,int,int>; // (bus, addr7, ch)
    QMap<Key, QSharedPointer<IioChannel>> iio;

    bool   iioOpenAddr(int bus, int addr7, int ch);
    double iioReadVAddr(int bus, int addr7, int ch);
    void   iioCloseAddr(int bus, int addr7, int ch);

    // HX711 (endail/hx711)
    struct HX {
        // 캘리브레이션 파라미터
        int    refUnit = -370;     // 예시 값(캘리브레이션 후 바꿔주세요)
        int    offset  = -367471;  // 예시 값

        // 초기화/옵션
        bool   begin(int dt = Pins::HX_DT, int sck = Pins::HX_SCK);
        void   setCalibration(int ref, int off);
        bool   tare(int samples = 16);
        double readKg(int samples = 35); // 단위 kg로 반환
        void   powerDown();
        void   powerUp();

    private:
        int _dt = Pins::HX_DT, _sck = Pins::HX_SCK;
        bool _ok = false;
#ifdef Q_OS_UNIX
        // endail/hx711
        // 참고: AdvancedHX711로 바꾸고 싶으면 타입만 변경하면 됨
        struct Impl;
        Impl* impl = nullptr;
#endif
    } hx;
};

struct Guard {
    bool ok;
    Guard();
    ~Guard();
};

}

#endif // HW_GPIO_H
