#pragma once
#include <QWidget>
#include <QGLWidget>
#include "application.h"
//------------------------------------------------------------------------------
class QGraphicsScene;
class QGraphicsView;
class QGLWidget;
class TApplication;
//------------------------------------------------------------------------------
class TGameView : public QWidget
{
    Q_OBJECT
public:
    explicit TGameView(TApplication* app, QWidget* parent = 0);
    ~TGameView();

protected:
    void keyReleaseEvent(QKeyEvent* event);

private:
    TApplication* Application;
    QGLWidget* Render;
    QGraphicsScene* BattlefieldScene;
    QGraphicsScene* MenuScene;
    QGraphicsView* RenderView;
};
//------------------------------------------------------------------------------
