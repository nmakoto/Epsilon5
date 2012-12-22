#pragma once
#include <QGraphicsView>
//------------------------------------------------------------------------------
class QGraphicsScene;
class TApplication;
class TObjects;
class TImageStorage;
class TMap;
//------------------------------------------------------------------------------
namespace Epsilon5
{
class World;
class Control;
}
//------------------------------------------------------------------------------
class TGameView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TGameView(TApplication* app, QGraphicsScene* scene, QWidget* parent = 0);
    void PrepareView();

signals:
    void QuitAction();

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

private:
    void SetMovementKeysState(bool state, const QKeyEvent* event);
    void DrawText(QPainter* painter, const QPoint& pos,
            const QString& text, int fontSizePt);
    void calcFps();

private:
    const TApplication* App;
    int Fps;

public:
    Epsilon5::Control* PlayerControl;
};
//------------------------------------------------------------------------------
