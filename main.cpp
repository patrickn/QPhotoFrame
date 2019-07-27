//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QFile>
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include "WeatherModel/WeatherData.h"
#include "WeatherModel/WeatherModel.h"
//-----------------------------------------------------------------------------

void customLogMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

auto main(int argc, char *argv[]) -> int
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   qInstallMessageHandler(customLogMessageHandler);
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

void customLogMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1]").arg(dt);
   QString cat = QString(context.category).leftJustified(8, ' ', true);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("{dbg :%1} %2").arg(cat).arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("{warn:%1} %2").arg(cat).arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("{crit:%1} %2").arg(cat).arg(msg);
         break;
      case QtInfoMsg:
         txt += QString("{info:%1} %2").arg(cat).arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("{fatal:%1} %2").arg(cat).arg(msg);
         abort();
   }

   static const QDir logLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
   if (!logLocation.exists()) {
      logLocation.mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
   }

   QFile outFile(logLocation.path() + "/message.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}
