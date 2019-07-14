//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
//-----------------------------------------------------------------------------

class WeatherData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dayOfWeek READ dayOfWeek WRITE setDayOfWeek NOTIFY dataChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY dataChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY dataChanged)
    Q_PROPERTY(QString temperature READ temperature WRITE setTemperature NOTIFY dataChanged)

public:
    explicit WeatherData(QObject* parent = nullptr);
    WeatherData(const WeatherData& other);

    QString dayOfWeek() const;
    QString weatherIcon() const;
    QString weatherDescription() const;
    QString temperature() const;

    void setDayOfWeek(const QString &value);
    void setWeatherIcon(const QString &value);
    void setWeatherDescription(const QString &value);
    void setTemperature(const QString &value);

signals:
    void dataChanged();

private:
    QString _dayOfWeek;
    QString _weather;
    QString _weatherDescription;
    QString _temperature;
};

Q_DECLARE_METATYPE(WeatherData)
