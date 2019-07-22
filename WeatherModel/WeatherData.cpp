//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QDebug>
#include "WeatherData.h"
//-----------------------------------------------------------------------------


WeatherData::WeatherData(QObject* parent)
    : QObject(parent)
{
}

WeatherData::WeatherData(const WeatherData& other)
    : QObject(nullptr),
      _dayOfWeek(other._dayOfWeek),
      _weather(other._weather),
      _weatherDescription(other._weatherDescription),
      _temperature(other._temperature)
{
}

QString WeatherData::dayOfWeek() const
{
    return _dayOfWeek;
}

QString WeatherData::weatherIcon() const
{
    return _weather;
}

QString WeatherData::weatherDescription() const
{
    return _weatherDescription;
}

QString WeatherData::temperature() const
{
    return _temperature;
}

void WeatherData::setDayOfWeek(const QString& value)
{
    _dayOfWeek = value;
    emit dataChanged();
}

void WeatherData::setWeatherIcon(const QString& value)
{
    _weather = value;
    emit dataChanged();
}

void WeatherData::setWeatherDescription(const QString& value)
{
    _weatherDescription = value;
    emit dataChanged();
}

void WeatherData::setTemperature(const QString& value)
{
    qDebug() << "Set external temperature: " << value;
    _temperature = value;
    emit dataChanged();
}
