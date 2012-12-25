#include <qglobal.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#ifdef Q_OS_LINUX
#include <linux/input.h>
#endif

#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "ui/uistatistic.h"
#include "ui/objectitem.h"
#include "application.h"
#include "map.h"
#include "imagestorage.h"
#include "objects.h"
#include "battlefieldscene.h"
//------------------------------------------------------------------------------
#define SHOW_SCENE_BORDER 1
//------------------------------------------------------------------------------
TBattlefieldScene::TBattlefieldScene(QObject *parent)
    : QGraphicsScene(parent)
    , Application(NULL)
    , ResImages(NULL)
    , ResObjects(NULL)
    , CurrentMap(NULL)
    , CurrentWorld(NULL)
    , PlayerControl(NULL)
{
//    ui::UIStatistic* statistic = new ui::UIStatistic;
//    this->addItem(statistic);
//    statistic->setParent(this);
//    statistic->resize(,1000);
//    qDebug() << statistic->rect();
//    statistic->setPos(0, 300);
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
void TBattlefieldScene::SetUiRect(const QRectF &rect)
{
    Q_UNUSED(rect);
}
//------------------------------------------------------------------------------
void TBattlefieldScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    const QImage* image = Application->GetModel()->GetMap()->GetBackground();
    if( !image ) {
        painter->fillRect(rect, Qt::black);
        return;
    }

    QRectF drawingRect = QRectF(image->rect().center() + rect.topLeft(), rect.size());

    painter->fillRect(rect, Application->GetModel()->GetMap()->GetBackgroundColor());
    painter->drawImage(rect.topLeft(), *image, drawingRect);

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
// TODO: rewrite this function
void TBattlefieldScene::UpdateScene()
{
    if( !IsValid() ) {
        emit ActionMainMenu();
        return;
    }

    this->clear();

    // Add objects
    for (int i = 0; i < CurrentWorld->objects_size(); ++i) {
        const Epsilon5::Object& object = CurrentWorld->objects(i);
        if (object.id() < 0) {
            continue;
        }

        TObjectItem* item = new TObjectItem(
            QPixmap::fromImage(*ResObjects->GetImageById(object.id())));
        this->addItem(item);
        item->setPos(object.x(), object.y());
        item->setRotationRad(object.angle());
    }

    // Add bullets
    const QImage* img;
    for (int i = 0; i != CurrentWorld->bullets_size(); i++) {
        const Epsilon5::Bullet& bullet = CurrentWorld->bullets(i);
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

        TObjectItem* item = new TObjectItem(QPixmap::fromImage(*img));
        this->addItem(item);
        item->setPos(bullet.x(), bullet.y());
    }

    // Add players
    for (int i = 0; i != CurrentWorld->players_size(); i++) {
        const Epsilon5::Player& player = CurrentWorld->players(i);
        if ((size_t)player.id() == Application->GetModel()->GetPlayerId()) {
            img = &ResImages->GetImage("player");
            PlayerPos = QPointF(player.x(), player.y());
        } else {
            if (player.team()) {
                img = &ResImages->GetImage("peka_t2");
            } else {
                img = &ResImages->GetImage("peka_t1");
            }
        }
        TObjectItem* item = new TObjectItem(QPixmap::fromImage(*img));
        this->addItem(item);
        item->setPos(player.x(), player.y());
    }

    // Add respawns
    static QVector<TRespPoint> RespPoints;
    if (CurrentWorld->resp_points_size() > 0) {
        RespPoints.clear();
        for (int i = 0; i < CurrentWorld->resp_points_size(); i++) {
            TRespPoint pos;
            pos.X = CurrentWorld->resp_points(i).x();
            pos.Y = CurrentWorld->resp_points(i).y();
            pos.Team = (ETeam)(CurrentWorld->resp_points(i).team());
            RespPoints.push_back(pos);
        }
    }

    for (int i = 0; i < RespPoints.size(); i++) {
        if (RespPoints[i].Team == T_One) {
            img = &ResImages->GetImage("flag_t1");
        } else if (RespPoints[i].Team == T_Second) {
            img = &ResImages->GetImage("flag_t2");
        } else {
            img = &ResImages->GetImage("flag_tn");
        }
        QPoint currentRespPos(RespPoints[i].X, RespPoints[i].Y);
        TObjectItem* item = new TObjectItem(QPixmap::fromImage(*img));
        this->addItem(item);
        item->setPos(currentRespPos.x(), currentRespPos.y());
    }
}
//------------------------------------------------------------------------------
