//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "ImageService.h"
#include "ImageServiceSortProxy.h"
//-----------------------------------------------------------------------------

void ImageServiceSortProxy::updateImage()
{
   ImageService* imageService = dynamic_cast<ImageService*>(sourceModel());
   imageService->updateImage();
}

Image* ImageServiceSortProxy::image() const
{
   ImageService* imageService = dynamic_cast<ImageService*>(sourceModel());
   return imageService->image();
}

void ImageServiceSortProxy::setSourceModel(QAbstractItemModel* sourceModel)
{
   QSortFilterProxyModel::setSourceModel(sourceModel);

   // Signals to be forwarded via the proxy
   ImageService* imageService = dynamic_cast<ImageService*>(this->sourceModel());
   connect(imageService, &ImageService::imageListUpdated, this, [this](){imageListUpdated();});
   connect(imageService, &ImageService::imageChanged, this, [this](){imageChanged();});
}

bool ImageServiceSortProxy::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
   qDebug() << "ImageServiceSortProxy::filterAcceptsRow(" << source_row << ", " << source_parent << ")";

   ImageService* imageService = dynamic_cast<ImageService*>(sourceModel());
   const Image* image = imageService->image(source_row);

   qDebug() << "XXXXXXXX Image: " << image->name() << " count: " << image->accessCount();

   if (image && image->accessCount()) {
      return true;
   }

   return false;
}
