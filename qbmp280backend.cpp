#include "qbmp280backend.h"

QBMP280Backend::QBMP280Backend(QSensor *sensor)
    : QSensorBackend{sensor}
{
    m_pollTimer = new QTimer(this);
    QObject::connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(poll()));

    reportEvent("QBMP280 BACKEND CREATED");

    setReading<QBMP280Reading>(&m_reading);
    reading();

    addDataRate(1, 157);
}

QBMP280Backend::~QBMP280Backend()
{
    if(m_pollTimer)
    {
        if(m_pollTimer->isActive())
            m_pollTimer->stop();

        delete m_pollTimer;
    }
}

void QBMP280Backend::start()
{
    if(m_pollTimer && m_pollTimer->isActive())
        return;

    if(!m_initialized && !initialize())
    {
        reportError("COULD NOT INITIALIZE SENSOR");
        handleFault();
        return;
    }

    m_pollTimer->setInterval(1000 / sensor()->dataRate());
    m_pollTimer->start();
}

void QBMP280Backend::stop()
{
    if(!m_pollTimer || !m_pollTimer->isActive())
        return;

    m_pollTimer->stop();
}

bool QBMP280Backend::isFeatureSupported(QSensor::Feature feature) const
{
    bool supported = false;

    switch(feature)
    {
    case QSensor::AlwaysOn:
        supported = true;
        break;
    default:
        break;
    };

    return supported;
}

void QBMP280Backend::poll()
{
    //start i2c
    if(!startI2C())
    {
        handleFault();
        return;
    }

    newLine();
    reportEvent("POLLING SENSOR");
    //force-wake the chip if we aren't always on
    if(m_controlData.mode == (quint8)Mode::FORCED && !wake())
    {
        handleFault();
        return;
    }

    //take measurements
    if(!readTemperature() || !readPressure() || !readAltitude())
    {
        handleFault();
        return;
    }

    reportEvent("================================");
    reportEvent(QString("DATA READ %1 %2 %3").arg(m_temperature, 0, 'f', 2).arg(m_pressure, 0, 'f', 2).arg(m_altitude, 0, 'f', 2));
    reportEvent("================================");

    if(!endI2C())
    {
        m_errno = errno; //errno is not set by endI2C()
        handleFault();
        return;
    }

    m_reading.setTemperature(m_temperature);
    m_reading.setPressure(m_pressure);
    m_reading.setAltitude(m_altitude);
    m_reading.setTimestamp(QDateTime::currentMSecsSinceEpoch());

    newReadingAvailable();
    reportEvent("UPDATED READING OBJECT");
}

bool QBMP280Backend::readTemperature()
{
    quint8 reg = (quint8)Register::TEMPDATA;
    quint8 *buffer = new quint8[3];

    struct i2c_msg messages[]
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &reg
        },
        {
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 3,
            .buf = buffer
        }
    };

    struct i2c_rdwr_ioctl_data payload =
    {
        .msgs = messages,
        .nmsgs = 2
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 1)
    {
        reportError("COULD NOT READ RAW TEMPERATURE DATA");
        m_errno = errno;
        return false;
    }

    qint32 var1 = 0;
    qint32 var2 = 0;
    qint32 reading = (((qint32)buffer[0] << 16) | ((qint32)buffer[1] << 8) | (qint32)buffer[2]);
    reading >>= 4;

    delete [] buffer;

    reportEvent(QString("RAW TEMPERATURE DATA %1").arg(reading));

    var1 = ((((reading>>3) - ((qint32)m_calibration.dig_T1<<1))) * ((qint32)m_calibration.dig_T2)) >> 11;
    var2 = (((((reading>>4) - ((qint32)m_calibration.dig_T1)) * ((reading>>4) - ((qint32)m_calibration.dig_T1)))
             >> 12) *
            ((qint32)m_calibration.dig_T3)) >> 14;

    m_temperatureData = var1 + var2;
    m_temperature = (qreal)((m_temperatureData * 5 + 128) >> 8) / 100;

    return true;
}

bool QBMP280Backend::readPressure()
{
    quint8 reg = (quint8)Register::PRESSUREDATA;
    quint8 *buffer = new quint8[3];

    struct i2c_msg messages[]
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &reg
        },
        {
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 3,
            .buf = buffer
        }
    };

    struct i2c_rdwr_ioctl_data payload =
    {
        .msgs = messages,
        .nmsgs = 2
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 1)
    {
        reportError("COULD NOT READ RAW PRESSURE DATA");
        m_errno = errno;
        return false;
    }

    qint64 var1 = 0;
    qint64 var2 = 0;
    qint64 pressureBuffer = 0;
    qint64 reading = (((qint64)buffer[0] << 16) | ((qint64)buffer[1] << 8) | (qint64)buffer[2]);
    reading >>= 4;

    delete [] buffer;

    reportEvent(QString("RAW PRESSURE DATA %1").arg(reading));

    var1 = ((qint64)m_temperatureData) - 128000;
    var2 = var1 * var1 * (qint64)m_calibration.dig_P6;
    var2 = var2 + ((var1*(qint64)m_calibration.dig_P5)<<17);
    var2 = var2 + (((qint64)m_calibration.dig_P4)<<35);
    var1 = ((var1 * var1 * (qint64)m_calibration.dig_P3)>>8) + ((var1 * (qint64)m_calibration.dig_P2)<<12);
    var1 = (((((qint64)1)<<47)+var1))*((qint64)m_calibration.dig_P1)>>33;

    if (var1 == 0)
    {
        return false; // avoid exception caused by division by zero
    }

    pressureBuffer = 1048576 - reading;
    pressureBuffer = (((pressureBuffer<<31)-var2)*3125)/var1;
    var1 = (((qint64)m_calibration.dig_P9) * (pressureBuffer>>13) * (pressureBuffer>>13)) >> 25;
    var2 = (((qint64)m_calibration.dig_P8) * pressureBuffer) >> 19;
    pressureBuffer = ((pressureBuffer + var1 + var2) >> 8) + (((qint64)m_calibration.dig_P7)<<4);

    m_pressure = ((qreal)pressureBuffer/256) / 100;

    return true;
}

bool QBMP280Backend::readAltitude()
{
    m_altitude = ((pow((m_seaLevelPressure / m_pressure), (1.0 / 5.257)) - 1) * (m_temperature + 273.15)) / 0.0065;
    return true;
}

bool QBMP280Backend::readCalibration()
{
    if(!readCalibrationRegister(Register::DIG_T1, m_calibration.dig_T1))
        return false;

    if(!readCalibrationRegister(Register::DIG_T2, m_calibration.dig_T2))
        return false;

    if(!readCalibrationRegister(Register::DIG_T3, m_calibration.dig_T3))
        return false;

    if(!readCalibrationRegister(Register::DIG_P1, m_calibration.dig_P1))
        return false;

    if(!readCalibrationRegister(Register::DIG_P2, m_calibration.dig_P2))
        return false;

    if(!readCalibrationRegister(Register::DIG_P3, m_calibration.dig_P3))
        return false;

    if(!readCalibrationRegister(Register::DIG_P4, m_calibration.dig_P4))
        return false;

    if(!readCalibrationRegister(Register::DIG_P5, m_calibration.dig_P5))
        return false;

    if(!readCalibrationRegister(Register::DIG_P6, m_calibration.dig_P6))
        return false;

    if(!readCalibrationRegister(Register::DIG_P7, m_calibration.dig_P7))
        return false;

    if(!readCalibrationRegister(Register::DIG_P8, m_calibration.dig_P8))
        return false;

    if(!readCalibrationRegister(Register::DIG_P9, m_calibration.dig_P9))
        return false;

    return true;
}

bool QBMP280Backend::readCalibrationRegister(Register reg, quint16 &buffer)
{
    quint8 *inbuffer = new quint8[2];
    quint8 outbuffer = (quint8)reg;

    struct i2c_msg messages[] =
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &outbuffer
        },
        {
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 2,
            .buf = inbuffer
        }
    };

    struct i2c_rdwr_ioctl_data payload =
    {
        .msgs = messages,
        .nmsgs = 2
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 0)
    {
        m_errno = errno;
        return false;
    }

    //swap MSB/LSB
    buffer = (inbuffer[1] << 8) | (inbuffer[0]);
    delete [] inbuffer;

    return true;
}

bool QBMP280Backend::readCalibrationRegister(Register reg, qint16 &buffer)
{
    quint8 *inbuffer = new quint8[2];
    quint8 outbuffer = (quint8)reg;

    struct i2c_msg messages[]
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &outbuffer
        },
        {
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 2,
            .buf = inbuffer
        }
    };

    struct i2c_rdwr_ioctl_data payload =
    {
        .msgs = messages,
        .nmsgs = 2
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 0)
    {
        m_errno = errno;
        return false;
    }

    //swap MSB/LSB
    buffer = (inbuffer[1] << 8) | (inbuffer[0]);
    delete [] inbuffer;

    return true;
}

bool QBMP280Backend::writeConfiguration(ConfigRegisterData &data)
{
    quint8 *buffer = new quint8[2];
    buffer[0] = (quint8)Register::CONTROL;
    buffer[1] = data.flatten();

    reportEvent(QString("WRITING 0x%1 TO REGISTER 0x%2").arg(buffer[1], 2, 16).arg(buffer[0],2,16));

    struct i2c_msg messages[] = {
        {
            .addr = m_address,
            .len = 2,
            .buf = buffer,
        },
    };

    struct i2c_rdwr_ioctl_data payload = {
        .msgs = messages,
        .nmsgs = sizeof(messages) / sizeof(messages[0]),
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 0)
    {
        m_errno = errno;
        reportError(QString("ERROR WRITING 0x%1 TO REGISTER 0x%2").arg(buffer[1], 2, 16).arg(buffer[0],2,16));

        return false;
    }

    m_config = data;
    delete [] buffer;

    return true;
}

bool QBMP280Backend::writeControlData(ControlRegisterData &data)
{
    quint8 *buffer = new quint8[2];
    buffer[0] = (quint8)Register::CONTROL;
    buffer[1] = data.flatten();

    reportEvent(QString("WRITING 0x%1 TO REGISTER 0x%2").arg(buffer[1], 2, 16).arg(buffer[0],2,16));

    struct i2c_msg messages[] = {
        {
            .addr = m_address,
            .len = 2,
            .buf = buffer,
        },
    };

    struct i2c_rdwr_ioctl_data payload = {
        .msgs = messages,
        .nmsgs = sizeof(messages) / sizeof(messages[0]),
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 0)
    {
        m_errno = errno;
        reportError(QString("ERROR WRITING 0x%1 TO REGISTER 0x%2").arg(buffer[1], 2, 16).arg(buffer[0],2,16));

        return false;
    }

    m_controlData = data;
    delete [] buffer;

    return true;
}

bool QBMP280Backend::initialize()
{
    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(!sensor) //make sure its a qbmp280
        return false;

    m_backendDebug = sensor->backendDebug();

    m_bus = sensor->bus();
    m_address = sensor->address();

    if(!startI2C())
        return false;

    if(!confirmID()) //make sure it's physically a bmp280
    {
        reportError(QString("CHIP ID FAILED"));
        return false;
    }

    if(!readCalibration())
    {
        reportError(QString("COULD NOT READ CALIBRATION"));
        return false;
    }

    //output calibration
    newLine();
    reportEvent("START CALIBRATION DATA");
    reportEvent(QString("DIG_P1: %1").arg(m_calibration.dig_P1));
    reportEvent(QString("DIG_P2: %1").arg(m_calibration.dig_P2));
    reportEvent(QString("DIG_P3: %1").arg(m_calibration.dig_P3));
    reportEvent(QString("DIG_P4: %1").arg(m_calibration.dig_P4));
    reportEvent(QString("DIG_P5: %1").arg(m_calibration.dig_P5));
    reportEvent(QString("DIG_P6: %1").arg(m_calibration.dig_P6));
    reportEvent(QString("DIG_P7: %1").arg(m_calibration.dig_P7));
    reportEvent(QString("DIG_P8: %1").arg(m_calibration.dig_P8));
    reportEvent(QString("DIG_P9: %1").arg(m_calibration.dig_P9));
    reportEvent(QString("DIG_T1: %1").arg(m_calibration.dig_T1));
    reportEvent(QString("DIG_T2: %1").arg(m_calibration.dig_T2));
    reportEvent(QString("DIG_T3: %1").arg(m_calibration.dig_T3));
    reportEvent("END CALIBRATION DATA");
    newLine();

    ConfigRegisterData config = m_config;
    config.filterMode = (quint8)sensor->filterMode();
    config.standyDuration = (quint8)sensor->standbyDuration();

    if(!writeConfiguration(config))
    {
        reportError(QString("COULD NOT WRITE CONFIGURATION"));
        return false;
    }

    ControlRegisterData data = m_controlData;

    //set recommended over-sampling based on power mode
    switch(sensor->powerMode())
    {
    case QBMP280::PowerMode::UltraLowPower:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X1;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::LowPower:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X2;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::Normal:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X4;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::HighResolution:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X8;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::UltraHighResolution:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X16;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X2;
        break;
    };

    if(sensor->isAlwaysOn())
        data.mode = (quint8)Mode::NORMAL;
    else
        data.mode = (quint8)Mode::FORCED;

    if(!writeControlData(data))
    {
        reportError(QString("COULD NOT WRITE CONTROL DATA"));
        return false;
    }

    m_seaLevelPressure = sensor->seaLevelPressure();

    connect(sensor, SIGNAL(addressChanged()), this, SLOT(onSensorAddressChanged()));
    connect(sensor, SIGNAL(busChanged()), this, SLOT(onSensorBusChanged()));
    connect(sensor, SIGNAL(filterModeChanged()), this, SLOT(onSensorFilterChanged()));
    connect(sensor, SIGNAL(powerModeChanged()), this, SLOT(onSensorPowerModeChanged()));
    connect(sensor, SIGNAL(seaLevelPressureChanged()), this, SLOT(onSensorSeaLevelPressureChanged()));
    connect(sensor, SIGNAL(standbyDurationChanged()), this, SLOT(onSensorStandyDurationChanged()));
    connect(sensor, SIGNAL(dataRateChanged()), this, SLOT(onSensorDataRateChanged()));
    connect(sensor, SIGNAL(alwaysOnChanged()), this, SLOT(onSensorAlwaysOnChanged()));

    if(!endI2C())
    {
        m_errno = errno;
        return false;
    }

    m_initialized = true;
    return true;
}

bool QBMP280Backend::softReset()
{
    //reset initialized state
    if(m_initialized)
    {
        m_initialized = false;

        disconnect(sensor(), SIGNAL(addressChanged()), this, SLOT(onSensorAddressChanged()));
        disconnect(sensor(), SIGNAL(busChanged()), this, SLOT(onSensorBusChanged()));
        disconnect(sensor(), SIGNAL(filterModeChanged()), this, SLOT(onSensorFilterChanged()));
        disconnect(sensor(), SIGNAL(overSamplingChanged()), this, SLOT(onSensorOverSampleChanged()));
        disconnect(sensor(), SIGNAL(seaLevelPressureChanged()), this, SLOT(onSensorSeaLevelPressureChanged()));
        disconnect(sensor(), SIGNAL(standbyDurationChanged()), this, SLOT(onSensorStandyDurationChanged()));
        disconnect(sensor(), SIGNAL(dataRateChanged()), this, SLOT(onSensorDataRateChanged()));
        disconnect(sensor(), SIGNAL(alwaysOnChanged()), this, SLOT(onSensorAlwaysOnChanged()));
    }

    //start i2c and select the softreset register
    if(!startI2C())
        return false;

    //write the controle code
    quint8 *buffer = new quint8[2];
    buffer[0] = (quint8)Register::SOFTRESET;
    buffer[1] = (quint8)Mode::RESET;

    struct i2c_msg messages[] =
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 2,
            .buf = buffer
        }
    };

    struct i2c_rdwr_ioctl_data payload = {
        .msgs = messages,
        .nmsgs = 1
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 1)
    {
        reportError("COULD NOT WRITE SOFTRESET REGISTER");
        m_errno = errno;
        endI2C();
        return false;
    }

    if(!endI2C())
    {
        m_errno = errno;
        return false;
    }

    delete [] buffer;

    //sleep for 20ms for reset
    QThread::msleep(20);

    return initialize();
}

bool QBMP280Backend::confirmID()
{
    quint8 id = 0;
    quint8 reg = (quint8)Register::CHIPID;

    struct i2c_msg messages[]
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &reg
        },
        {
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = &id
        }
    };

    struct i2c_rdwr_ioctl_data payload =
    {
        .msgs = messages,
        .nmsgs = 2
    };

    if(ioctl(m_i2c, I2C_RDWR, &payload) < 1)
    {
        reportError("COULD NOT READ CHIP_ID REGISTER");
        m_errno = errno;
        return false;
    }

    reportEvent(QString("CHIP ID %1").arg(id, 2, 16));

    return (id == m_chipId);
}

bool QBMP280Backend::wake()
{
    if(m_controlData.mode != (quint8)Mode::FORCED)
        return false;

    if(!writeControlData(m_controlData)) //write forced mode to wake it up
        return false;

    //wait for ready
    quint8 status = 0;
    quint8 reg = (quint8)Register::STATUS;

    struct i2c_msg messages[]
    {
        {
            .addr = m_address,
            .flags = 0,
            .len = 1,
            .buf = &reg
        },
        {
            .addr = m_address,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = &status
        }
    };

    struct i2c_rdwr_ioctl_data payload =
    {
        .msgs = messages,
        .nmsgs = 2
    };

    qreal timeout = 50; //50ms timeout
    qreal timeStarted = QDateTime::currentMSecsSinceEpoch();
    qreal timeNow = 0;

    while(true)
    {
        timeNow = QDateTime::currentMSecsSinceEpoch();

        if((timeNow - timeStarted) >= timeout)
        {
            reportError("TIMEOUT WAKING UP CHIP");
            return false;
        }

        if(ioctl(m_i2c, I2C_RDWR, &payload) < 1)
        {
            reportError("COULD NOT READ STATUS REGISTER");

            //save the error
            m_errno = errno;

            //end i2c
            endI2C();

            return false;
        }

        reportEvent(QString("STATUS REGISTER: 0x%1 (QBMP280 @ %2:0x%3)").arg(status, 2, 16));

        if(!(status & (quint8)Mode::WAKING) && !(status & 1))
            break;

        QThread::msleep(2);
    }

    return true;
}

bool QBMP280Backend::startI2C()
{
    if(m_i2c < 0)
    {
        if((m_i2c = open(m_bus.toStdString().c_str(), O_RDWR)) < 0)
        {
            reportError("COULD NOT OPEN I2C BUS");
            m_errno = errno;

            return false;
        }
    }

    if(ioctl(m_i2c, I2C_SLAVE, m_address) < 0)
    {
        reportError("DEVICE NOT FOUND");
        m_errno = errno;

        //close and reset m_i2c
        endI2C();

        return false;
    }

    reportEvent("I2C STARTED");

    return true;
}

bool QBMP280Backend::endI2C()
{
    if(m_i2c < 0)
        return true; //already closed

    //close and reset m_i2c
    if(close(m_i2c) < 0)
    {
        reportError("COULD NOT CLOSE I2C BUS");
        m_i2c = -1;
        return false;
    }

    m_i2c = -1;
    reportEvent("I2C ENDED");

    return true;
}

void QBMP280Backend::handleFault()
{
    //stop();
    reportError(QString("FAULT %1").arg(m_errno));
    //TODO
}

void QBMP280Backend::onSensorBusChanged()
{
    if(!m_initialized)
        return;

    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(sensor)
        m_bus = sensor->bus();
}

void QBMP280Backend::onSensorAddressChanged()
{
    if(!m_initialized)
        return;

    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(sensor)
        m_address = sensor->address();
}

void QBMP280Backend::onSensorPowerModeChanged()
{
    if(!m_initialized)
        return;

    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(!sensor)
        return;

    ControlRegisterData data = m_controlData;

    //set recommended over-sampling based on power mode
    switch(sensor->powerMode())
    {
    case QBMP280::PowerMode::UltraLowPower:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X1;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::LowPower:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X2;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::Normal:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X4;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::HighResolution:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X8;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X1;
        break;
    case QBMP280::PowerMode::UltraHighResolution:
        data.pressureOverSampling = (quint8)QBMP280::OverSampling::X16;
        data.temperatureOverSampling = (quint8)QBMP280::OverSampling::X2;
        break;
    };

    if(!startI2C() || !writeControlData(data) || !endI2C())
    {
        handleFault();
        return;
    }

    m_controlData = data;
}

void QBMP280Backend::onSensorSeaLevelPressureChanged()
{
    if(!m_initialized)
        return;

    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(sensor)
        m_seaLevelPressure = sensor->seaLevelPressure();
}

void QBMP280Backend::onSensorStandyDurationChanged()
{
    if(!m_initialized)
        return;

    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(!sensor)
        return;

    ConfigRegisterData data = m_config;
    data.standyDuration = (quint8)sensor->standbyDuration();

    if(!startI2C() || !writeConfiguration(data) || !endI2C())
    {
        handleFault();
        return;
    }

    m_config = data;
}

void QBMP280Backend::onSensorFilterChanged()
{
    if(!m_initialized)
        return;

    QBMP280 *sensor = qobject_cast<QBMP280*>(this->sensor());

    if(!sensor)
        return;

    ConfigRegisterData data = m_config;
    data.filterMode = (quint8)sensor->filterMode();

    if(!startI2C() || !writeConfiguration(data) || !endI2C())
    {
        handleFault();
        return;
    }

    m_config = data;
}

void QBMP280Backend::onSensorDataRateChanged()
{
    stop();
    start();
}

void QBMP280Backend::onSensorAlwaysOnChanged()
{
    if(!m_initialized)
        return;

    //use a copy in case the write faults
    ControlRegisterData data = m_controlData;

    if(sensor()->isAlwaysOn())
        data.mode = (quint8)Mode::NORMAL;
    else
        data.mode = (quint8)Mode::FORCED;

    if(!startI2C() || !writeControlData(data) || !endI2C())
    {
        handleFault();
        return;
    }

    m_controlData = data;
}

void QBMP280Backend::reportEvent(QString message)
{
    //report event if backendDebug is true
    if(m_backendDebug)
        qDebug() << QString("** %1 - (QBMP280@%2:0x%3)").arg(message).arg(m_bus).arg(m_address, 2, 16);
}

void QBMP280Backend::reportError(QString message)
{
    qDebug() << QString("!! ERROR: %1 - (QBMP280@%2:0x%3)").arg(message).arg(m_bus).arg(m_address, 2, 16);
}

void QBMP280Backend::newLine()
{
    //add newline if backendDebug is true
    if(m_backendDebug)
        qDebug() << "";
}
