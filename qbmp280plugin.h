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

#ifndef QBMP28_PLUGIN_H
#define QBMP28_PLUGIN_H

#include <QObject>
#include <QSensorBackendFactory>
#include <QSensorPluginInterface>
#include <QSensorChangesInterface>

#include "QBMP280_global.h"
#include "qbmp280backend.h"
#include "qbmp280.h"

class QBMP28__EXPORT QBMP280Plugin : public QObject, public QSensorPluginInterface, public QSensorChangesInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0")
    Q_INTERFACES(QSensorPluginInterface QSensorChangesInterface)
public:
    void registerSensors() override
    {
        QSensor::defaultSensorForType(QBMP280::sensorType);
        QSensorManager::registerBackend(QBMP280::sensorType, QBMP280Backend::id, this);
    }

    void sensorsChanged() override
    {
        //register backend on initial load
        if(!QSensorManager::isBackendRegistered(QBMP280::sensorType, QBMP280Backend::id))
            QSensorManager::registerBackend(QBMP280::sensorType, QBMP280Backend::id, this);
    }

    QSensorBackend *createBackend(QSensor *sensor) override
    {
        if (sensor->identifier() == QBMP280Backend::id)
            return new QBMP280Backend(sensor);

        return 0;
    }
};

#endif // QBMP28_PLUGIN_H
