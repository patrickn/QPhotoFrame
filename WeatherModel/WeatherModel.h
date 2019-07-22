//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QElapsedTimer>
#include <QGeoPositionInfoSource>
#include <QNetworkAccessManager>
#include <QNetworkSession>
#include <QObject>
#include <QTimer>
#include "WeatherData.h"
//-----------------------------------------------------------------------------


class WeatherModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(bool hasSource READ hasSource NOTIFY readyChanged)
    Q_PROPERTY(bool hasValidCity READ hasValidCity NOTIFY cityChanged)
    Q_PROPERTY(bool hasValidWeather READ hasValidWeather NOTIFY weatherChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(WeatherData* weather READ weather NOTIFY weatherChanged)

public:
    explicit WeatherModel(QObject* parent = nullptr);
    ~WeatherModel();

    static const int baseMsBeforeNewRequest = 5 * 1000; // 5 s, increased after each missing answer up to 10x

    bool ready() const;
    bool hasSource() const;
    bool hasValidCity() const;
    bool hasValidWeather() const;
    void hadError(bool tryAgain);

    QString city() const;
    void setCity(const QString& value);

    WeatherData* weather() const;
//    QQmlListProperty<WeatherData> forecast() const;

public slots:
    Q_INVOKABLE void refreshWeather();

private slots:
    void queryCity();
    void networkSessionOpened();
    void positionUpdated(QGeoPositionInfo gpsPos);
    void positionError(QGeoPositionInfoSource::Error e);
    void handleGeoNetworkData(QNetworkReply* networkReply);
    void handleWeatherNetworkData(QNetworkReply* networkReply);
    void handleForecastNetworkData(QNetworkReply* networkReply);

signals:
    void readyChanged();
    void cityChanged();
    void weatherChanged();

private:
    QGeoPositionInfoSource* _src;
    QGeoCoordinate _coord;
    QNetworkAccessManager* _nam;
    QNetworkSession *_ns;
    WeatherData _now;
    QList<WeatherData*> _forecast;
    QString _city;
    bool _ready;
    int _nErrors;
    int _minMsBeforeNewRequest;
    QElapsedTimer _throttle;
    QTimer _delayedCityRequestTimer;
    QTimer _requestNewWeatherTimer;
};
