#ifndef QBMP28__P_H
#define QBMP28__P_H

#include "QBMP280_global.h"

class QBMP28__EXPORT QBMP280ReadingPrivate
{
public:
    QBMP280ReadingPrivate() : temperature(0.0), pressure(0.0), altitude(0.0) { }

    qreal temperature;
    qreal pressure;
    qreal altitude;
};

#endif // QBMP28__P_H
