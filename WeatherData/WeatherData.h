//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QGeoPositionInfoSource>
#include <QNetworkAccessManager>
#include <QNetworkSession>
#include <QObject>
//-----------------------------------------------------------------------------


class WeatherData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(bool hasSource READ hasSource NOTIFY readyChanged)
    Q_PROPERTY(bool hasValidCity READ hasValidCity NOTIFY cityChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)

public:
    explicit WeatherData(QObject *parent = nullptr);
    ~WeatherData();

    bool ready() const;
    bool hasSource() const;
    bool hasValidCity() const;
    void hadError(bool tryAgain);

    QString city() const;
    void setCity(const QString& value);

public slots:
    Q_INVOKABLE void refreshWeather();

private slots:
    void queryCity();
    void networkSessionOpened();
    void positionUpdated(QGeoPositionInfo gpsPos);
    void positionError(QGeoPositionInfoSource::Error e);
    void handleGeoNetworkData(QNetworkReply* networkReply);

signals:
    void readyChanged();
    void cityChanged();

private:
    QGeoPositionInfoSource* _src;
    QGeoCoordinate _coord;
    QNetworkAccessManager* _nam;
    QNetworkSession *_ns;
    QString _city;
    bool _ready;
    int _nErrors;
};

