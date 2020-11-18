//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include "Logging.h"
//-----------------------------------------------------------------------------

Q_LOGGING_CATEGORY(imageLog, "qpf.img")
Q_LOGGING_CATEGORY(imageServiceLog, "qpf.img.svc")
Q_LOGGING_CATEGORY(temperatureModuleLog, "qpf.temp.mod")
Q_LOGGING_CATEGORY(weatherDataLog, "qpf.wthr.data")
Q_LOGGING_CATEGORY(weatherModelLog, "qpf.wthr.mod")

void customLogMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1]").arg(dt);
   QString cat = QString(context.category).leftJustified(14, ' ', true);

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

   static const QDir logLocation = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/qphotoframe";
   if (!logLocation.exists()) {
      logLocation.mkdir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/qphotoframe");
   }

   QFile outFile(logLocation.path() + "/message.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}
