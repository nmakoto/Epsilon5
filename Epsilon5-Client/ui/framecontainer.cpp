#include <QCoreApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QEvent>
#include "ui/framecontainer.h"
//------------------------------------------------------------------------------
TFrameContainer::TFrameContainer(QObject* parent)
    : QObject(parent)
{
}
//------------------------------------------------------------------------------
void TFrameContainer::addWidget(QWidget* widget)
{
    Frames.push_back(widget);
}
//------------------------------------------------------------------------------
bool TFrameContainer::eventFilter(QObject* object, QEvent* event)
{
    if( event->type() != QEvent::MouseButtonPress )
        return false;

    for( auto frame : Frames ) {
//        qDebug() << frame << frame->children();
        for( auto child : frame->children() )
        {

//            qDebug() << frame << ":" << child;
//            QWidget* widget = qobject_cast<QWidget*>(child);
//            if( widget->rect().contains(static_cast<QMouseEvent*>(event)->pos()))
              if( qApp->sendEvent(child, event) )
                  return true;
        }
        if( qApp->sendEvent(frame, event) )
            return true;
    }

    return false;
    return true;
}
//------------------------------------------------------------------------------
