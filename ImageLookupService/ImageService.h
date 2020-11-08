#pragma once

#include <map>

#include <QDateTime>
#include <QNetworkReply>
#include <QObject>
#include <QTimer>

struct ImageRecord {
   double latitude;
   double longitude;
   double altitude;
   QDateTime timestamp;

   bool operator==(const ImageRecord& other) const
   {
      return (latitude == other.latitude) &&
             (longitude == other.longitude) &&
             (altitude == other.altitude) &&
             (timestamp == other.timestamp);
   }
};

class ImageService : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QDateTime lastModified READ lastModified NOTIFY lastModifiedChanged)
   Q_PROPERTY(int numberOfImages READ numberOfImages NOTIFY numberOfImagesChanged)
   Q_PROPERTY(QStringList imageList NOTIFY imageListChanged)
   Q_PROPERTY(QString randomImage READ randomImage NOTIFY randomImageChanged)
   Q_PROPERTY(QPair<double, double> imageLocation READ imageLocation NOTIFY imageLocationChanged)

public:
   explicit ImageService(QObject* parent = nullptr);

   QDateTime lastModified() const { return m_lastModified; }
   int numberOfImages() const { return m_imageList.size(); }
   std::map<QString, ImageRecord> imageList() const { return m_imageList; }
   QString randomImage();
   QPair<double, double> imageLocation() const { return m_imageLocation; }

   void setLastModified(const QDateTime& lastModified);
   void setImageList(const std::map<QString, ImageRecord>& imageList);

private slots:
   void handleNetworkData(QNetworkReply* networkReply);

signals:
   void lastModifiedChanged();
   void numberOfImagesChanged();
   void imageListChanged();
   void randomImageChanged();
   void imageLocationChanged();

private:
   void updateJSONImageList();

   QDateTime m_lastModified = QDateTime::currentDateTime();
   std::map<QString, ImageRecord> m_imageList;
   QTimer m_imageListUpdateTimer;
   QPair<double, double> m_imageLocation;
};
