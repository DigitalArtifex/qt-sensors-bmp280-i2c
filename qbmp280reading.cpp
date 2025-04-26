#include "qbmp280reading.h"
#include "qbmp280_p.h"

qreal QBMP280Reading::pressure() const { return d->pressure; }

qreal QBMP280Reading::altitude() const { return d->altitude; }

qreal QBMP280Reading::temperature() const { return d->temperature; }

void QBMP280Reading::setPressure(qreal pressure)
{
    d->pressure = pressure;
}

void QBMP280Reading::setAltitude(qreal altitude)
{
    d->altitude = altitude;
}

void QBMP280Reading::setTemperature(qreal temperature)
{
    d->temperature = temperature;
}
