#include <QDebug>
#include <QGraphicsLinearLayout>
#include <QPen>
#include <QPainter>
#include "formitem.h"
//------------------------------------------------------------------------------
class TLayoutItem : public QGraphicsItem, public QGraphicsLayoutItem
{
public:
    TLayoutItem(const QPixmap& pixmap, qreal opacity = 1.0, QGraphicsItem* parent = 0)
        : QGraphicsItem(parent), QGraphicsLayoutItem()
        , Pixmap(new QPixmap(pixmap))
    {
        setGraphicsItem(this);
        setOpacity(opacity);
        setFlags(ItemIsFocusable | ItemIsSelectable);
    }
    ~TLayoutItem()
    {
        delete Pixmap;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(widget);
        Q_UNUSED(option);
        painter->drawPixmap(contentsRect().center() - Pixmap->rect().center(), *Pixmap);
#ifdef QT_DEBUG
        QPen oldPen = painter->pen();
        painter->setPen(QPen(QBrush(Qt::yellow),2));
        painter->drawRect(contentsRect());
        painter->setPen(oldPen);
#endif
    }

    QRectF boundingRect() const
    {
        return QRectF(QPointF(0,0), geometry().size());
    }

    void setGeometry(const QRectF &geom)
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
            return Pixmap->size();
        case Qt::MaximumSize:
            return QSizeF(3000, 3000);
        default:
            break;
        }
        return constraint;
    }

protected:
    void hoverEnterEvent(QGraphicsSceneMouseEvent *event) {
        qDebug() << "mouse hovers";
    }
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
        qDebug() << "mouse left";
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        qDebug() << "mouse press";
    }

private:
    QPixmap* Pixmap;
};
//------------------------------------------------------------------------------
TFormItem::TFormItem(QGraphicsItem* parent)
    : QGraphicsWidget(parent)
{
    QGraphicsLinearLayout* windowLayout = new QGraphicsLinearLayout(Qt::Vertical);
    windowLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    windowLayout->addStretch();
    item = new TLayoutItem(QPixmap("resources/menu-connect.png"), 0.9, this);
    windowLayout->setAlignment(item, Qt::AlignHCenter);
    windowLayout->addItem(item);
    item = new TLayoutItem(QPixmap("resources/menu-exit.png"), 0.9, this);
    windowLayout->setAlignment(item, Qt::AlignHCenter);
    windowLayout->addItem(item);
    windowLayout->addStretch();

    setLayout(windowLayout);
    setFlag(QGraphicsItem::ItemIsPanel);
}
//------------------------------------------------------------------------------
TFormItem::~TFormItem()
{
}
//------------------------------------------------------------------------------
void TFormItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    painter->fillRect(rect(), Qt::black);
#ifdef QT_DEBUG
    QPen oldPen = painter->pen();
    painter->setPen(QPen(QBrush(Qt::white),2));
    painter->drawRect(contentsRect());
    painter->setPen(oldPen);
#endif
}
//------------------------------------------------------------------------------
void TFormItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO;
}
//------------------------------------------------------------------------------
