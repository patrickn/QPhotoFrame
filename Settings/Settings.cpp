//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QCoreApplication>
#include "Settings.h"
//-----------------------------------------------------------------------------

Settings::Settings(QObject* parent)
   : QObject(parent)
{
   QCoreApplication::setOrganizationName("Milhouse");
   QCoreApplication::setOrganizationDomain("milhouse.uk");
   QCoreApplication::setApplicationName("QPhotoFrame");

   // Read from file
   QSettings settings;
   m_imageListDownloadTimer = settings.value(ImageListDownloadTimerString, ImageListDownloadTimerDefault).toInt() * 1000;

//   settings.setValue(ImageListDownloadTimerStr, ImageListDownloadTimer);
}

void Settings::setImageListDownloadTimer(int msec)
{
   if (m_imageListDownloadTimer != msec) {
      m_imageListDownloadTimer = msec;
      // Write to file
      emit imageListDownloadTimerChanged();
   }
}

