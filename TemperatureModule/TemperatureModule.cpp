//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QLoggingCategory>
#include "TemperatureModule.h"
//-----------------------------------------------------------------------------

Q_LOGGING_CATEGORY(temperatureModuleLog, "tempMod")

TemperatureModule::TemperatureModule(QObject *parent)
   : QObject(parent)
   , m_ds18b20(new DS18B20("28-00000acc55c2"))
{
   qCDebug(temperatureModuleLog()) << "TemperatureModule::TemperatureModule()";

   connect(&m_readTemperatureTimer, &QTimer::timeout, [this](){ readTemperature(); });
   readTemperature();
   m_readTemperatureTimer.start(60 * 1000); // TODO: Add to settings.
}

void TemperatureModule::setTemperature(double internalTemperature)
{
   if (m_internalTemperature != internalTemperature) {
      m_internalTemperature = internalTemperature;
      emit temperatureChanged();
   }
}

void TemperatureModule::readTemperature()
{
#ifdef __arm__
   setTemperature(m_ds18b20->getTemp());
#else
   setTemperature(99);
#endif
}

