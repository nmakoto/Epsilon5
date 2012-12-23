#pragma once
#include <QGraphicsWidget>
//------------------------------------------------------------------------------
class TLayoutItem;
//------------------------------------------------------------------------------
class TFormItem : public QGraphicsWidget
{
public:
    TFormItem(QGraphicsItem* parent = 0);
    ~TFormItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    TLayoutItem* item;
};
//------------------------------------------------------------------------------
