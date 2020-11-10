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
   Q_PROPERTY(double latitude READ latitude NOTIFY latitudeChanged)
   Q_PROPERTY(double longitude READ longitude NOTIFY longitudeChanged)

public:
   explicit ImageService(QObject* parent = nullptr);

   QDateTime lastModified() const { return m_lastModified; }
   int numberOfImages() const { return m_imageList.size(); }
   std::map<QString, ImageRecord> imageList() const { return m_imageList; }
   QString randomImage();
   double latitude() const { return m_latitude; }
   double longitude() const { return m_longitude; }

   void setLastModified(const QDateTime& lastModified);
   void setImageList(const std::map<QString, ImageRecord>& imageList);
   void setLatitude(double latitude);
   void setLongitude(double longitude);

private slots:
   void handleNetworkData(QNetworkReply* networkReply);

signals:
   void lastModifiedChanged();
   void numberOfImagesChanged();
   void imageListChanged();
   void randomImageChanged();
   void latitudeChanged();
   void longitudeChanged();

private:
   void updateJSONImageList();

   QDateTime m_lastModified = QDateTime::currentDateTime();
   std::map<QString, ImageRecord> m_imageList;
   QTimer m_imageListUpdateTimer;
   double m_latitude = 0;
   double m_longitude = 0;
};
