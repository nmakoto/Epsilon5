#pragma once
#include <QGraphicsPixmapItem>
//------------------------------------------------------------------------------
class TObjectItem : public QGraphicsPixmapItem
{
public:
    explicit TObjectItem(QGraphicsItem* parent = 0);
    explicit TObjectItem(const QPixmap& pixmap, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;
    void setPos(qreal x, qreal y);
    void setPos(const QPointF pos);
    QPointF pos() const;
    QPointF center() const;
    void setRotation(qreal angle);
    void setRotationRad(qreal rad);
    qreal rotation() const;
    qreal rotationRad() const;
};
//------------------------------------------------------------------------------
