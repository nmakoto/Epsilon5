#include <QDebug>
#include <QPaintEvent>
#include "ui/uibutton.h"
//------------------------------------------------------------------------------
TUiButton::TUiButton(QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(120, 40);
    setStyleSheet("* { background: transparent; border: 2px solid black; }");
}
//------------------------------------------------------------------------------
void TUiButton::mousePressEvent(QMouseEvent *event)
{
//    emit clicked();
}
//------------------------------------------------------------------------------
//bool TUiButton::event(QEvent *event)
//{
//    if( event->type() == QEvent::MouseButtonPress ||
//            event->type() == QEvent::MouseButtonRelease ||
//            event->type() == QEvent::MouseMove) {
//        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
//        QPoint pp = mapFromGlobal(mouseEvent->pos());
//        if( !rect().contains(pp) ) {
//            event->ignore();
//            return false;
//        }
//        return QWidget::event(event);
//    }

//    return QWidget::event(event);
//}
//------------------------------------------------------------------------------
