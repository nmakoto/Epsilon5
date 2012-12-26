#pragma once
#include <QObject>
#include <QGraphicsView>
#include "application.h"
//------------------------------------------------------------------------------
class QGLWidget;
class QKeyEvent;
class TObjects;
class TImageStorage;
class TBattlefieldScene;
class TMenuScene;
//------------------------------------------------------------------------------
class TGameWindow: public QGraphicsView
{
    Q_OBJECT
public:
    explicit TGameWindow(TApplication* app, QWidget* parent = 0);
    ~TGameWindow();
    void Init();
    void PrepareView(EGameState state);
    void UpdateView(EGameState state);

    void ShowMainMenu();
    void ShowLoading();
    void ShowConnecting();
    void ShowInGame();

signals:
    void ActionConnect();
    void ActionMainMenu();
    void ActionQuit();

protected:
    void timerEvent(QTimerEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);

private:
    void DrawText(QPainter* painter, const QPoint& pos, const QString& text,
            int fontSizePt = 10);
    void calcFps();

private:
    TApplication* Application;
    QGLWidget* Render;
    TImageStorage* ResImages;
    TObjects* ResObjects;
    TBattlefieldScene* BattlefieldScene;
    TMenuScene* MenuScene;
    int LastFps;
    quint32 LastPing;
    EGameState LastState;
};
//------------------------------------------------------------------------------
