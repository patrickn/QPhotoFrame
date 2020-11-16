//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QUrl>

#include "ImageService.h"
//-----------------------------------------------------------------------------


ImageService::ImageService(QObject* parent)
   : QObject(parent),
     m_networkAccessManager(new QNetworkAccessManager)
{
   qDebug() << "ImageService::ImageService()";

   connect(&m_imageListUpdateTimer, &QTimer::timeout, [this](){ updateJSONImageList(); });
   updateJSONImageList();
   m_imageListUpdateTimer.start(24 * 60 * 60 * 1000); // TODO: Add to settings.
}

Q_INVOKABLE void ImageService::updateImage()
{
   qDebug() << "void ImageService::updateImage()";

   // Return new random image
   std::uniform_int_distribution<> dist(0, m_images.size() - 1);
   const size_t imageIndex = dist(*QRandomGenerator::global());

   if (m_currentImageIndex != imageIndex) {
      if (m_images.at(imageIndex)->isCached()) {
         qDebug() << "Cache - hit: " + m_images.at(imageIndex)->name();
         m_currentImageIndex = imageIndex;
         emit imageChanged();
      } else {
         qDebug() << "Cache - miss: " + m_images.at(imageIndex)->name();
         downloadImage(imageIndex);
      }
   }
}

Image ImageService::image() const
{
   return isIndexValid() ? *m_images.at(m_currentImageIndex.value()) : Image{};
}

void ImageService::setLastModified(const QDateTime& lastModified)
{
   if (m_lastModified != lastModified) {
      m_lastModified = lastModified;
      emit lastModifiedChanged();
   }
}

void ImageService::updateJSONImageList()
{
   QUrl url("https://www.neavey.net/image_list.json");  // TODO: Add to settings.

   QNetworkReply* rep = m_networkAccessManager->get(QNetworkRequest(url));
   connect(rep, &QNetworkReply::finished, this, [this, rep]() { handleImageListNetworkData(rep); });
}

void ImageService::downloadImage(int imageIndex)
{
   qDebug() << "ImageService::downloadImage(" << imageIndex << ")";

   QNetworkReply* rep = m_networkAccessManager->get(QNetworkRequest(m_images.at(imageIndex)->url()));
   connect(rep, &QNetworkReply::finished, this, [this, rep, imageIndex]() { handleImageDownloadNetworkData(rep, imageIndex); });
}

bool ImageService::isIndexValid() const
{
   return m_currentImageIndex.has_value() && (m_currentImageIndex < m_images.size());
}

void ImageService::handleImageListNetworkData(QNetworkReply* networkReply)
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
         return;
      }

      QVariantMap jsonMap = jsonObject.toVariantMap();
      const int newNumberOfImages = jsonMap["number_of_images"].toInt();
      const QDateTime& newLastModified = jsonMap["last_modified"].toDateTime();

      if ((numberOfImages() != newNumberOfImages) || (lastModified() < newLastModified)) {

         qDebug() << "Found" << newNumberOfImages - numberOfImages() << "new images.";

         setLastModified(newLastModified);

         for (const auto& record : jsonMap["image_list"].toList()) {
            QString url = record.toMap().value("name").toString();
            Image* image = new Image(QString(url));
            m_images.emplace_back(std::move(image));
         }
      } else {
         qDebug() << "No new images found.";
         return;
      }
   }

   emit imageListUpdated();
}

void ImageService::handleImageDownloadNetworkData(QNetworkReply* networkReply, int imageIndex)
{
   qDebug() << "ImageService::handleNetworkData(" << networkReply << ", " << imageIndex << ")";

   if (!networkReply) {
      qDebug() << "Network error";
      return;
   }

   if (!networkReply->error()) {

      const QString imageDataDirectory = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/imageData"; // TODO: constants
      if (!QDir(imageDataDirectory).exists()) {
         if (!QDir().mkpath(imageDataDirectory)) {
            qWarning() << "Could not create directory: " + imageDataDirectory;
            return;
         }
      }
      if (m_images.at(imageIndex)->cacheLocalFile(networkReply->readAll())) {
         m_currentImageIndex = imageIndex;
         emit imageChanged();
      }
   }
}
