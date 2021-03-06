//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QDateTime>
#include <QImage>
#include <QObject>
#include <QUrl>
//-----------------------------------------------------------------------------


class Image : public QObject
{
   Q_OBJECT

public:
   Image() = default;
   explicit Image(const QUrl& url, QObject* parent = nullptr)
      : QObject(parent)
      , m_url(url)
      , m_name(url.fileName()) {}

   Q_INVOKABLE QString name() const { return m_name; }
   Q_INVOKABLE QString sourcePath() { ++m_accessCount; return "file:" + m_cachedFile; }
   Q_INVOKABLE QString date() const { return m_timestamp.date().toString(); }
   Q_INVOKABLE QString time() const { return m_timestamp.time().toString(); }
   Q_INVOKABLE double latitude() const { return m_latitude; }
   Q_INVOKABLE QString latitudeStr() const { return m_latitudeStr; }
   Q_INVOKABLE double longitude() const { return m_longitude; }
   Q_INVOKABLE QString longitudeStr() const { return m_longitudeStr; }
   Q_INVOKABLE double altitude() const { return m_altitude; }

   QUrl url() const { return m_url; };
   bool isCached() const;
   bool cacheLocalFile(const QByteArray& data);
   QString cachedFile() const { return m_cachedFile; }
   int accessCount() const { return m_accessCount; }

private:
   QUrl m_url;
   QString m_name;
   QString m_cachedFile;
   QDateTime m_timestamp;
   double m_latitude = 0;
   QString m_latitudeStr;
   double m_longitude = 0;
   QString m_longitudeStr;
   double m_altitude = 0;

   // Image stats
   QDateTime m_downloaded;
   int m_accessCount = 0;
};

Q_DECLARE_METATYPE(Image*)
