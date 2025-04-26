/*
 * This file is part of the QubeSat distribution
 * Copyright (c) 2025 QubeSat Project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QBMP28_READING_H
#define QBMP28_READING_H

#include <QSensor>
#include <QSensorReading>

#include "QBMP280_global.h"

QT_BEGIN_NAMESPACE

class QBMP280Backend;

class QBMP280ReadingPrivate;

class QBMP28__EXPORT QBMP280Reading : public QSensorReading
{
    Q_OBJECT
    Q_PROPERTY(qreal altitude READ altitude)
    Q_PROPERTY(qreal pressure READ pressure)
    Q_PROPERTY(qreal temperature READ temperature)
    DECLARE_READING(QBMP280Reading)
public:

    qreal pressure() const;
    qreal altitude() const;
    qreal temperature() const;

    void setPressure(qreal pressure);

    void setAltitude(qreal altitude);

    void setTemperature(qreal temperature);
};

class QBMP28__EXPORT QBMP280Filter : public QSensorFilter
{
public:
    virtual bool filter(QBMP280Reading *reading) = 0;
private:
    bool filter(QSensorReading *reading) override { return filter(static_cast<QBMP280Reading*>(reading)); }
};

QT_END_NAMESPACE
#endif // QBMP28_READING_H
