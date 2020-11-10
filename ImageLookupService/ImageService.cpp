
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QRandomGenerator>
#include <QUrl>

#include "ImageService.h"


ImageService::ImageService(QObject* parent)
   : QObject(parent)
{
   qDebug() << "ImageService::ImageService()";

   connect(&m_imageListUpdateTimer, &QTimer::timeout, [this](){ updateJSONImageList(); });
   updateJSONImageList();
   m_imageListUpdateTimer.start(24 * 60 * 60 * 1000); // TODO: Add to settings.
}

QString ImageService::randomImage()
{
   const int availableImages = imageList().size();

   if (availableImages == 0) {
      return "/Assets/error.jpg";
   }

   std::uniform_int_distribution<> dist(0, availableImages - 1);
   const int imageNo = dist(*QRandomGenerator::global());
   auto item = m_imageList.begin();
   std::advance(item, imageNo);

   setLatitude(item->second.latitude);
   setLongitude(item->second.longitude);

   return item->first;
}

void ImageService::setLastModified(const QDateTime& lastModified)
{
   if (m_lastModified != lastModified) {
      m_lastModified = lastModified;
      emit lastModifiedChanged();
   }
}

void ImageService::setImageList(const std::map<QString, ImageRecord>& imageList)
{
   if (m_imageList != imageList) {
      m_imageList = imageList;
      emit imageListChanged();
      emit numberOfImagesChanged();
   }
}

void ImageService::setLatitude(double latitude)
{
   if (m_latitude != latitude) {
      m_latitude = latitude;
      emit latitudeChanged();
   }
}

void ImageService::setLongitude(double longitude)
{
   if (m_longitude != longitude) {
      m_longitude = longitude;
      emit longitudeChanged();
   }
}

void ImageService::updateJSONImageList()
{
   QUrl url("https://www.neavey.net/image_list.json");  // TODO: Add to settings.

   QNetworkAccessManager* nam = new QNetworkAccessManager(this);
   QNetworkReply* rep = nam->get(QNetworkRequest(url));
   connect(rep, &QNetworkReply::finished, this, [this, rep]() { handleNetworkData(rep); });
}

void ImageService::handleNetworkData(QNetworkReply* networkReply)
{
   qDebug() << "ImageService::handleNetworkData(QNetworkReply* networkReply)";

   if (!networkReply) {
      qDebug() << "Network error";
      return;
   }

   if (!networkReply->error()) {
      QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

      if (document.isNull()) {
         qDebug() << "Failed to create JSON document.";
         return;
      }

      if (!document.isObject()) {
         qDebug() << "JSON is not and object.";
         return;
      }

      QJsonObject jsonObject = document.object();
      if (jsonObject.isEmpty()) {
         qDebug() << "JSON object is empty.";
      }

      QVariantMap jsonMap = jsonObject.toVariantMap();
      const int newNumberOfImages = jsonMap["number_of_images"].toInt();
      const QDateTime& newLastModified = jsonMap["last_modified"].toDateTime();

      if ((numberOfImages() != newNumberOfImages) || (lastModified() < newLastModified)) {

         qDebug() << "Found" << newNumberOfImages - numberOfImages() << "new images.";

         setLastModified(newLastModified);

         std::map<QString, ImageRecord> newImageList;
         for (const auto& record : jsonMap["image_list"].toList()) {
            ImageRecord imageRecord;
            QString name = record.toMap().value("name").toString();
            imageRecord.latitude = record.toMap().value("lat").toDouble();
            imageRecord.longitude = record.toMap().value("lon").toDouble();
            imageRecord.altitude = record.toMap().value("alt").toDouble();
            imageRecord.timestamp = record.toMap().value("timestamp").toDateTime();
            newImageList.insert({name, imageRecord});
         }
         setImageList(newImageList);
      } else {
         qDebug() << "No new images found.";
      }
   }
   emit randomImageChanged();
}
