#pragma once
#include "application.h"
//------------------------------------------------------------------------------
class QGraphicsScene;
class QGLWidget;
class QKeyEvent;
class TApplication;
class TGameView;
class TMap;
class TObjects;
class TImageStorage;
class TFormItem;
class QGraphicsBlurEffect;
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

    TFormItem* MainMenu;
    QGraphicsBlurEffect* BlurEffect;
};
//------------------------------------------------------------------------------
