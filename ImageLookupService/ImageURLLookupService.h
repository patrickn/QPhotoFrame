//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <random>
#include <QNetworkReply>
#include <QObject>
#include <QRandomGenerator>
#include <QString>
#include <QTimer>
//-----------------------------------------------------------------------------

class ImageURLLookupService : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString imageURL READ imageURL NOTIFY imageURLChanged)

public:
   explicit ImageURLLookupService(QObject* parent = nullptr);


   QString imageURL() const { return m_imageURL; }
   void setImageURL(const QString& imageURL);

   void refreshImage();
   void refreshImageList();

private slots:
   void handleNetworkData(QNetworkReply* networkReply);

signals:
   void imageURLChanged();

private:
   QStringList m_imageList;
   QTimer m_imageRefreshTimer;
   QTimer m_imageListRefreshTimer;
   QString m_imageURL;
};
