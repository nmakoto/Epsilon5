#pragma once
#include <QObject>
#include <QMouseEvent>
#include <QEvent>
#include <QLineEdit>
#include "ui/uiframe.h"
//------------------------------------------------------------------------------
class TUiButton;
class QPushButton;
//------------------------------------------------------------------------------
class TRespawnFrame : public TUiFrame
{
    Q_OBJECT
public:
    explicit TRespawnFrame(QWidget* parent, QGLWidget* shared = 0);
    ~TRespawnFrame();

    void Paint(QPainter& painter);

public slots:
    void test();

private:
    TUiButton* CloseButton;
};
//------------------------------------------------------------------------------
class TUiEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit TUiEdit(QWidget* parent = 0)
        : QLineEdit(parent)
    {
    }

protected:
    bool event(QEvent *event) {
        if( event->type() == QEvent::MouseButtonPress ||
                event->type() == QEvent::MouseButtonRelease ||
                event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint pp = mapFromGlobal(mouseEvent->pos());
            if( !rect().contains(pp) ) {
                event->ignore();
                return false;
            }
            return QWidget::event(event);
        }

        return QWidget::event(event);
    }
};
//------------------------------------------------------------------------------
