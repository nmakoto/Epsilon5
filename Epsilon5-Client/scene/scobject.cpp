#include <qmath.h>
#include "scene/scobject.h"
//------------------------------------------------------------------------------
const qreal RAD_CONVERT = 180 / M_PI;
//------------------------------------------------------------------------------
using namespace scene;
//------------------------------------------------------------------------------
SCObject::SCObject(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
}
//------------------------------------------------------------------------------
SCObject::SCObject(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
}
//------------------------------------------------------------------------------
QRectF SCObject::boundingRect() const
{
    return pixmap().rect();
}
//------------------------------------------------------------------------------
void SCObject::setPos(const QPointF pos)
{
    setPos(pos.x(), pos.y());
}
//------------------------------------------------------------------------------
void SCObject::setPos(qreal x, qreal y)
{
    setTransformOriginPoint(0, 0);
    QGraphicsPixmapItem::setPos(x - center().x(), y - center().y());
    setTransformOriginPoint(center());
}
//------------------------------------------------------------------------------
QPointF SCObject::pos() const
{
    return QGraphicsPixmapItem::pos() + center();
}
//------------------------------------------------------------------------------
QPointF SCObject::center() const
{
    return QPointF(pixmap().width() / 2, pixmap().height() / 2);
}
//------------------------------------------------------------------------------
void SCObject::setRotation(qreal angle)
{
    setTransformOriginPoint(center());
    QGraphicsPixmapItem::setRotation(angle);
}
//------------------------------------------------------------------------------
void SCObject::setRotationRad(qreal rad)
{
    setRotation(rad * RAD_CONVERT);
}
//------------------------------------------------------------------------------
qreal SCObject::rotation() const
{
    return QGraphicsPixmapItem::rotation();
}
//------------------------------------------------------------------------------
qreal SCObject::rotationRad() const
{
    return QGraphicsPixmapItem::rotation() * RAD_CONVERT;
}
//------------------------------------------------------------------------------
