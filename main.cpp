//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <ImageService/ImageServiceSortProxy.h>
#include "Common/Logging.h"
#include "ImageService/ImageService.h"
#include "TemperatureModule/TemperatureModule.h"
#include "WeatherModel/WeatherData.h"
#include "WeatherModel/WeatherModel.h"
//-----------------------------------------------------------------------------

auto main(int argc, char *argv[]) -> int
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   qInstallMessageHandler(customLogMessageHandler);
   // Set default logging rules
//   QLoggingCategory::setFilterRules("*.debug=false\n"
//                                    "*.info=false\n");

   qmlRegisterType<WeatherData>("QPhotoFrame", 1, 0, "WeatherData");
   qmlRegisterType<WeatherModel>("QPhotoFrame", 1, 0, "WeatherModel");
   qmlRegisterType<TemperatureModule>("QPhotoFrame", 1, 0, "TemperatureModule");
   qRegisterMetaType<WeatherData>();

   QGuiApplication app(argc, argv);
   QQmlApplicationEngine engine;

   ImageService imageService;
   ImageServiceSortProxy sortFilterProxy;
   sortFilterProxy.setSourceModel(&imageService);
//   sortFilterProxy.setDynamicSortFilter(true);
//   sortFilterProxy.sort(0);

//   engine.rootContext()->setContextProperty("imageService", &imageService);
   engine.rootContext()->setContextProperty("sortFilterProxy", &sortFilterProxy);

   const QUrl url(QStringLiteral("qrc:/main.qml"));
   QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                    &app, [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl)
         QCoreApplication::exit(-1);
   }, Qt::QueuedConnection);
   engine.load(url);

   return app.exec();
}
