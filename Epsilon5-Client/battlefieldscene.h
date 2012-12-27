#pragma once
#include <QGraphicsScene>
#include "ui/objectitem.h"
//------------------------------------------------------------------------------
class QKeyEvent;
class TApplication;
class TImageStorage;
class TObjects;
class TMap;
//------------------------------------------------------------------------------
namespace Epsilon5
{
class World;
class Control;
}
//------------------------------------------------------------------------------
class TBattlefieldScene : public QGraphicsScene
{
    Q_OBJECT
    typedef QHash<quint32, TObjectItem*> TGraphItemHash;
public:
    explicit TBattlefieldScene(QObject *parent = 0);
    ~TBattlefieldScene();

    void Init(const TApplication* application, const TImageStorage* images,
            const TObjects* objects);
    bool IsValid() const;
    void PrepareScene();
    void UpdateScene();
    void SetUiRect(const QRectF& rect);

    QPointF GetPlayerPos() const;
    void SetPlayerAngle(qreal angle);

signals:
#ifdef QT_DEBUG
    void ActionQuit();
#endif
    void ActionMainMenu();

protected:
    void timerEvent(QTimerEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void drawBackground(QPainter* painter, const QRectF& rect);
//    void drawForeground(QPainter* painter, const QRectF& rect);

private:
    void SetMovementKeysState(bool state, const QKeyEvent* event);
    bool IsAtShowingDistance(const QPointF& playerPos, const QPointF& pos);
    void UpdateObjects();
    void UpdateBullets();
    void UpdateRespawns();
    void UpdatePlayers();

private:
    const TApplication* Application;
    const TImageStorage* ResImages;
    const TObjects* ResObjects;
    const TMap* CurrentMap;
    const Epsilon5::World* CurrentWorld;
    Epsilon5::Control* PlayerControl;
    QPointF PlayerPos;
    TGraphItemHash ItemHash;
    TGraphItemHash BulletItemHash;
};
//------------------------------------------------------------------------------
