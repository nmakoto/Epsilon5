#include <QDebug>
#include <QPainter>
#include <QGraphicsLinearLayout>
#include "ui/uilayoutitem.h"
#include "uistatistic.h"
//------------------------------------------------------------------------------
using namespace ui;
namespace ui {
//------------------------------------------------------------------------------
class UITextItem : public QGraphicsTextItem, public QGraphicsLayoutItem
{
public:
    UITextItem(QGraphicsItem* parent)
        : QGraphicsTextItem(parent)
        , QGraphicsLayoutItem()
    {
        setGraphicsItem(this);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsTextItem::paint(painter, option, widget);
    }

    QRectF boundingRect() const
    {
        return QRectF(QPointF(0, 0), geometry().size());
    }

    void setGeometry(const QRectF& geom)
    {
        prepareGeometryChange();
        QGraphicsLayoutItem::setGeometry(geom);
        setPos(geom.topLeft());
    }

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
    {
        switch (which) {
        case Qt::MinimumSize:
        case Qt::PreferredSize:
            return QGraphicsTextItem::boundingRect().size();
        case Qt::MaximumSize:
        default:
            break;
        }
        return constraint;
    }
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
UIStatistic::UIStatistic(QGraphicsItem* parent)
    : QGraphicsWidget(parent)
{
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->addStretch();
    UITextItem* textItem = new UITextItem(this);
    textItem->setHtml("<span style='background:rgba(0,0,0,0.6);"
            "color:#eee;border-radius:40px;'>"
            + tr("sdgklgsfksfhsklfsfsfsdgsg") + "</span>");
    layout->addStretch();
    layout->addItem(textItem);

    setLayout(layout);
    setFlag(QGraphicsItem::ItemIsPanel);
}
//------------------------------------------------------------------------------
