//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QUrl>
#include "Common/Logging.h"
#include "ImageService.h"
//-----------------------------------------------------------------------------

ImageService::ImageService(QObject* parent)
   : QAbstractListModel(parent)
   , m_networkAccessManager(new QNetworkAccessManager)
{
   qCDebug(imageServiceLog()) << "ImageService::ImageService()";

   connect(&m_imageListUpdateTimer, &QTimer::timeout, [this](){ updateJSONImageList(); });
   updateJSONImageList();
   m_imageListUpdateTimer.start(24 * 60 * 60 * 1000); // TODO: Add to settings.
}

QVariant ImageService::data(const QModelIndex& index, int role) const
{
   const int row = index.row();
   const int column = index.column();

   if (index.isValid() &&
       isRowValid(row) &&
       isColumnValid(column)) {

      switch (role) {
         case NameRole: return m_images.at(row)->name();
         case ImageRole: return m_images.at(row)->isCached() ? "file:" + m_images.at(row)->cachedFile() : "/Assets/error.jpg";
         case UseCountRole: return m_images.at(row)->accessCount();
      }
   }
   return QVariant();
}

int ImageService::rowCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent)
   return m_images.size();
}

QHash<int, QByteArray> ImageService::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[NameRole] = "nameRole";
   roles[ImageRole] = "imageRole";
   roles[UseCountRole] = "useCountRole";
   return roles;
}

Q_INVOKABLE void ImageService::updateImage()
{
   qCDebug(imageServiceLog()) << "void ImageService::updateImage()";

   // Return new random image
   std::uniform_int_distribution<> dist(0, m_images.size() - 1);
   const size_t imageIndex = dist(*QRandomGenerator::global());

   if (m_currentImageIndex != imageIndex) {
      if (m_images.at(imageIndex)->isCached()) {
         qCDebug(imageServiceLog()) << "Cache - hit: " + m_images.at(imageIndex)->name();
         m_currentImageIndex = imageIndex;
         emit imageChanged();
      } else {
         qCDebug(imageServiceLog()) << "Cache - miss: " + m_images.at(imageIndex)->name();
         downloadImage(imageIndex);
      }
   }
}

Image* ImageService::image() const
{
   return isIndexValid() ? m_images.at(m_currentImageIndex.value()) : nullptr;
}

Image* ImageService::image(int row) const
{
   return isRowValid(row) ? m_images.at(row) : nullptr;
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
   qCDebug(imageServiceLog()) << "ImageService::downloadImage(" << imageIndex << ")";

   QNetworkReply* rep = m_networkAccessManager->get(QNetworkRequest(m_images.at(imageIndex)->url()));
   connect(rep, &QNetworkReply::finished, this, [this, rep, imageIndex]() { handleImageDownloadNetworkData(rep, imageIndex); });
}

bool ImageService::isIndexValid() const
{
   return m_currentImageIndex.has_value() && (m_currentImageIndex < m_images.size());
}

bool ImageService::isRowValid(int index) const
{
   return ((index >= 0) && (index < rowCount()));
}

bool ImageService::isColumnValid(int index) const
{
   return ((index >= 0) && (index < 3));
}

void ImageService::handleImageListNetworkData(QNetworkReply* networkReply)
{
   qCInfo(imageServiceLog()) << "ImageService::handleNetworkData(QNetworkReply* networkReply)";

   if (!networkReply) {
      qCWarning(imageServiceLog()) << "Network error";
      return;
   }

   if (!networkReply->error()) {
      QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

      if (document.isNull()) {
         qCWarning(imageServiceLog()) << "Failed to create JSON document.";
         return;
      }

      if (!document.isObject()) {
         qCWarning(imageServiceLog()) << "JSON is not and object.";
         return;
      }

      QJsonObject jsonObject = document.object();
      if (jsonObject.isEmpty()) {
         qCWarning(imageServiceLog()) << "JSON object is empty.";
         return;
      }

      QVariantMap jsonMap = jsonObject.toVariantMap();
      const int newNumberOfImages = jsonMap["number_of_images"].toInt();
      const QDateTime& newLastModified = jsonMap["last_modified"].toDateTime();

      if ((numberOfImages() != newNumberOfImages) || (lastModified() < newLastModified)) {

         qCDebug(imageServiceLog()) << "Found" << newNumberOfImages - numberOfImages() << "new images.";

         setLastModified(newLastModified);

         for (const auto& record : jsonMap["image_list"].toList()) {
            QString url = record.toMap().value("name").toString();
            Image* image = new Image(QString(url));
            beginInsertRows(QModelIndex(), static_cast<int>(m_images.size()), static_cast<int>(m_images.size()));
            m_images.emplace_back(std::move(image));
            endInsertRows();
         }
      } else {
         qCDebug(imageServiceLog()) << "No new images found.";
         return;
      }
   }

   emit imageListUpdated();
}

void ImageService::handleImageDownloadNetworkData(QNetworkReply* networkReply, int imageIndex)
{
   qCDebug(imageServiceLog()) << "ImageService::handleNetworkData(" << networkReply << ", " << imageIndex << ")";

   if (!networkReply) {
      qCWarning(imageServiceLog()) << "Network error";
      return;
   }

   if (!networkReply->error()) {

      const QString imageDataDirectory = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/imageData"; // TODO: constants
      if (!QDir(imageDataDirectory).exists()) {
         if (!QDir().mkpath(imageDataDirectory)) {
            qCWarning(imageServiceLog()) << "Could not create directory: " + imageDataDirectory;
            return;
         }
      }
      if (m_images.at(imageIndex)->cacheLocalFile(networkReply->readAll())) {
         m_currentImageIndex = imageIndex;
         emit imageChanged();
      }
   }
}
