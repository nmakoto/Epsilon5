#pragma once
#include <QGraphicsWidget>
//------------------------------------------------------------------------------
class QGraphicsLinearLayout;
//------------------------------------------------------------------------------
namespace ui
{
//------------------------------------------------------------------------------
class UILayoutItem;
//------------------------------------------------------------------------------
class UIMenu : public QGraphicsWidget
{
    Q_OBJECT
public:
    UIMenu(QGraphicsItem* parent = 0);
    ~UIMenu();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
            QWidget* widget);
    void grabChildrenEvents();
    void ungrabChildrenEvents();
    void addMenuItem(const QString& name, const QPixmap& pixNormal,
            const QPixmap& pixHovered = QPixmap(), bool opacity = 1.0);

signals:
    void clicked(const QString& name);
    void hovered(const QString& name);
    void unhovered(const QString& name);

protected:
    bool sceneEventFilter(QGraphicsItem* watched, QEvent* event);

private:
//    UILayoutItem* item;
    QGraphicsLinearLayout* windowLayout;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
