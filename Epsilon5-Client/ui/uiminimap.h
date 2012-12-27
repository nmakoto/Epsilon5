#pragma once
#include <QGraphicsItem>
//------------------------------------------------------------------------------
namespace ui
{
//------------------------------------------------------------------------------
class UIMinimap : public QGraphicsItem
{
    typedef QList<QGraphicsItem*> TMapObjectsList;
    typedef QHash<QString, TMapObjectsList> TGroupsHash;
public:
    explicit UIMinimap(QGraphicsItem *parent = 0);
    UIMinimap(const QPixmap& background, QGraphicsItem* parent = 0);
    ~UIMinimap();

    QRectF boundingRect() const;

    void SetMaximumSize(quint32 size);
    void SetMaximumSize(const QSize& size);
    void SetMapInfo(const QSize& mapSize, const QImage& mapBackground);

    void AddItem(const QString& groupName, QGraphicsItem* item);
    void RemoveItem(const QString& groupName, QGraphicsItem* item);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget);

protected:
    QPointF GetPosOnMap(const QPointF& point);

private:
    QPixmap* Background;
    TGroupsHash Objects;
    QPointF WHRatio;
    QSizeF MaximumSize;
    QSizeF MapSize;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
