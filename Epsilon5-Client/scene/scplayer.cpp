#include <QPainter>
#include "scplayer.h"
//------------------------------------------------------------------------------
using namespace scene;
//------------------------------------------------------------------------------
const quint8 HEALTHBAR_WIDTH = 40;
const quint8 HEALTHBAR_HEIGHT = 4;
const quint8 HEALTHBAR_SPACE = 2;
//------------------------------------------------------------------------------
SCPlayer::SCPlayer(QGraphicsItem* parent)
    : SCObject(parent)
{
    setZValue(1);
}
//------------------------------------------------------------------------------
SCPlayer::SCPlayer(const QPixmap &pixmap, QGraphicsItem *parent)
    : SCObject(pixmap, parent)
{
    setZValue(1);
}
//------------------------------------------------------------------------------
QRectF SCPlayer::boundingRect() const
{
    QRectF rect(SCObject::boundingRect());
    rect.moveTop(-HEALTHBAR_HEIGHT - HEALTHBAR_SPACE);
    rect.setHeight(rect.height() + HEALTHBAR_HEIGHT + HEALTHBAR_SPACE);
    return rect;
}
//------------------------------------------------------------------------------
void SCPlayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    SCObject::paint(painter, option, widget);

    QRectF rect(pixmap().rect().topLeft().x(),
            pixmap().rect().topLeft().y() - HEALTHBAR_SPACE - HEALTHBAR_HEIGHT,
            pixmap().rect().width(), HEALTHBAR_HEIGHT);
    QRectF hpRect = rect;
    hpRect.setWidth(hpRect.width() * Hp / 100);

    QPen oldPen = painter->pen();
    painter->setPen(Qt::black);
    painter->fillRect(rect, Qt::darkRed);
    painter->fillRect(hpRect, Qt::red);
    painter->drawRect(rect);
    painter->setPen(oldPen);
}
//------------------------------------------------------------------------------
void SCPlayer::SetHp(quint16 hp)
{
    Hp = hp;
}
//------------------------------------------------------------------------------
