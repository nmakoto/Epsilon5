#pragma once
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>
//------------------------------------------------------------------------------
namespace ui
{
//------------------------------------------------------------------------------
class UILayoutItem : public QGraphicsItem, public QGraphicsLayoutItem
{
public:
    UILayoutItem(const QString& name, const QPixmap& pixmap,
            const QPixmap& hovered = QPixmap(), QGraphicsItem* parent = 0);
    ~UILayoutItem();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
            QWidget* widget);
    QRectF boundingRect() const;
    void setGeometry(const QRectF& geom);
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint) const;
    bool isHovered() const;
    void setHovered(bool value = true);
    const QString& name() const;

private:
    QPixmap* NormalPixmap;
    QPixmap* HoveredPixmap;
    bool Hovered;
    QString Name;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
