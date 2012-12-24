#pragma once
#include <QGraphicsPixmapItem>
//------------------------------------------------------------------------------
class TMinimapItem : public QGraphicsPixmapItem
{
public:
    explicit TMinimapItem(QGraphicsItem* parent = 0);
    explicit TMinimapItem(const QPixmap& pixmap, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;
    void GenerateFromImage(const QImage& image, const QSize& maxSize);
    QSize GetMinimapSize(quint16 maxMinimapSize) const;

private:
    QPointF WHRatio;
};
//------------------------------------------------------------------------------
