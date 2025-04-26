#include "qbmp280.h"
#include "qbmp280_p.h"

IMPLEMENT_READING(QBMP280Reading)

QBMP280::QBMP280(QObject *parent) : QSensor{sensorType, parent}
{
    qDebug() << "** CREATED SENSOR OBJECT";
}

QBMP280Reading *QBMP280::reading() const
{
    return qobject_cast<QBMP280Reading*>(QSensor::reading());
}

QString QBMP280::bus() const
{
    return m_bus;
}

void QBMP280::setBus(const QString &bus)
{
    if (m_bus == bus)
        return;

    m_bus = bus;
    emit busChanged();
}

quint8 QBMP280::address() const
{
    return m_address;
}

void QBMP280::setAddress(quint8 address)
{
    if (m_address == address)
        return;

    m_address = address;
    emit addressChanged();
}

QBMP280::Filter QBMP280::filterMode() const
{
    return m_filterMode;
}

void QBMP280::setFilterMode(Filter filterMode)
{
    if (m_filterMode == filterMode)
        return;

    m_filterMode = filterMode;
    emit filterModeChanged();
}

QBMP280::StandbyDuration QBMP280::standbyDuration() const
{
    return m_standbyDuration;
}

void QBMP280::setStandbyDuration(StandbyDuration standbyDuration)
{
    if (m_standbyDuration == standbyDuration)
        return;

    m_standbyDuration = standbyDuration;
    emit standbyDurationChanged();
}

qreal QBMP280::seaLevelPressure() const
{
    return m_seaLevelPressure;
}

void QBMP280::setSeaLevelPressure(qreal seaLevelPressure)
{
    if (qFuzzyCompare(m_seaLevelPressure, seaLevelPressure))
        return;

    m_seaLevelPressure = seaLevelPressure;
    emit seaLevelPressureChanged();
}

QBMP280::PowerMode QBMP280::powerMode() const
{
    return m_powerMode;
}

void QBMP280::setPowerMode(PowerMode powerMode)
{
    if (m_powerMode == powerMode)
        return;
    m_powerMode = powerMode;
    emit powerModeChanged();
}

bool QBMP280::backendDebug() const
{
    return m_backendDebug;
}

void QBMP280::setBackendDebug(bool backendDebug)
{
    if (m_backendDebug == backendDebug)
        return;
    m_backendDebug = backendDebug;
    emit backendDebugChanged();
}
