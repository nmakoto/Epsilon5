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
    : SCPlayer(QPixmap(), parent)
{
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
    rect.moveTopLeft(QPointF(rect.left() - 40, -HEALTHBAR_HEIGHT - HEALTHBAR_SPACE));
    rect.setHeight(rect.height() + HEALTHBAR_HEIGHT + HEALTHBAR_SPACE + 20);
    rect.setWidth(80 + rect.width());
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

    QFont oldFont = painter->font();
    painter->setFont(QFont("Ubuntu", 12, 72));
    painter->drawText(pixmap().rect().center().x()
            - painter->fontMetrics().width(Name) / 2,
            pixmap().height() + painter->fontInfo().pixelSize(), Name);
    painter->setFont(oldFont);
    painter->setPen(oldPen);

//    painter->drawRect(boundingRect());
}
//------------------------------------------------------------------------------
void SCPlayer::SetHp(quint16 hp)
{
    Hp = hp;
}
//------------------------------------------------------------------------------
void SCPlayer::SetName(const QString &name)
{
    Name = name.trimmed();
}
//------------------------------------------------------------------------------
