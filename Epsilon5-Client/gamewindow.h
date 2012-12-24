#pragma once
#include <QObject>
#include <QGraphicsView>
//------------------------------------------------------------------------------
class QGraphicsScene;
class QGLWidget;
class QKeyEvent;
class TApplication;
class TMap;
class TObjects;
class TImageStorage;
class QGraphicsBlurEffect;
class QGraphicsTextItem;
//------------------------------------------------------------------------------
namespace Epsilon5
{
class World;
class Control;
}
//------------------------------------------------------------------------------
namespace ui
{
class UIMenu;
}
//------------------------------------------------------------------------------
class TGameWindow: public QGraphicsView
{
    Q_OBJECT
public:
    explicit TGameWindow(TApplication* app, QWidget* parent = 0);
    ~TGameWindow();
    void Init();
    void PrepareView();
    void UpdateView();
    void ShowMainMenu();
    void ShowLoading();
    void ShowConnecting();
    void ShowInGame();

signals:
    void QuitAction();
    void ConnectAction();
    void MainMenuAction();

protected:
    void timerEvent(QTimerEvent* event);
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void resizeEvent(QResizeEvent *event);

    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);

private:
    void SetMovementKeysState(bool state, const QKeyEvent* event);
    void DrawText(QPainter* painter, const QPoint& pos,
            const QString& text, int fontSizePt);
    void calcFps();

private slots:
    void menuItemClicked(const QString& name);

private:
    TApplication* Application;
    QGLWidget* Render;
    QGraphicsScene* GameScene;
    QGraphicsScene* MenuScene;
    TImageStorage* Images;
    TObjects* Objects;
    const TMap* CurrentMap;
    const Epsilon5::World* CurrentWorld;
    Epsilon5::Control* PlayerControl;
    QGraphicsTextItem* LoadingItemText;
    ui::UIMenu* MainMenu;
    QGraphicsBlurEffect* BlurEffect;
    int Fps;
};
//------------------------------------------------------------------------------
