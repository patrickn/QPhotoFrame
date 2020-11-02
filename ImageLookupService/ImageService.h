#pragma once

#include <QMap>
#include <QObject>
#include <QNetworkReply>
#include <QTimer>


class ImageService : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QDateTime lastModified READ lastModified NOTIFY lastModifiedChanged)
   Q_PROPERTY(int numberOfImages READ numberOfImages NOTIFY numberOfImagesChanged)
   Q_PROPERTY(QStringList imageList NOTIFY imageListChanged)
   Q_PROPERTY(QString randomImage READ randomImage NOTIFY randomImageChanged)

public:
   explicit ImageService(QObject* parent = nullptr);

   QDateTime lastModified() const { return m_lastModified; }
   int numberOfImages() const { return m_imageList.size(); }
   QStringList imageList() const { return m_imageList; }
   QString randomImage();

   void setLastModified(const QDateTime& lastModified);
   void setImageList(const QStringList& imageList);

private slots:
   void handleNetworkData(QNetworkReply* networkReply);

signals:
   void lastModifiedChanged();
   void numberOfImagesChanged();
   void imageListChanged();
   void randomImageChanged();

private:
   void updateJSONImageList();

   QDateTime m_lastModified = QDateTime::currentDateTime();
   QStringList m_imageList;
   QTimer m_imageListUpdateTimer;
};
