#include <QDebug>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include "uiminimap.h"
//------------------------------------------------------------------------------
using namespace ui;
//------------------------------------------------------------------------------
UIMinimap::UIMinimap(QGraphicsItem* parent)
    : UIMinimap(QPixmap(), parent)
{
}
//------------------------------------------------------------------------------
UIMinimap::UIMinimap(const QPixmap &background, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , Background(new QPixmap(background))
    , WHRatio(QPointF(1.0, 1.0))
    , MaximumSize(QSizeF(0, 0))
{
    setZValue(20);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(2);
    shadow->setOffset(2);
    shadow->setColor(Qt::black);

    setGraphicsEffect(shadow);
}
//------------------------------------------------------------------------------
UIMinimap::~UIMinimap()
{
    delete Background;
}
//------------------------------------------------------------------------------
QRectF UIMinimap::boundingRect() const
{
    return QRectF(0, 0, MaximumSize.width(), MaximumSize.height());
}
//------------------------------------------------------------------------------
void UIMinimap::SetMaximumSize(quint32 size)
{
    MaximumSize = QSizeF(size, size);
}
//------------------------------------------------------------------------------
void UIMinimap::SetMaximumSize(const QSize &size)
{
    MaximumSize = size;
}
//------------------------------------------------------------------------------
void UIMinimap::SetMapInfo(const QSize &mapSize, const QImage &mapBackground)
{
    MapSize = mapSize;
    WHRatio = QPointF(1.0, 1.0);
    if( mapSize.width() > 0 && mapSize.height() > 0 ) {
        qreal wh = (qreal)mapSize.width() / mapSize.height();
        if( mapSize.width() > mapSize.height() ) {
            WHRatio = QPointF( 1.0, 1 / wh );
        } else {
            WHRatio = QPointF( wh, 1.0 );
        }
    }

    *Background = QPixmap::fromImage(mapBackground.scaled(
            MaximumSize.width() * WHRatio.x(),
            MaximumSize.height() * WHRatio.y()));

    QPixmap transparent(Background->size());
    transparent.fill(Qt::transparent);

    QPainter p;
    p.begin(&transparent);
    p.setCompositionMode(QPainter::CompositionMode_Source);

    p.fillRect(transparent.rect(), Qt::darkGray);
//    p.drawPixmap(0, 0, *Background);
//    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
//    p.fillRect(transparent.rect(), QColor(0, 0, 0, 220));

    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(transparent.rect(), QColor(0, 0, 0, 80));
    p.end();

    *Background = transparent.copy();
}
//------------------------------------------------------------------------------
void UIMinimap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0, 0, *Background);
//    painter->fillRect(Background->rect(), Qt::darkGray);

    QPen oldPen = painter->pen();
    painter->setPen(QPen(QBrush(Qt::black), 2));
    painter->drawRect(Background->rect());
    painter->setPen(oldPen);

    if( Objects.count() <= 0 )
        return;

    for( auto key : Objects.keys() ) {
        for( auto it2 = Objects[key].begin(); it2 != Objects[key].end(); ++it2 ) {
            QGraphicsPixmapItem* item = qgraphicsitem_cast<QGraphicsPixmapItem*>(*it2);
            painter->drawPixmap(GetPosOnMap(item->pos()), item->pixmap().scaledToWidth(10));
        }
    }
}
//------------------------------------------------------------------------------
void UIMinimap::AddItem(const QString &groupName, QGraphicsItem *item)
{
    if( !Objects.keys().contains(groupName) ) {
        Objects[groupName] = TMapObjectsList();
    }

    Objects[groupName].append(item);
}
//------------------------------------------------------------------------------
void UIMinimap::RemoveItem(const QString &groupName, QGraphicsItem *item)
{
    if( !Objects.keys().contains(groupName)
            || !Objects[groupName].contains(item)) {
        return;
    }

    Objects[groupName].removeAll(item);
}
//------------------------------------------------------------------------------
QPointF UIMinimap::GetPosOnMap(const QPointF &point)
{
    return QPointF(
            ((qreal)point.x() / MapSize.width() + 0.5) * MaximumSize.width() * WHRatio.x(),
            ((qreal)point.y() / MapSize.height() + 0.5) * MaximumSize.height() * WHRatio.y());
}
//------------------------------------------------------------------------------
