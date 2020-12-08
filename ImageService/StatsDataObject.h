//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QUrl>
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
