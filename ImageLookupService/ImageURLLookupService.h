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

public:
   explicit ImageURLLookupService(QObject* parent = nullptr);

   Q_INVOKABLE QString getImageURL() const;

private slots:
   void refreshFileList();
   void handleNetworkData(QNetworkReply* networkReply);

private:
   QStringList _imageFileList;
   QTimer _requestNewFileListTimer;
};
