//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QUrl>

#include "ImageURLLookupService.h"
//-----------------------------------------------------------------------------

Q_LOGGING_CATEGORY(imageLookupLog, "lookup")

ImageURLLookupService::ImageURLLookupService(QObject* parent)
   : QObject(parent)
{
   qDebug() << "ImageURLLookupService::ImageURLLookupService(QObject* parent)";

   connect(&m_imageListRefreshTimer, &QTimer::timeout, [this](){ refreshImageList(); });
   m_imageListRefreshTimer.start(24 * 60 * 60 * 1000);

   // Get the initial image list.
   refreshImageList();

   connect(&m_imageRefreshTimer, &QTimer::timeout, [this](){ refreshImage(); });
   m_imageRefreshTimer.start(5000);
}

void ImageURLLookupService::refreshImage()
{
   qDebug() << "ImageURLLookupService::refreshImage()";

   if (0 == m_imageList.size()) {
      qDebug() << "   <<< ";
      return;
   }

   std::uniform_int_distribution<> dist(0, m_imageList.size() - 1);
   const int line_no = dist(*QRandomGenerator::global());
   setImageURL("https://www.neavey.net/" + m_imageList[line_no]);
   qDebug() << "   <<< " + imageURL();
}

void ImageURLLookupService::setImageURL(const QString& imageURL)
{
   qDebug() << "ImageURLLookupService::setImageURL(" << imageURL << ")";

   if (m_imageURL != imageURL)
   {
      m_imageURL = imageURL;
      emit imageURLChanged();
   }
}

void ImageURLLookupService::refreshImageList()
{
   qDebug() << "ImageURLLookupService::refreshFileList()";

   QUrl url("https://www.neavey.net/index.txt");

   QNetworkAccessManager* nam = new QNetworkAccessManager(this);
   QNetworkReply* rep = nam->get(QNetworkRequest(url));
   // connect up the signal right away
   connect(rep, &QNetworkReply::finished, this, [this, rep]() { handleNetworkData(rep); });
}

void ImageURLLookupService::handleNetworkData(QNetworkReply* networkReply)
{
   qDebug() << "ImageURLLookupService::handleNetworkData(QNetworkReply* networkReply)";

   if (!networkReply) {
      qCWarning(imageLookupLog) << "Network error";
      return;
   }

   size_t count = 0;
   if (!networkReply->error()) {
      auto all = networkReply->readAll();
      QList<QByteArray> lines = all.split('\n');
      foreach (const QByteArray& line, lines) {
         if (!m_imageList.contains(line)) {
            m_imageList.push_back(line);
            ++count;
         }
      }
      qCDebug(imageLookupLog) << "found" << count << "new images (total listed images:" << m_imageList.size() << ")";
   }
   else {
      qCWarning(imageLookupLog) << "Network error: " << networkReply->error();
   }
   networkReply->deleteLater();
}
