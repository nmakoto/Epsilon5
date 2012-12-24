#include "../utils/uexception.h"
#include "minimapitem.h"
//------------------------------------------------------------------------------
TMinimapItem::TMinimapItem(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , WHRatio(1.0, 1.0)
{
}
//------------------------------------------------------------------------------
TMinimapItem::TMinimapItem(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
    , WHRatio(1.0, 1.0)
{
}
//------------------------------------------------------------------------------
QRectF TMinimapItem::boundingRect() const
{
    return pixmap().rect();
}
//------------------------------------------------------------------------------
void TMinimapItem::GenerateFromImage(const QImage& image, const QSize& maxSize)
{
    WHRatio = QPointF(1.0, 1.0);
    if (!image.width() || !image.height()) {
        throw UException("Illegal image size:"
                + QString::number(image.width()) + "x"
                + QString::number(image.height()));
        return;
    }

    // 1.0 means max size, other always less than 1
    WHRatio = QPointF(
            image.width() > image.height()
            ?  1.0 : (qreal)image.width() / image.height(),
            image.width() > image.height()
            ? (qreal)image.width() / image.height() : 1.0);
}
//------------------------------------------------------------------------------
QSize TMinimapItem::GetMinimapSize(quint16 maxMinimapSize) const
{
    return QSize(maxMinimapSize * WHRatio.x(), maxMinimapSize * WHRatio.y());
}
//------------------------------------------------------------------------------
