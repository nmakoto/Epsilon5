#pragma once
#include "scene/scobject.h"
//------------------------------------------------------------------------------
namespace scene
{
//------------------------------------------------------------------------------
class SCPlayer : public SCObject
{
public:
    explicit SCPlayer(QGraphicsItem* parent = 0);
    SCPlayer(const QPixmap &pixmap, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget);

    void SetHp(quint16 hp);
    void SetName(const QString& name);

private:
    quint16 Hp = 100;
    QString Name;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
