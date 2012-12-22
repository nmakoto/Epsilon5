#pragma once
#include <QWidget>
#include <QGLWidget>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "application.h"
//------------------------------------------------------------------------------
class QGraphicsScene;
class TGameView;
class QGLWidget;
class TApplication;
class TMap;
class TObjects;
class TImageStorage;
//------------------------------------------------------------------------------
class TGameWindow: public QObject
{
    Q_OBJECT
public:
    explicit TGameWindow(TApplication* app);
    ~TGameWindow();
    void Init();
    void PrepareView();
    void UpdateView();

signals:
    void QuitAction();

protected:
    void timerEvent(QTimerEvent* event);

private:
    void SetMovementKeysState(bool state, const QKeyEvent* event);

private:
    TApplication* Application;
    QGLWidget* Render;
    QGraphicsScene* GameScene;
    TGameView* GameView;
    TImageStorage* Images;
    TObjects* Objects;
    const TMap* CurrentMap;
    const Epsilon5::World* CurrentWorld;
};
//------------------------------------------------------------------------------
