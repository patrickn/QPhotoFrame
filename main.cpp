//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "WeatherModel/WeatherData.h"
#include "WeatherModel/WeatherModel.h"
//-----------------------------------------------------------------------------

auto main(int argc, char *argv[]) -> int
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<WeatherData>("WeatherInfo", 1, 0, "WeatherData");
    qmlRegisterType<WeatherModel>("WeatherInfo", 1, 0, "WeatherModel");
    qRegisterMetaType<WeatherData>();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
