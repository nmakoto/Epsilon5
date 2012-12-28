#include <qglobal.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>

#ifdef Q_OS_LINUX
#include <linux/input.h>
#endif

#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "ui/uiminimap.h"
#include "ui/uistatistic.h"
#include "scene/scplayer.h"
#include "application.h"
#include "map.h"
#include "imagestorage.h"
#include "objects.h"
#include "battlefieldscene.h"
//------------------------------------------------------------------------------
#define SHOW_SCENE_BORDER 1
#define NO_IMAGE_BACKGROUND 0
//------------------------------------------------------------------------------
const quint32 DEFAULT_CORRECT_SCENE_TIME = 500;
const quint16 MAXIMUM_MINIMAP_SIZE = 300;
//------------------------------------------------------------------------------
TBattlefieldScene::TBattlefieldScene(QObject *parent)
    : QGraphicsScene(parent)
    , Application(nullptr)
    , ResImages(nullptr)
    , ResObjects(nullptr)
    , CurrentMap(nullptr)
    , CurrentWorld(nullptr)
    , PlayerControl(nullptr)
    , Minimap(new ui::UIMinimap)
{
//    ui::UIStatistic* statistic = new ui::UIStatistic;
//    this->addItem(statistic);
//    statistic->setParent(this);
//    statistic->resize(,1000);
//    qDebug() << statistic->rect();
//    statistic->setPos(0, 300);
    addItem(Minimap);

    startTimer(DEFAULT_CORRECT_SCENE_TIME);
}
//------------------------------------------------------------------------------
TBattlefieldScene::~TBattlefieldScene()
{
}
//------------------------------------------------------------------------------
void TBattlefieldScene::Init(const TApplication* application,
        const TImageStorage* images, const TObjects* objects)
{
    Application = application;
    ResImages = images;
    ResObjects = objects;
    PlayerControl = Application->GetModel()->GetPlayerControl();
}
//------------------------------------------------------------------------------
bool TBattlefieldScene::IsValid() const
{
    return Application && CurrentMap && CurrentWorld && PlayerControl
            && ResImages && ResObjects;
}
//------------------------------------------------------------------------------
void TBattlefieldScene::PrepareScene()
{
    CurrentMap = Application->GetModel()->GetMap();
    CurrentWorld = Application->GetModel()->GetWorld();

    this->setSceneRect((qreal)CurrentMap->GetWidth() / -2,
            (qreal)CurrentMap->GetHeight() / -2,
            CurrentMap->GetWidth(), CurrentMap->GetHeight());
    Minimap->SetMaximumSize( MAXIMUM_MINIMAP_SIZE );
    Minimap->SetMapInfo(QSize(CurrentMap->GetWidth(), CurrentMap->GetHeight()),
            *CurrentMap->GetBackground());
}
//------------------------------------------------------------------------------
void TBattlefieldScene::SetMovementKeysState(bool state, const QKeyEvent* event)
{
#ifdef Q_OS_LINUX
    // NOTE: Codes in input.h differ from event->scancodes by MAGIC_NUMBER.
    //       Need some checks
    const int MAGIC_NUMBER = 8;
    if (event->nativeScanCode() == (KEY_W + MAGIC_NUMBER)) {
        PlayerControl->mutable_keystatus()->set_keyup(state);
    }
    if (event->nativeScanCode() == (KEY_S + MAGIC_NUMBER)) {
        PlayerControl->mutable_keystatus()->set_keydown(state);
    }
    if (event->nativeScanCode() == (KEY_A + MAGIC_NUMBER)) {
        PlayerControl->mutable_keystatus()->set_keyleft(state);
    }
    if (event->nativeScanCode() == (KEY_D + MAGIC_NUMBER)) {
        PlayerControl->mutable_keystatus()->set_keyright(state);
    }
#endif
#ifdef Q_OS_WIN32
    if (event->key() == Qt::Key_W || event->nativeVirtualKey() == Qt::Key_W) {
        PlayerControl->mutable_keystatus()->set_keyup(state);
    }
    if (event->key() == Qt::Key_S || event->nativeVirtualKey() == Qt::Key_S) {
        PlayerControl->mutable_keystatus()->set_keydown(state);
    }
    if (event->key() == Qt::Key_A || event->nativeVirtualKey() == Qt::Key_A) {
        PlayerControl->mutable_keystatus()->set_keyleft(state);
    }
    if (event->key() == Qt::Key_D || event->nativeVirtualKey() == Qt::Key_D) {
        PlayerControl->mutable_keystatus()->set_keyright(state);
    }
#endif
    if (event->key() == Qt::Key_Up) {
        PlayerControl->mutable_keystatus()->set_keyup(state);
    }
    if (event->key() == Qt::Key_Down) {
        PlayerControl->mutable_keystatus()->set_keydown(state);
    }
    if (event->key() == Qt::Key_Left) {
        PlayerControl->mutable_keystatus()->set_keyleft(state);
    }
    if (event->key() == Qt::Key_Right) {
        PlayerControl->mutable_keystatus()->set_keyright(state);
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
//    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        PlayerControl->mutable_keystatus()->set_keyattack1(true);
    } else {
        PlayerControl->mutable_keystatus()->set_keyattack2(true);
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
//    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        PlayerControl->mutable_keystatus()->set_keyattack1(false);
    } else {
        PlayerControl->mutable_keystatus()->set_keyattack2(false);
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::keyPressEvent(QKeyEvent* event)
{
    SetMovementKeysState(true, event);

    switch (event->key()) {
    case '1':
        PlayerControl->set_weapon(Epsilon5::Pistol);
        return;
    case '2':
        PlayerControl->set_weapon(Epsilon5::Machinegun);
        return;
    case '3':
        PlayerControl->set_weapon(Epsilon5::Shotgun);
        return;
    case Qt::Key_Tab:
//        ShowStats = true;
        return;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::keyReleaseEvent(QKeyEvent* event)
{
    SetMovementKeysState(false, event);

    switch (event->key()) {
    case Qt::Key_Escape:
        emit ActionMainMenu();
        return;
    case Qt::Key_Tab:
//        ShowStats = false;
        return;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if( Application->GetState() != ST_InGame )
        return;
}
//------------------------------------------------------------------------------
void TBattlefieldScene::SetUiRect(const QRectF &rect)
{
    ViewRect = rect;
}
//------------------------------------------------------------------------------
void TBattlefieldScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    const QImage* image = Application->GetModel()->GetMap()->GetBackground();
    if( !image ) {
        painter->fillRect(rect, Qt::black);
        return;
    }

#if NO_IMAGE_BACKGROUND && defined(QT_DEBUG)
    painter->fillRect(rect,
            QBrush(Application->GetModel()->GetMap()->GetBackgroundColor(),
                    Qt::Dense7Pattern));
#else
    QRectF drawingRect = QRectF(image->rect().center() + rect.topLeft(), rect.size());
    painter->fillRect(rect, Application->GetModel()->GetMap()->GetBackgroundColor());
    painter->drawImage(rect.topLeft(), *image, drawingRect);
#endif

    QPen oldPen = painter->pen();
#if SHOW_SCENE_BORDER && defined(QT_DEBUG)
    const quint8 SCENE_BORDER_SIZE = 4;
    painter->setPen(QPen(QBrush(Qt::black), SCENE_BORDER_SIZE));
    painter->drawRect(QRect(
            image->rect().topLeft() - image->rect().center(),
            image->size()));
    painter->setPen(oldPen);
#endif
}
//------------------------------------------------------------------------------
void TBattlefieldScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
//    ViewRect = rect;
//    Minimap->setPos(rect.topLeft());
}
//------------------------------------------------------------------------------
QPointF TBattlefieldScene::GetPlayerPos() const
{
    return PlayerPos;
}
//------------------------------------------------------------------------------
void TBattlefieldScene::SetPlayerAngle(qreal angle)
{
    PlayerControl->set_angle(angle);
}
//------------------------------------------------------------------------------
bool TBattlefieldScene::IsAtShowingDistance(const QPointF &playerPos,
        const QPointF &pos)
{
    const QGraphicsView* item = views().first();
    if( !item )
        return true;

    return abs(playerPos.x() - pos.x()) < item->rect().width()
            && abs(playerPos.y() - pos.y()) < item->rect().height();
}
//------------------------------------------------------------------------------
void TBattlefieldScene::UpdateScene()
{
    if( !IsValid() ) {
        emit ActionMainMenu();
        return;
    }

    // Add items to scene if not already there or update'em (pos, status, etc)
    UpdateObjects();
    UpdateBullets();
    UpdatePlayers();
    UpdateRespawns();

    if( Application->GetModel()->GetFullWorldState() )
    {
//        QList<QGraphicsItem*>& list = this->items(ViewRect);
//        for( auto& item : list ) {
//        }
    }

    Minimap->setPos(PlayerPos - ViewRect.center() + QPointF(10, 10));
}
//------------------------------------------------------------------------------
void TBattlefieldScene::UpdateObjects()
{
    scene::SCObject* item;
    for( int i = 0; i < CurrentWorld->objects_size(); ++i ) {
        const Epsilon5::Object& object = CurrentWorld->objects(i);
        if( object.resource_id() < 0 || !IsAtShowingDistance(
                PlayerPos, QPointF(object.x(), object.y()))) {
            continue;
        }

        if( ItemHash.keys().contains(object.id()) ) {
            item = ItemHash[object.id()];
        } else {
            item = new scene::SCObject(QPixmap::fromImage(
                    *ResObjects->GetImageById(object.resource_id())));
            ItemHash[object.id()] = item;
            this->addItem(item);
            Minimap->AddItem("objects", item);
        }

        if( object.has_x() && object.has_y() )
            item->setPos(object.x(), object.y());
        if( object.has_angle() )
            item->setRotationRad(object.angle());
        Minimap->update();
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::UpdateBullets()
{
    // Clear all bullet item at scene
    auto it = BulletItemHash.constBegin();
    for( ; it != BulletItemHash.constEnd(); ++it ) {
        this->removeItem(*it);
    }
    BulletItemHash.clear();

    // Add bullets
    for( int i = 0; i < CurrentWorld->bullets_size(); ++i ) {
        const Epsilon5::Bullet& bullet = CurrentWorld->bullets(i);

        const QImage* img;
        switch (bullet.bullet_type()) {
        case Epsilon5::Bullet_Type_ARBUZ:
            img = &ResImages->GetImage("arbuz");
            break;
        case Epsilon5::Bullet_Type_LITTLE_BULLET:
            img = &ResImages->GetImage("bullet");
            break;
        default:
            throw UException("Unknown bullet");
            break;
        }

        scene::SCObject* item = new scene::SCObject(QPixmap::fromImage(*img));
        BulletItemHash[BulletItemHash.count()] = item;
        this->addItem(item);
        item->setPos(bullet.x(), bullet.y());
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::UpdateRespawns()
{
    scene::SCObject* item;
    for( int i = 0; i < CurrentWorld->resp_points_size(); ++i ) {
        const Epsilon5::RespPoint& respawn = CurrentWorld->resp_points(i);
        if( ItemHash.keys().contains(respawn.id()) ) {
            item = ItemHash[respawn.id()];
        } else {
            const QImage* img;
            if (respawn.team() == T_One) {
                img = &ResImages->GetImage("flag_t1");
            } else if (respawn.team() == T_Second) {
                img = &ResImages->GetImage("flag_t2");
            } else {
                img = &ResImages->GetImage("flag_tn");
            }

            item = new scene::SCObject(QPixmap::fromImage(*img));
            // TODO: remove magic number
            item->setZValue(10);
            ItemHash[respawn.id()] = item;
            this->addItem(item);
            Minimap->AddItem("respawns", item);
        }
        item->setPos(respawn.x(), respawn.y());
        Minimap->update();
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::UpdatePlayers()
{
    scene::SCPlayer* item;
    size_t currentPlayerId = Application->GetModel()->GetPlayerId();
    for (int i = 0; i != CurrentWorld->players_size(); i++) {
        const Epsilon5::Player& player = CurrentWorld->players(i);

        if( (size_t)player.id() == currentPlayerId) {
            PlayerPos = QPointF(player.x(), player.y());
        }

        if( ItemHash.keys().contains(player.id()) ) {
            item = qgraphicsitem_cast<scene::SCPlayer*>(ItemHash[player.id()]);
        } else {
            const QImage* img;
            if ((size_t)player.id() == currentPlayerId) {
                img = &ResImages->GetImage("player");
            } else {
                if (player.team()) {
                    img = &ResImages->GetImage("peka_t2");
                } else {
                    img = &ResImages->GetImage("peka_t1");
                }
            }
            item = new scene::SCPlayer(QPixmap::fromImage(*img));
            ItemHash[player.id()] = item;
            this->addItem(item);
            Minimap->AddItem("players", item);
        }

        item->setPos(player.x(), player.y());
        item->SetHp(player.hp());
        if( player.has_name())
            item->SetName(player.name().c_str());
        Minimap->update();
    }
}
//------------------------------------------------------------------------------
void TBattlefieldScene::SetViewRect(const QRectF &rect)
{
    ViewRect = rect;
}
//------------------------------------------------------------------------------
QRectF TBattlefieldScene::GetUiRect() const
{
    QPointF topLeft(PlayerPos - ViewRect.center());
    if(PlayerPos.x() < ViewRect.center().x())
        topLeft.setX(ViewRect.left());
    if(PlayerPos.y() < ViewRect.center().y())
        topLeft.setY(ViewRect.top());
    if(PlayerPos.x() > ViewRect.center().x())
        topLeft.setX(ViewRect.left());
    if(PlayerPos.y() > ViewRect.center().y())
        topLeft.setY(ViewRect.left());

    return QRectF(topLeft, ViewRect.size());
}
//------------------------------------------------------------------------------
