//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QAbstractListModel>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QTimer>
#include <map>

#include "Image.h"
//-----------------------------------------------------------------------------

class StatsDataObject : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name NOTIFY nameChanged)
   Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
   Q_PROPERTY(int useCount READ useCount NOTIFY useCountChanged)

public:
   explicit StatsDataObject(const QString& name,
                            const QUrl& url,
                            int useCount,
                            QObject* parent = nullptr)
      : QObject(parent)
      , m_name(name)
      , m_url(url)
      , m_useCount(useCount) {}

   QString name() const { return m_name; }
   QUrl url() const { return m_url; }
   int useCount() const { return m_useCount; }

signals:
   void nameChanged();
   void urlChanged();
   void useCountChanged();

private:
   QString m_name;
   QUrl m_url;
   int m_useCount;
};


class ImageService : public QAbstractListModel
{
   Q_OBJECT
   Q_PROPERTY(Image* image READ image NOTIFY imageChanged)
   Q_PROPERTY(QList<QObject*> imageList READ imageList NOTIFY imageListChanged)

   Q_PROPERTY(QDateTime lastModified READ lastModified NOTIFY lastModifiedChanged)
   Q_PROPERTY(int numberOfImages READ numberOfImages NOTIFY numberOfImagesChanged)

public:
   explicit ImageService(QObject* parent = nullptr);

   enum ImageRoles {
      NameRole = Qt::UserRole + 1,
      ImageRole,
      UseCountRole
   };

   QVariant data(const QModelIndex& index, int role) const override;
   int rowCount(const QModelIndex& parent = QModelIndex()) const override;
   QHash<int, QByteArray> roleNames() const override;

   Q_INVOKABLE void updateImage();

   Image* image() const;
   QList<QObject*> imageList();
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
   bool isRowValid(int index) const;
   bool isColumnValid(int index) const;

   QNetworkAccessManager* m_networkAccessManager;
   QDateTime m_lastModified = QDateTime::currentDateTime();
   QTimer m_imageListUpdateTimer;

   std::optional<size_t> m_currentImageIndex;
   std::vector<Image*> m_images;
};
