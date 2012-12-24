#include <QPainter>
#include "uilayoutitem.h"
//------------------------------------------------------------------------------
#define UI_TEST_BORDERS 0
//------------------------------------------------------------------------------
using namespace ui;
//------------------------------------------------------------------------------
UILayoutItem::UILayoutItem(const QString& name, const QPixmap& pixmap,
        const QPixmap& hovered, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , QGraphicsLayoutItem()
    , NormalPixmap(new QPixmap(pixmap))
    , HoveredPixmap(new QPixmap(hovered))
    , Hovered(false)
    , Name(name)
{
    setGraphicsItem(this);
    setOpacity(parent->opacity());
    setFlags(ItemIsFocusable);
    setAcceptHoverEvents(true);
}
//------------------------------------------------------------------------------
UILayoutItem::~UILayoutItem()
{
    delete NormalPixmap;
    delete HoveredPixmap;
}
//------------------------------------------------------------------------------
void UILayoutItem::paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    QPixmap* pixmap = Hovered ? HoveredPixmap : NormalPixmap;
    painter->drawPixmap(contentsRect().center() - pixmap->rect().center(), *pixmap);
#if UI_TEST_BORDERS && defined(QT_DEBUG)
    QPen oldPen = painter->pen();
    painter->setPen(QPen(QBrush(Hovered ? Qt::red : Qt::yellow), 2));
    painter->drawRect(contentsRect());
    painter->setPen(oldPen);
#endif
}
//------------------------------------------------------------------------------
QRectF UILayoutItem::boundingRect() const
{
    return QRectF(QPointF(0, 0), geometry().size());
}
//------------------------------------------------------------------------------
void UILayoutItem::setGeometry(const QRectF& geom)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.topLeft());
}
//------------------------------------------------------------------------------
QSizeF UILayoutItem::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    switch (which) {
    case Qt::MinimumSize:
    case Qt::PreferredSize:
        return NormalPixmap->size();
    case Qt::MaximumSize:
    default:
        break;
    }
    return constraint;
}
//------------------------------------------------------------------------------
bool UILayoutItem::isHovered() const
{
    return Hovered;
}
//------------------------------------------------------------------------------
void UILayoutItem::setHovered(bool value)
{
    Hovered = value;
}
//------------------------------------------------------------------------------
const QString& UILayoutItem::name() const
{
    return Name;
}
//------------------------------------------------------------------------------
