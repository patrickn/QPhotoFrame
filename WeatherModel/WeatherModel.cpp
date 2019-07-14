//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include "WeatherModel.h"
//-----------------------------------------------------------------------------

WeatherModel::WeatherModel(QObject* parent)
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

WeatherModel::~WeatherModel()
{
    _ns->close();
    if (_src) {
        _src->stopUpdates();
    }
}

bool WeatherModel::ready() const
{
    return _ready;
}

bool WeatherModel::hasSource() const
{
    return (_src != nullptr);
}

void WeatherModel::hadError(bool tryAgain)
{
    qDebug() << "hadError, will " << (tryAgain ? "" : "not ") << "rety";
}

void WeatherModel::refreshWeather()
{
    qDebug() << "WeatherData::refreshWeather()";

    if (_city.isEmpty()) {
        qDebug() << "Cannot refresh weather, no city";
        return;
    }

    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;

    query.addQueryItem("q", _city);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", QStringLiteral("36496bad1955bf3365448965a42b9eac"));
    url.setQuery(query);

    QNetworkReply* rep = _nam->get(QNetworkRequest(url));
    connect(rep, &QNetworkReply::finished, this, [this, rep]() {
        handleWeatherNetworkData(rep);
    });
}

bool WeatherModel::hasValidCity() const
{
    return (!(_city.isEmpty()) && _city.size() > 1 && _city != "");
}

QString WeatherModel::city() const
{
    return _city;
}

void WeatherModel::setCity(const QString& value)
{
    _city = value;
    emit cityChanged();
    refreshWeather();
}

void WeatherModel::queryCity()
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

void WeatherModel::networkSessionOpened()
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

void WeatherModel::positionUpdated(QGeoPositionInfo gpsPos)
{
    qDebug() << "WeatherData::positionUpdated(...)";

    _coord = gpsPos.coordinate();
    queryCity();
}

void WeatherModel::positionError(QGeoPositionInfoSource::Error e)
{
    qDebug() << "WeatherData::positionError(...)";
}

void WeatherModel::handleGeoNetworkData(QNetworkReply* networkReply)
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

void WeatherModel::handleWeatherNetworkData(QNetworkReply* networkReply)
{
    qDebug() << "WeatherData::handleWeatherNetworkData(...)";

    if (!networkReply) {
        return;
    }

    if (!networkReply->error()) {
//        foreach (WeatherData* inf, _forecast) {
//            delete inf;
//        }
//        _forecast.clear();

        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        if (document.isObject()) {
            QJsonObject obj = document.object();
            QJsonObject tempObject;
            QJsonValue val;

            if (obj.contains(QStringLiteral("weather"))) {
                val = obj.value(QStringLiteral("weather"));
                QJsonArray weatherArray = val.toArray();
                val = weatherArray.at(0);
                tempObject = val.toObject();
//                _now.setWeatherDescription(tempObject.value(QStringLiteral("description")).toString());
//                _now.setWeatherIcon(tempObject.value("icon").toString());
            }
            if (obj.contains(QStringLiteral("main"))) {
                val = obj.value(QStringLiteral("main"));
                tempObject = val.toObject();
                val = tempObject.value(QStringLiteral("temp"));
//                _now.setTemperature(niceTemperatureString(val.toDouble()));
            }
        }
    }
    networkReply->deleteLater();

    //retrieve the forecast
//    QUrl url("http://api.openweathermap.org/data/2.5/forecast/daily");
//    QUrlQuery query;

//    query.addQueryItem("q", d->city);
//    query.addQueryItem("mode", "json");
//    query.addQueryItem("cnt", "5");
//    query.addQueryItem("APPID", d->app_ident);
//    url.setQuery(query);

//    QNetworkReply *rep = d->nam->get(QNetworkRequest(url));
//    // connect up the signal right away
//    connect(rep, &QNetworkReply::finished, this, [this, rep]() {
//        handleForecastNetworkData(rep);
//    });
}

void WeatherModel::handleForecastNetworkData(QNetworkReply* networkReply)
{
    qDebug() << "WeatherData::handleForecastNetworkData(...)";

}
