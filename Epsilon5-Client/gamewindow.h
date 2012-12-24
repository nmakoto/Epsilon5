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
class QGraphicsBlurEffect;
namespace ui
{
class UIMenu;
}
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
    void ConnectAction();
    void MainMenuAction();

protected:
    void timerEvent(QTimerEvent* event);

private:
    void SetMovementKeysState(bool state, const QKeyEvent* event);

private slots:
    void menuItemClicked(const QString& name);

private:
    TApplication* Application;
    QGLWidget* Render;
    QGraphicsScene* GameScene;
    TGameView* GameView;
    TImageStorage* Images;
    TObjects* Objects;
    const TMap* CurrentMap;
    const Epsilon5::World* CurrentWorld;

    ui::UIMenu* MainMenu;
    QGraphicsBlurEffect* BlurEffect;
};
//------------------------------------------------------------------------------
