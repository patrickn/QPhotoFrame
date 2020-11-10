//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QObject>
#include <QTimer>

#include "DS18B20.h"
//-----------------------------------------------------------------------------


class TemperatureModule : public QObject
{
   Q_OBJECT
   Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged)

public:
   explicit TemperatureModule(QObject *parent = nullptr);

   double temperature() const { return m_internalTemperature; }

   void setTemperature(double internalTemperature);

signals:
   void temperatureChanged();

private:
   void readTemperature();

   DS18B20* m_ds18b20;
   QTimer m_readTemperatureTimer;
   double m_internalTemperature = 0;
};
