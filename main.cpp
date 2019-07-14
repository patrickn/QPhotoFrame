//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "WeatherData/WeatherData.h"
//-----------------------------------------------------------------------------

auto main(int argc, char *argv[]) -> int
{
   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   QGuiApplication app(argc, argv);

   qmlRegisterType<WeatherData>("WeatherData", 1, 0, "WeatherData");

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
