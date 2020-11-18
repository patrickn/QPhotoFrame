//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QLoggingCategory>
//-----------------------------------------------------------------------------

Q_DECLARE_LOGGING_CATEGORY(imageLog)
Q_DECLARE_LOGGING_CATEGORY(imageServiceLog)
Q_DECLARE_LOGGING_CATEGORY(temperatureModuleLog)
Q_DECLARE_LOGGING_CATEGORY(weatherDataLog)
Q_DECLARE_LOGGING_CATEGORY(weatherModelLog)

void customLogMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
