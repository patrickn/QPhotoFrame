//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "ImageService.h"
#include "ImageServiceSortProxy.h"
//-----------------------------------------------------------------------------

#include <QDebug>

//bool ImageServiceSortProxy::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
//{
//   qDebug() << "ImageServiceSortProxy::filterAcceptsRow(" << source_row << ", " << source_parent << ")";

//   return true;
//}

void ImageServiceSortProxy::updateImage()
{
   qDebug() << "/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\";

   ImageService* imageService = dynamic_cast<ImageService*>(sourceModel());
   imageService->updateImage();
}

Image* ImageServiceSortProxy::image() const
{
   qDebug() << "=============================================";

   ImageService* imageService = dynamic_cast<ImageService*>(sourceModel());
   return imageService->image();
}

void ImageServiceSortProxy::setSourceModel(QAbstractItemModel* sourceModel)
{
   QSortFilterProxyModel::setSourceModel(sourceModel);

   ImageService* imageService = dynamic_cast<ImageService*>(this->sourceModel());
   connect(imageService, &ImageService::imageListUpdated, this, [this](){imageListUpdated();});
   connect(imageService, &ImageService::imageChanged, this, [this](){imageChanged();});
}
