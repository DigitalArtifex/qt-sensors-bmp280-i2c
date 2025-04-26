#ifndef QBMP28__H
#define QBMP28__H

#include <QObject>
#include <QSensor>
#include <QString>

#include "QBMP280_global.h"

#include "qbmp280reading.h"

class QBMP280Backend;

class QBMP28__EXPORT QBMP280 : public QSensor
{
    Q_OBJECT

    friend class QBMP280Backend;
public:
    explicit QBMP280(QObject *parent = nullptr);

    QBMP280Reading *reading() const;

    static inline char const * const sensorType = "QBMP280-I2C";

    enum class PowerMode : quint8
    {
        UltraLowPower = 1,
        LowPower,
        Normal,
        HighResolution,
        UltraHighResolution
    };
    Q_ENUM(PowerMode)

    enum class OverSampling : quint8 {
        /** No over-sampling. */
        NONE = 0x00,
        /** 1x over-sampling. */
        X1 = 0x01,
        /** 2x over-sampling. */
        X2 = 0x02,
        /** 4x over-sampling. */
        X4 = 0x03,
        /** 8x over-sampling. */
        X8 = 0x04,
        /** 16x over-sampling. */
        X16 = 0x05
    };
    Q_ENUM(OverSampling)

    enum class Filter : quint8 {
        /** No filtering. */
        OFF = 0x00,
        /** 2x filtering. */
        X2 = 0x01,
        /** 4x filtering. */
        X4 = 0x02,
        /** 8x filtering. */
        X8 = 0x03,
        /** 16x filtering. */
        X16 = 0x04
    };
    Q_ENUM(Filter)

    /** Standby duration in ms */
    enum class StandbyDuration : quint8 {
        /** 1 ms standby. */
        MS_1 = 0x00,
        /** 62.5 ms standby. */
        MS_63 = 0x01,
        /** 125 ms standby. */
        MS_125 = 0x02,
        /** 250 ms standby. */
        MS_250 = 0x03,
        /** 500 ms standby. */
        MS_500 = 0x04,
        /** 1000 ms standby. */
        MS_1000 = 0x05,
        /** 2000 ms standby. */
        MS_2000 = 0x06,
        /** 4000 ms standby. */
        MS_4000 = 0x07
    };
    Q_ENUM(StandbyDuration)

    /**
     * @brief I2C Bus
     * @return Path to the I2C Bus
     */
    QString bus() const;

    /**
     * @brief I2C Address
     * @return Address of the I2C device
     */
    quint8 address() const;

    /**
     * @brief Data Filter Mode
     * @return Current Filer Mode
     */
    Filter filterMode() const;

    /**
     * @brief Standby Duration
     * @return Current Standby Duration
     */
    StandbyDuration standbyDuration() const;

    /**
     * @brief Sea Level Pressure
     *
     * This value is used when calculating current altitude
     *
     * @return Current Sea Level Pressure
     */
    qreal seaLevelPressure() const;

    /**
     * @brief powerMode
     * @return
     */
    PowerMode powerMode() const;

    /**
     * @brief backendDebug
     * @return
     */
    bool backendDebug() const;

public slots:
    /**
     * @brief Sets the I2C Bus path
     * @param bus
     */
    void setBus(const QString &bus);

    /**
     * @brief Sets the I2C Address
     * @param address
     */
    void setAddress(quint8 address);

    /**
     * @brief Sets the Data Filter Mode
     * @param filterMode
     */
    void setFilterMode(Filter filterMode);

    /**
     * @brief Sets the Standby Duration
     * @param standbyDuration
     */
    void setStandbyDuration(StandbyDuration standbyDuration);

    /**
     * @brief Sets the Sea Level Pressure
     *
     * This value is used when calculating current altitude
     *
     * @param seaLevelPressure
     */
    void setSeaLevelPressure(qreal seaLevelPressure);

    /**
     * @brief Sets the current power mode
     *
     * BMP280 docs specify 5 power levels from UltraLowPower
     * to UltraHighResolution (see PowerMode enum). Each power
     * level describes the optimal oversampling modes for
     * temperature and pressure measurements for that mode.
     *
     * @param powerMode
     */
    void setPowerMode(PowerMode powerMode);

    /**
     * @brief Sets backend debugging mode
     *
     * Setting this to true will cause the backend to report all changes in
     * the sensor state
     *
     * @param backendDebug
     */
    void setBackendDebug(bool backendDebug);

signals:
    /**
     * @brief busChanged
     *
     * This signal is emitted when the I2C Bus has been changed. It's mostly
     * used to keep the backend up-to-date.
     */
    void busChanged();

    /**
     * @brief addressChanged
     */
    void addressChanged();

    /**
     * @brief filterModeChanged
     */
    void filterModeChanged();

    /**
     * @brief standbyDurationChanged
     */
    void standbyDurationChanged();

    /**
     * @brief seaLevelPressureChanged
     */
    void seaLevelPressureChanged();

    /**
     * @brief powerModeChanged
     */
    void powerModeChanged();

    /**
     * @brief backendDebugChanged
     */
    void backendDebugChanged();

private:
    QBMP280Reading *m_reading = nullptr;

    QString m_bus = "/dev/i2c-1"; //i2c bus path
    quint8 m_address = 0x77; //i2c device address

    Filter m_filterMode = Filter::OFF;
    StandbyDuration m_standbyDuration = StandbyDuration::MS_63;

    qreal m_seaLevelPressure = 1013.25;

    PowerMode m_powerMode = PowerMode::UltraLowPower;
    bool m_backendDebug = false;

    Q_PROPERTY(QString bus READ bus WRITE setBus NOTIFY busChanged FINAL)
    Q_PROPERTY(quint8 address READ address WRITE setAddress NOTIFY addressChanged FINAL)
    Q_PROPERTY(Filter filterMode READ filterMode WRITE setFilterMode NOTIFY filterModeChanged FINAL)
    Q_PROPERTY(StandbyDuration standbyDuration READ standbyDuration WRITE setStandbyDuration NOTIFY standbyDurationChanged FINAL)
    Q_PROPERTY(qreal seaLevelPressure READ seaLevelPressure WRITE setSeaLevelPressure NOTIFY seaLevelPressureChanged FINAL)
    Q_PROPERTY(PowerMode powerMode READ powerMode WRITE setPowerMode NOTIFY powerModeChanged FINAL)
    Q_PROPERTY(bool backendDebug READ backendDebug WRITE setBackendDebug NOTIFY backendDebugChanged FINAL)
};

Q_DECLARE_METATYPE(QBMP280)

#endif // QBMP28__H
