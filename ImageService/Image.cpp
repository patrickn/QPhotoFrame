//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <QFile>
#include <QStandardPaths>

#include "Image.h"
#include "easyexif/exif.h"
//-----------------------------------------------------------------------------

bool Image::isCached() const
{
   // TODO: Check if previous run has left a file behind which needs updating.
   return QFile::exists(m_cachedFile);
}

bool Image::cacheLocalFile(const QByteArray& byteArray)
{
   using namespace easyexif;
   m_downloaded = QDateTime::currentDateTime();

   EXIFInfo info;
   if (int code = info.parseFrom((unsigned char *)byteArray.data(), byteArray.size())){
      qDebug() << "Error parsing EXIF: code " << code;
      return false;
   }

   m_timestamp = QDateTime::fromString(info.DateTime.c_str(), "yyyy:MM:dd HH:mm:ss");
   m_latitude = info.GeoLocation.Latitude;
   m_longitude = info.GeoLocation.Longitude;
   m_altitude = info.GeoLocation.Altitude;

   const QString imageDataDirectory = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/imageData"; // TODO: constants
   QImage localFile = QImage::fromData(byteArray);
   m_cachedFile = imageDataDirectory + "/" + m_name;
   return localFile.save(m_cachedFile);
}
