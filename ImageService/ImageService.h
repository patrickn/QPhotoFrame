//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QTimer>
#include <map>

#include "Image.h"
//-----------------------------------------------------------------------------


class ImageService : public QObject
{
   Q_OBJECT
   Q_PROPERTY(Image* image READ image NOTIFY imageChanged)
   Q_PROPERTY(QList<QObject*> imageList READ imageList NOTIFY imageListChanged)

   Q_PROPERTY(QDateTime lastModified READ lastModified NOTIFY lastModifiedChanged)
   Q_PROPERTY(int numberOfImages READ numberOfImages NOTIFY numberOfImagesChanged)

public:
   explicit ImageService(QObject* parent = nullptr);

   Q_INVOKABLE void updateImage();

   Image* image() const;
   QList<QObject*> imageList();
   static bool dataListComparator(const QObject* left, const QObject* right);
   QDateTime lastModified() const { return m_lastModified; }
   int numberOfImages() const { return m_images.size(); }
   void setLastModified(const QDateTime& lastModified);

private slots:
   void handleImageListNetworkData(QNetworkReply* networkReply);
   void handleImageDownloadNetworkData(QNetworkReply* networkReply, int imageIndex);

signals:
   void imageChanged();
   void imageListUpdated();
   void lastModifiedChanged();
   void numberOfImagesChanged();
   void imageListChanged();

private:
   void updateJSONImageList();
   void downloadImage(int imageIndex);
   bool isIndexValid() const;

   QNetworkAccessManager* m_networkAccessManager;
   QDateTime m_lastModified = QDateTime::currentDateTime();
   QTimer m_imageListUpdateTimer;

   std::optional<size_t> m_currentImageIndex;
   std::vector<Image*> m_images;
};
