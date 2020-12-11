//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QObject>
#include <QSettings>
//-----------------------------------------------------------------------------


class Settings final : public QObject
{
   const int ImageListDownloadTimerDefault = 24 * 60 * 60; // Seconds
   const QString ImageListDownloadTimerString = "Settings/ImageListDownloadTimer";

   Q_OBJECT
   Q_PROPERTY(int imageListDownloadTimer READ imageListDownloadTimer WRITE setImageListDownloadTimer NOTIFY imageListDownloadTimerChanged)

public:
   virtual ~Settings() override = default;

   static Settings& Instance()
   {
      static Settings instance;
      return instance;
   }

   int imageListDownloadTimer() const { return m_imageListDownloadTimer; }
   void setImageListDownloadTimer(int msec);

signals:
   void imageListDownloadTimerChanged();

private:
   explicit Settings(QObject* parent = nullptr);

   int m_imageListDownloadTimer = 24 * 60 * 60 * 1000;
};
