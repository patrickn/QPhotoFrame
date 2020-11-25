//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <QSortFilterProxyModel>
#include "Image.h"
//-----------------------------------------------------------------------------

class ImageServiceSortProxy : public QSortFilterProxyModel
{
   Q_OBJECT
   Q_PROPERTY(Image* image READ image NOTIFY imageChanged)

public:
   ImageServiceSortProxy(QObject* parent = nullptr)
      : QSortFilterProxyModel(parent) {}

   Q_INVOKABLE void updateImage();
   Image* image() const;

   void setSourceModel(QAbstractItemModel* sourceModel) override;

   //   bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

signals:
   void imageChanged();
   void imageListUpdated();

   // QAbstractProxyModel interface
public:
};
