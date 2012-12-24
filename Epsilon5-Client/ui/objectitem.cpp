#include <qmath.h>
#include "objectitem.h"
//------------------------------------------------------------------------------
const qreal RAD_CONVERT = 180 / M_PI;
//------------------------------------------------------------------------------
TObjectItem::TObjectItem(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
}
//------------------------------------------------------------------------------
TObjectItem::TObjectItem(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
}
//------------------------------------------------------------------------------
QRectF TObjectItem::boundingRect() const
{
    return pixmap().rect();
}
//------------------------------------------------------------------------------
void TObjectItem::setPos(const QPointF pos)
{
    setPos(pos.x(), pos.y());
}
//------------------------------------------------------------------------------
void TObjectItem::setPos(qreal x, qreal y)
{
    setTransformOriginPoint(0, 0);
    QGraphicsPixmapItem::setPos(x - center().x(), y - center().y());
    setTransformOriginPoint(center());
}
//------------------------------------------------------------------------------
QPointF TObjectItem::pos() const
{
    return QGraphicsPixmapItem::pos() + center();
}
//------------------------------------------------------------------------------
QPointF TObjectItem::center() const
{
    return QPointF(pixmap().width() / 2, pixmap().height() / 2);
}
//------------------------------------------------------------------------------
void TObjectItem::setRotation(qreal angle)
{
    setTransformOriginPoint(center());
    QGraphicsPixmapItem::setRotation(angle);
}
//------------------------------------------------------------------------------
void TObjectItem::setRotationRad(qreal rad)
{
    setRotation(rad * RAD_CONVERT);
}
//------------------------------------------------------------------------------
qreal TObjectItem::rotation() const
{
    return QGraphicsPixmapItem::rotation();
}
//------------------------------------------------------------------------------
qreal TObjectItem::rotationRad() const
{
    return QGraphicsPixmapItem::rotation() * RAD_CONVERT;
}
//------------------------------------------------------------------------------
