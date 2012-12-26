#include <QDebug>
#include <QWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneHoverEvent>
#include <QPen>
#include <QPainter>
#include "ui/uilayoutitem.h"
#include "ui/uimenu.h"
//------------------------------------------------------------------------------
#define UI_TEST_BORDERS 0
//------------------------------------------------------------------------------
using namespace ui;
//------------------------------------------------------------------------------
UIMenu::UIMenu(QGraphicsItem* parent)
    : QGraphicsWidget(parent)
{
    windowLayout = new QGraphicsLinearLayout(Qt::Vertical);
    windowLayout->addStretch();
    windowLayout->addStretch();

    setLayout(windowLayout);
    setFlag(QGraphicsItem::ItemIsPanel);
}
//------------------------------------------------------------------------------
UIMenu::~UIMenu()
{
}
//------------------------------------------------------------------------------
void UIMenu::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->fillRect(rect(), Qt::black);
#if UI_TEST_BORDERS && defined(QT_DEBUG)
    QPen oldPen = painter->pen();
    painter->setPen(QPen(QBrush(Qt::white), 2));
    painter->drawRect(contentsRect());
    painter->setPen(oldPen);
#endif
}
//------------------------------------------------------------------------------
bool UIMenu::sceneEventFilter(QGraphicsItem* watched, QEvent* event)
{
    UILayoutItem* menuItem = qgraphicsitem_cast<UILayoutItem*>(watched);
    if (!menuItem) {
        return false;
    }

    static UILayoutItem* lastMenuItem = nullptr;

    switch (event->type()) {
    case QEvent::GraphicsSceneHoverEnter:
        if (menuItem->isHovered()) {
            return true;
        }
        emit hovered(menuItem->name());
        menuItem->setHovered();
        if (lastMenuItem) {
            lastMenuItem->setHovered(false);
        }
        lastMenuItem = menuItem;
        return true;
    case QEvent::GraphicsSceneHoverLeave:
        emit unhovered(menuItem->name());
        menuItem->setHovered(false);
        return true;
    case QEvent::GraphicsSceneMousePress:
        emit clicked(menuItem->name());
        return true;
    default:
        break;
    }
    return false;
}
//------------------------------------------------------------------------------
void UIMenu::grabChildrenEvents()
{
    auto it = childItems().constBegin();
    for (; it != childItems().constEnd(); ++it) {
        (*it)->installSceneEventFilter(this);
    }
}
//------------------------------------------------------------------------------
void UIMenu::ungrabChildrenEvents()
{
    auto it = childItems().constBegin();
    for (; it != childItems().constEnd(); ++it) {
        (*it)->removeSceneEventFilter(this);
    }
}
//------------------------------------------------------------------------------
void UIMenu::addMenuItem(const QString& name, const QPixmap& pixNormal,
        const QPixmap& pixHovered, bool opacity)
{
    UILayoutItem* item = new UILayoutItem(name, pixNormal, pixHovered, this);
    item->setOpacity(opacity);
    windowLayout->insertItem(1, item);
}
//------------------------------------------------------------------------------
