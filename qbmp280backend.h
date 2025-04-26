#ifndef QBMP28_BACKEND_H
#define QBMP28_BACKEND_H

#include <QObject>
#include <QString>
#include <QSensorBackend>
#include <QTimer>
#include <QThread>
#include <QDateTime>

#include "QBMP280_global.h"
#include "qbmp280.h"

#include "fcntl.h"
#include "i2c/smbus.h"
#include "linux/i2c-dev.h"
#include "linux/errno.h"
#include "sys/ioctl.h"

class QBMP28__EXPORT QBMP280Backend : public QSensorBackend
{
    Q_OBJECT

    struct CalibrationData {
        quint16 dig_T1; /**< dig_T1 cal register. */
        qint16 dig_T2;  /**<  dig_T2 cal register. */
        qint16 dig_T3;  /**< dig_T3 cal register. */

        quint16 dig_P1; /**< dig_P1 cal register. */
        qint16 dig_P2;  /**< dig_P2 cal register. */
        qint16 dig_P3;  /**< dig_P3 cal register. */
        qint16 dig_P4;  /**< dig_P4 cal register. */
        qint16 dig_P5;  /**< dig_P5 cal register. */
        qint16 dig_P6;  /**< dig_P6 cal register. */
        qint16 dig_P7;  /**< dig_P7 cal register. */
        qint16 dig_P8;  /**< dig_P8 cal register. */
        qint16 dig_P9;  /**< dig_P9 cal register. */
    };

    enum class Register : quint8 {
        DIG_T1 = 0x88,
        DIG_T2 = 0x8A,
        DIG_T3 = 0x8C,
        DIG_P1 = 0x8E,
        DIG_P2 = 0x90,
        DIG_P3 = 0x92,
        DIG_P4 = 0x94,
        DIG_P5 = 0x96,
        DIG_P6 = 0x98,
        DIG_P7 = 0x9A,
        DIG_P8 = 0x9C,
        DIG_P9 = 0x9E,
        CHIPID = 0xD0,
        VERSION = 0xD1,
        SOFTRESET = 0xE0,
        CAL26 = 0xE1, /**< R calibration = 0xE1-0xF0 */
        STATUS = 0xF3,
        CONTROL = 0xF4,
        CONFIG = 0xF5,
        PRESSUREDATA = 0xF7,
        TEMPDATA = 0xFA
    };

    /** Operating mode for the sensor. */
    enum class Mode : quint8 {
        /** Sleep mode. */
        SLEEP = 0x00,
        /** Forced mode. */
        FORCED = 0x01,
        /** Normal mode. */
        NORMAL = 0x03,
        /** Software reset. */
        RESET = 0xB6,
        /** Waking */
        WAKING = 0x08
    };

    /** Encapsulates the config register */
    struct ConfigRegisterData {
        /** Initialize to power-on-reset state */
        ConfigRegisterData() : standyDuration((quint8)QBMP280::StandbyDuration::MS_1), filterMode((quint8)QBMP280::Filter::OFF), none(0), spi3w_en(0) {}
        /** Inactive duration (standby time) in normal mode */
        quint8 standyDuration : 3;
        /** Filter settings */
        quint8 filterMode : 3;
        /** Unused - don't set */
        quint8 none : 1;
        /** Enables 3-wire SPI */
        quint8 spi3w_en : 1;
        /** Used to retrieve the assembled config register's byte value. */
        quint8 flatten() { return (standyDuration << 5) | (filterMode << 2) | spi3w_en; }
    };

    /** Encapsulates trhe ctrl_meas register */
    struct ControlRegisterData {
        /** Initialize to power-on-reset state */
        ControlRegisterData()
            : temperatureOverSampling((quint8)QBMP280::OverSampling::NONE), pressureOverSampling((quint8)QBMP280::OverSampling::NONE), mode((quint8)Mode::SLEEP) {}
        /** Temperature oversampling. */
        quint8 temperatureOverSampling : 3;
        /** Pressure oversampling. */
        quint8 pressureOverSampling : 3;
        /** Device mode */
        quint8 mode : 2;
        /** Used to retrieve the assembled ctrl_meas register's byte value. */
        quint8 flatten() { return (temperatureOverSampling << 5) | (pressureOverSampling << 2) | mode; }
    };

public:
    static inline const char* id = "QBMP280-Backend-I2C";
    static inline const quint8 m_chipId = 0x58; //i2c chip id
    QBMP280Backend(QSensor *sensor = nullptr);
    ~QBMP280Backend();

    virtual void start() override;
    virtual void stop() override;
    virtual bool isFeatureSupported(QSensor::Feature feature) const override;

signals:

protected slots:
    void poll();

    bool readTemperature();
    bool readPressure();
    bool readAltitude();
    bool readCalibration();
    bool readCalibrationRegister(Register reg, quint16 &buffer);
    bool readCalibrationRegister(Register reg, qint16 &buffer);

    bool writeConfiguration(QBMP280Backend::ConfigRegisterData &data);
    bool writeControlData(QBMP280Backend::ControlRegisterData &data);
    bool initialize();
    bool softReset();
    //bool hardReset(); //requires GPIO setup and mosfet
    bool confirmID();
    bool wake();

    bool startI2C();
    bool endI2C();
    void handleFault();

    void onSensorBusChanged();
    void onSensorAddressChanged();
    void onSensorPowerModeChanged();
    void onSensorSeaLevelPressureChanged();
    void onSensorStandyDurationChanged();
    void onSensorFilterChanged();
    void onSensorDataRateChanged();
    void onSensorAlwaysOnChanged();
    void reportEvent(QString message);
    void reportError(QString message);
    void newLine();

private:

    //When a fault occurs due to the init state, handleFault()
    //will ignore it unless we are initializing
    bool m_initialized = false;
    bool m_initializing = false;
    bool m_backendDebug = false;

    ConfigRegisterData m_config;
    ControlRegisterData m_controlData;

    CalibrationData m_calibration;

    int m_i2c = -1;
    int m_errno;

    qreal m_temperature = 0;
    qint32 m_temperatureData = 0;
    qreal m_pressure = 0;
    qreal m_altitude = 0;
    qreal m_seaLevelPressure;

    QString m_bus;
    quint8 m_address;

    QTimer *m_pollTimer = nullptr;

    QBMP280Reading m_reading;
};

#endif // QBMP28_BACKEND_H
