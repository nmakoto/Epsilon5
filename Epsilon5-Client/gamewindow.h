#pragma once
#include <QObject>
//#include "application.h"
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
class QGraphicsTextItem;
//------------------------------------------------------------------------------
namespace Epsilon5
{
class World;
}
//------------------------------------------------------------------------------
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

public:
    void ShowMainMenu();
    void ShowLoading();
    void ShowConnecting();
    void ShowInGame();

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
    QGraphicsScene* MenuScene;
    QGraphicsTextItem* LoadingItemText;
    TGameView* GameView;
    TImageStorage* Images;
    TObjects* Objects;
    const TMap* CurrentMap;
    const Epsilon5::World* CurrentWorld;

    ui::UIMenu* MainMenu;
    QGraphicsBlurEffect* BlurEffect;
};
//------------------------------------------------------------------------------
