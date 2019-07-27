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
   _requestNewFileListTimer.setSingleShot(false);
   _requestNewFileListTimer.setInterval(60 * 1000); // TODO: Make File List refresh 24 hours

   connect(&_requestNewFileListTimer, SIGNAL(timeout()), this, SLOT(refreshFileList()));
   _requestNewFileListTimer.start();

   // Get the initial image list.
   refreshFileList();
}

QString ImageURLLookupService::getImageURL() const
{
   if (0 == _imageFileList.size()) {
      return {};
   }

   std::uniform_int_distribution<> dist(0, _imageFileList.size() - 1);
   const int line_no = dist(*QRandomGenerator::global());
   return "https://www.neavey.net/" + _imageFileList[line_no];
}

void ImageURLLookupService::refreshFileList()
{
   QUrl url("https://www.neavey.net/index.txt");

   QNetworkAccessManager* nam = new QNetworkAccessManager(this);
   QNetworkReply* rep = nam->get(QNetworkRequest(url));
   // connect up the signal right away
   connect(rep, &QNetworkReply::finished, this, [this, rep]() { handleNetworkData(rep); });
}

void ImageURLLookupService::handleNetworkData(QNetworkReply* networkReply)
{
   if (!networkReply) {
      qCWarning(imageLookupLog) << "Network error";
      return;
   }

   size_t count = 0;
   if (!networkReply->error()) {
      auto all = networkReply->readAll();
      QList<QByteArray> lines = all.split('\n');
      foreach (const QByteArray& line, lines) {
         if (!_imageFileList.contains(line)) {
            _imageFileList.push_back(line);
            ++count;
         }
      }
      qCDebug(imageLookupLog) << "add" << count << "new images to list (total images:" << _imageFileList.size() << ")";
   }
   else {
      qCWarning(imageLookupLog) << "Network error";
   }
   networkReply->deleteLater();
}
