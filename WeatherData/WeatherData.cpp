//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include "WeatherData.h"
//-----------------------------------------------------------------------------

WeatherData::WeatherData(QObject* parent)
    : QObject(parent),
      _src(nullptr),
      _nam(nullptr),
      _ns(nullptr),
      _nErrors(0)
{
    _nam = new QNetworkAccessManager;

    QNetworkConfigurationManager ncm;
    _ns = new QNetworkSession(ncm.defaultConfiguration(), this);
    connect(_ns, SIGNAL(opened()), this, SLOT(networkSessionOpened()));
    if (_ns->isOpen()) {
        this->networkSessionOpened();
    }
    _ns->open();
}

WeatherData::~WeatherData()
{
    _ns->close();
    if (_src) {
        _src->stopUpdates();
    }
}

bool WeatherData::ready() const
{
    return _ready;
}

bool WeatherData::hasSource() const
{
    return (_src != nullptr);
}

void WeatherData::hadError(bool tryAgain)
{
    qDebug() << "hadError, will " << (tryAgain ? "" : "not ") << "rety";
}

void WeatherData::refreshWeather()
{
    qDebug() << "WeatherData::refreshWeather()";
    qDebug() << "City: " << _city;
}

bool WeatherData::hasValidCity() const
{
    return (!(_city.isEmpty()) && _city.size() > 1 && _city != "");
}

QString WeatherData::city() const
{
    return _city;
}

void WeatherData::setCity(const QString& value)
{
    _city = value;
    emit cityChanged();
    refreshWeather();
}

void WeatherData::queryCity()
{
    qDebug() << "WeatherData::queryCity()";

    // TODO: Don't update more than once a minute to keep server load low

    QString latitude, longitude;
    longitude.setNum(_coord.longitude());
    latitude.setNum(_coord.latitude());

    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("lat", latitude);
    query.addQueryItem("lon", longitude);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", QStringLiteral("36496bad1955bf3365448965a42b9eac"));
    url.setQuery(query);
    qDebug() << "submitting API request";

    QNetworkReply* rep = _nam->get(QNetworkRequest(url));
    // connect up the signal right away
    connect(rep, &QNetworkReply::finished, this, [this, rep]() {
        handleGeoNetworkData(rep);
    });
}

void WeatherData::networkSessionOpened()
{
    _src = QGeoPositionInfoSource::createDefaultSource(this);

    if (_src) {
        connect(_src, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        connect(_src, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(positionError(QGeoPositionInfoSource::Error)));
        _src->startUpdates();
    } else {
        qDebug() << "Could not create default source.";
    }
}

void WeatherData::positionUpdated(QGeoPositionInfo gpsPos)
{
    qDebug() << "WeatherData::positionUpdated(...)";

    _coord = gpsPos.coordinate();
    queryCity();
}

void WeatherData::positionError(QGeoPositionInfoSource::Error e)
{
    qDebug() << "WeatherData::positionError(...)";
}

void WeatherData::handleGeoNetworkData(QNetworkReply* networkReply)
{
    qDebug() << "WeatherData::handleGeoNetworkData(...)";

    if (!networkReply) {
        hadError(false); // should retry?
        return;
    }

    if (!networkReply->error()) {
        _nErrors = 0;

        //convert coordinates to city name
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        QJsonObject jo = document.object();
        QJsonValue jv = jo.value(QStringLiteral("name"));

        const QString city = jv.toString();
        qDebug() << "got city: " << city;
        if (city != _city) {
            _city = city;
            emit cityChanged();
            refreshWeather();
        }
    } else {
        hadError(true);
    }
    networkReply->deleteLater();
}
