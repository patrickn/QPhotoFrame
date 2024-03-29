//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QProcessEnvironment>
#include <QUrlQuery>
#include "Common/Logging.h"
#include "WeatherModel.h"
//-----------------------------------------------------------------------------

static const double ZERO_KELVIN = 273.15;

WeatherModel::WeatherModel(QObject* parent)
    : QObject(parent),
      _src(nullptr),
      _nam(nullptr),
      _ns(nullptr),
      _nErrors(0),
      _minMsBeforeNewRequest(baseMsBeforeNewRequest),
      m_weatherDataAppId(QProcessEnvironment::systemEnvironment().value("QPF_WEATHERDATA_APPID"))
{
   if (m_weatherDataAppId.isEmpty()) {
      qCCritical(weatherModelLog()) << "Critical error: QPF_WEATHERDATA_APPID not set. Unable to retrieve weather data.";
   }

   _delayedCityRequestTimer.setSingleShot(true);
   _delayedCityRequestTimer.setInterval(1000); // 1 s
   _requestNewWeatherTimer.setSingleShot(false);
   _requestNewWeatherTimer.setInterval(20 * 60 * 1000); // 20 mins
   _throttle.invalidate();

   connect(&_delayedCityRequestTimer, SIGNAL(timeout()), this, SLOT(queryCity()));
   connect(&_requestNewWeatherTimer, SIGNAL(timeout()), this, SLOT(refreshWeather()));
   _requestNewWeatherTimer.start();

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
    qCDebug(weatherModelLog) << "hadError, will " << (tryAgain ? "" : "not ") << "rety";
    _throttle.start();
    if (_nErrors < 10) {
        ++_nErrors;
    }
    _minMsBeforeNewRequest = (_nErrors + 1) * baseMsBeforeNewRequest;
    if (tryAgain) {
        _delayedCityRequestTimer.start();
    }
}

void WeatherModel::refreshWeather()
{
    if (_city.isEmpty()) {
        qCDebug(weatherModelLog) << "Cannot refresh weather, no city";
        return;
    }

    qCDebug(weatherModelLog) << "refreshing weather";
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;

    query.addQueryItem("q", _city);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", m_weatherDataAppId);
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

bool WeatherModel::hasValidWeather() const
{
    return hasValidCity() && (!(_now.weatherIcon().isEmpty()) &&
                              (_now.weatherIcon().size() > 1) &&
                              _now.weatherIcon() != "");
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

WeatherData* WeatherModel::weather() const
{
    return const_cast<WeatherData*>(&(_now));
}

void WeatherModel::queryCity()
{
    // Don't update more than once a minute to keep server load low
    if (_throttle.isValid() && _throttle.elapsed() < _minMsBeforeNewRequest) {
        qCDebug(weatherModelLog) << "delaying query of city";
        if (_delayedCityRequestTimer.isActive()) {
            _delayedCityRequestTimer.start();
        }
    }
    qCDebug(weatherModelLog) << "requested city";
    _throttle.start();
    _minMsBeforeNewRequest = (_nErrors + 1) * baseMsBeforeNewRequest;

    QString latitude, longitude;
    longitude.setNum(_coord.longitude());
    latitude.setNum(_coord.latitude());

    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("lat", latitude);
    query.addQueryItem("lon", longitude);
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", m_weatherDataAppId);
    url.setQuery(query);
    qCDebug(weatherModelLog) << "submitting API request";

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
        qCDebug(weatherModelLog) << "Could not create default source.";
    }
}

void WeatherModel::positionUpdated(QGeoPositionInfo gpsPos)
{
    qCDebug(weatherModelLog) << "WeatherModel::positionUpdated(...)";

    _coord = gpsPos.coordinate();
    queryCity();
}

void WeatherModel::positionError(QGeoPositionInfoSource::Error e)
{
   qCDebug(weatherModelLog) << "WeatherModel::positionError(...): " << e;
}

void WeatherModel::handleGeoNetworkData(QNetworkReply* networkReply)
{
    qCDebug(weatherModelLog) << "WeatherModel::handleGeoNetworkData(...)";

    if (!networkReply) {
        hadError(false); // should retry?
        return;
    }

    if (!networkReply->error()) {
        _nErrors = 0;
        if (!_throttle.isValid()) {
            _throttle.start();
        }
        _minMsBeforeNewRequest = baseMsBeforeNewRequest;

        //convert coordinates to city name
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        QJsonObject jo = document.object();
        QJsonValue jv = jo.value(QStringLiteral("name"));

        const QString city = jv.toString();
        qCDebug(weatherModelLog) << "got city: " << city;
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

static QString niceTemperatureString(double t)
{
    return QString::number((t - ZERO_KELVIN), 'f', 1)/* + QChar(0xB0)*/;
}

void WeatherModel::handleWeatherNetworkData(QNetworkReply* networkReply)
{
    qCDebug(weatherModelLog) << "WeatherModel::handleWeatherNetworkData(...)";

    if (!networkReply) {
        return;
    }

    if (!networkReply->error()) {
        foreach (WeatherData* inf, _forecast) {
            delete inf;
        }
        _forecast.clear();

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
                _now.setWeatherDescription(tempObject.value(QStringLiteral("description")).toString());
                _now.setWeatherIcon(tempObject.value("icon").toString());
            }
            if (obj.contains(QStringLiteral("main"))) {
                val = obj.value(QStringLiteral("main"));
                tempObject = val.toObject();
                val = tempObject.value(QStringLiteral("temp"));
                _now.setTemperature(niceTemperatureString(val.toDouble()));
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

    // Move this to handleForecastNetworkData
    emit weatherChanged();
}

void WeatherModel::handleForecastNetworkData(QNetworkReply* networkReply)
{
   Q_UNUSED(networkReply)

   qCDebug(weatherModelLog) << "WeatherModel::handleForecastNetworkData(...)";
}
