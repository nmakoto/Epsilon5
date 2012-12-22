#pragma once
#include <QWidget>
#include <QGLWidget>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "application.h"
//------------------------------------------------------------------------------
class QGraphicsScene;
class QGraphicsView;
class QGLWidget;
class TApplication;
class TMap;
class TObjects;
class TImageStorage;
//------------------------------------------------------------------------------
class TGameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TGameWindow(TApplication* app, QWidget* parent = 0);
    ~TGameWindow();
    inline const Epsilon5::Control& GetControl() {
        return Control;
    }
    void PrepareView();


protected:
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    void SetMovementKeysState(bool state, const QKeyEvent* event);
    void Fps();

private:
    TApplication* Application;
    QGLWidget* Render;
    QGraphicsScene* BattlefieldScene;
    QGraphicsScene* MenuScene;
    QGraphicsView* RenderView;
    TImageStorage* Images;
    TObjects* Objects;
    Epsilon5::Control Control;
    const TMap* CurrentMap;
    const Epsilon5::World* CurrentWorld;
};
//------------------------------------------------------------------------------
