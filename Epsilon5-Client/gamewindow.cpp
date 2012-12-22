#include <qglobal.h>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGLWidget>
#include <QVBoxLayout>

#ifdef Q_OS_LINUX
#include <linux/input.h>
#endif

#include "map.h"
#include "imagestorage.h"
#include "objects.h"
#include "gamewindow.h"
//------------------------------------------------------------------------------
// Transform direction vector into angle
static double getAngle(const QPoint& point)
{
    double angle;
    double x = point.x();
    double y = point.y();
    if (x > 0) {
        angle = atan(y / x);
    } else if (x < 0 && y > 0) {
        angle = M_PI + atan(y / x);
    } else if (x < 0 && y < 0) {
        angle = -M_PI + atan(y / x);
    } else if (x == 0 && y > 0) {
        angle = M_PI / 2;
    } else {
        angle = -M_PI / 2;
    }
    return -angle;
}
//------------------------------------------------------------------------------
TGameWindow::TGameWindow(TApplication* app, QWidget* parent)
    : QWidget(parent, Qt::Dialog)
    , Application(app)
    , Render(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel | QGL::Rgba)))
    , BattlefieldScene(new QGraphicsScene(this))
    , MenuScene(new QGraphicsScene(this))
    , RenderView(new QGraphicsView(BattlefieldScene, this))
    , Images(new TImageStorage(this))
    , Objects(new TObjects(this))
    , CurrentMap(NULL)
    , CurrentWorld(NULL)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setMargin(0);
//    layout->addSpacing(100);
    layout->addWidget(RenderView);
    setFocusProxy(RenderView);

    QGLFormat f = QGLFormat::defaultFormat();
    f.setSampleBuffers(true);
    f.setStencilBufferSize(8);
    f.setSamples(4);
//    f.setVersion(QGLFormat::OpenGL_Version_4_0, QGLFormat::OpenGL_Version_4_0);
    Render->setFormat(f);

    Objects->LoadObjects("objects/objects.txt");
    Images->LoadAll();

    MenuScene->setSceneRect(0, 0, width(), height());
    BattlefieldScene->setSceneRect(0, 0, 4000, 3000);

    MenuScene->addRect(0,0,100,100, QPen(QBrush(Qt::darkGray), 4));

    RenderView->setViewport(Render);
    RenderView->setFrameStyle(QFrame::NoFrame);
    Render->setFixedSize(BattlefieldScene->width(), BattlefieldScene->height());

    Render->show();
    RenderView->show();
    this->show();

    Control.set_angle(0);
    Control.mutable_keystatus()->set_keyattack1(false);
    Control.mutable_keystatus()->set_keyattack2(false);
    Control.mutable_keystatus()->set_keyleft(false);
    Control.mutable_keystatus()->set_keyright(false);
    Control.mutable_keystatus()->set_keyup(false);
    Control.mutable_keystatus()->set_keydown(false);
    Control.set_weapon(Epsilon5::Pistol);

    startTimer(20);
}
//------------------------------------------------------------------------------
TGameWindow::~TGameWindow()
{
    BattlefieldScene->clear();
    delete Render;
}
//------------------------------------------------------------------------------
void TGameWindow::PrepareView()
{
    CurrentMap = Application->GetModel()->GetMap();
    CurrentWorld = Application->GetModel()->GetWorld();

    qDebug() << "1" << BattlefieldScene->sceneRect();
    qDebug() << CurrentMap->GetWidth() << CurrentMap->GetHeight();
    BattlefieldScene->setSceneRect(
            (qreal)CurrentMap->GetWidth() / -2, (qreal)CurrentMap->GetHeight() / -2,
            CurrentMap->GetWidth(), CurrentMap->GetHeight());
    qDebug() << "2" << BattlefieldScene->sceneRect();

//    RenderView->setSceneRect(BattlefieldScene->sceneRect());
//    RenderView->setSceneRect(0, 0 , 400, 500);
//    qDebug() << BattlefieldScene->sceneRect() << RenderView->sceneRect();

//    RenderView->centerOn(0,0);
//    RenderView->scale(2,2);
}
//------------------------------------------------------------------------------
void TGameWindow::timerEvent(QTimerEvent *)
{
    if( !CurrentWorld )
        return;

    BattlefieldScene->clear();
    for( int i = 0; i < CurrentWorld->objects_size(); ++i ) {
        const Epsilon5::Object& object = CurrentWorld->objects(i);
        if( object.id() < 0 )
            continue;
        QGraphicsPixmapItem* item = BattlefieldScene->addPixmap(
                QPixmap::fromImage(*Objects->GetImageById(object.id())));
        item->setPos(object.x(), object.y());
        item->rotate(object.angle());
    }

    const QImage* img;
    for (int i = 0; i != CurrentWorld->bullets_size(); i++) {
        const Epsilon5::Bullet& bullet = CurrentWorld->bullets(i);
        switch (bullet.bullet_type()) {
        case Epsilon5::Bullet_Type_ARBUZ:
            img = &Images->GetImage("arbuz");
            break;
        case Epsilon5::Bullet_Type_LITTLE_BULLET:
            img = &Images->GetImage("bullet");
            break;
        default:
            throw UException("Unknown bullet");
            break;
        }

        QGraphicsPixmapItem* item = BattlefieldScene->addPixmap(
                QPixmap::fromImage(*img));
        item->setPos(bullet.x(), bullet.y());
//        item->rotate(object.angle());
    }

    for (int i = 0; i != CurrentWorld->players_size(); i++) {
        const Epsilon5::Player& player = CurrentWorld->players(i);
        if ((size_t)player.id() == Application->GetModel()->GetPlayerId()) {
            const QImage* img = &Images->GetImage("player");
            QGraphicsPixmapItem* playerItem = BattlefieldScene->addPixmap(QPixmap::fromImage(*img));
            playerItem->setPos(player.x(), player.y());
//            QPoint lt = RenderView->mapFromScene(player.x(), player.y());
            QPointF viewOnScene = QPointF(/*RenderView->mapToScene(*/width(),height());
//                    RenderView->viewport()->width(),
//                    RenderView->viewport()->height());

            RenderView->setSceneRect(
                    player.x() - viewOnScene.x() / 2,
                    player.y() - viewOnScene.y() / 2,
                    viewOnScene.x(),
                    viewOnScene.y()
            );
//            qDebug() << playerItem->pos() << RenderView->sceneRect() << viewOnScene;
//            RenderView->centerOn(player.x(), player.y());
//            RenderView->centerOn(playerItem);
        }
    }

    QPoint cursorPos = this->mapFromGlobal(QCursor::pos());
    double angle = getAngle(cursorPos - rect().center());
    Control.set_angle(angle);

    Fps();
}
//------------------------------------------------------------------------------
void TGameWindow::SetMovementKeysState(bool state, const QKeyEvent* event)
{
#ifdef Q_OS_UNIX
    // NOTE: Codes in input.h differ from event->scancodes by MAGIC_NUMBER.
    //       Need some checks
    const int MAGIC_NUMBER = 8;
    if (event->nativeScanCode() == (KEY_W + MAGIC_NUMBER)) {
        Control.mutable_keystatus()->set_keyup(state);
    }
    if (event->nativeScanCode() == (KEY_S + MAGIC_NUMBER)) {
        Control.mutable_keystatus()->set_keydown(state);
    }
    if (event->nativeScanCode() == (KEY_A + MAGIC_NUMBER)) {
        Control.mutable_keystatus()->set_keyleft(state);
    }
    if (event->nativeScanCode() == (KEY_D + MAGIC_NUMBER)) {
        Control.mutable_keystatus()->set_keyright(state);
    }
#endif
#ifdef Q_OS_WIN
    if (event->key() == Qt::Key_W || event->nativeVirtualKey() == Qt::Key_W) {
        Control.mutable_keystatus()->set_keyup(state);
    }
    if (event->key() == Qt::Key_S || event->nativeVirtualKey() == Qt::Key_S) {
        Control.mutable_keystatus()->set_keydown(state);
    }
    if (event->key() == Qt::Key_A || event->nativeVirtualKey() == Qt::Key_A) {
        Control.mutable_keystatus()->set_keyleft(state);
    }
    if (event->key() == Qt::Key_D || event->nativeVirtualKey() == Qt::Key_D) {
        Control.mutable_keystatus()->set_keyright(state);
    }
#endif
    if (event->key() == Qt::Key_Up) {
        Control.mutable_keystatus()->set_keyup(state);
    }
    if (event->key() == Qt::Key_Down) {
        Control.mutable_keystatus()->set_keydown(state);
    }
    if (event->key() == Qt::Key_Left) {
        Control.mutable_keystatus()->set_keyleft(state);
    }
    if (event->key() == Qt::Key_Right) {
        Control.mutable_keystatus()->set_keyright(state);
    }
}
//------------------------------------------------------------------------------
void TGameWindow::keyPressEvent(QKeyEvent* event)
{
    SetMovementKeysState(true, event);

    switch (event->key()) {
    case '1':
        Control.set_weapon(Epsilon5::Pistol);
        break;
    case '2':
        Control.set_weapon(Epsilon5::Machinegun);
        break;
    case '3':
        Control.set_weapon(Epsilon5::Shotgun);
        break;
    case Qt::Key_Tab:
//        ShowStats = true;
        break;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TGameWindow::keyReleaseEvent(QKeyEvent* event)
{
    SetMovementKeysState(false, event);

    switch (event->key()) {
    case Qt::Key_F11:
//        ToggleFullscreen();
        setWindowState(windowState() ^ Qt::WindowFullScreen);
        break;
#ifdef QT_DEBUG
    case Qt::Key_F12:
//        emit QuitAction();
        close();
        break;
    case Qt::Key_F1:
//        emit ToggleRespawnFrameAction();
        RenderView->setScene(MenuScene);
        break;
    case Qt::Key_F2:
        RenderView->setScene(BattlefieldScene);
        break;
#endif
    case Qt::Key_Escape:
//        emit MainMenuAction();
        break;
    case Qt::Key_Tab:
//        ShowStats = false;
        break;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TGameWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        Control.mutable_keystatus()->set_keyattack1(true);
    } else {
        Control.mutable_keystatus()->set_keyattack2(true);
    }
    qDebug() << "press event";
//    qApp->sendEvent(&Menu, event);
}
//------------------------------------------------------------------------------
void TGameWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        Control.mutable_keystatus()->set_keyattack1(false);
    } else {
        Control.mutable_keystatus()->set_keyattack2(false);
    }
    qDebug() << "release event";
}
//------------------------------------------------------------------------------
void TGameWindow::Fps()
{
    static int frames = 0;
    static int fps = 0;
    static QTime lasttime = QTime::currentTime();

    const QTime& time = QTime::currentTime();
    if (lasttime.msecsTo(time) >= 1000) {
        fps = frames;
        frames = 0;
        lasttime = time;
    }
//    qDebug() << "FPS:" << fps;

//    const QPen penOld = painter.pen();
//    DrawText(painter, QPoint(0, 10), QString("Fps: %1").arg(fps), 10);
    ++frames;
}
//------------------------------------------------------------------------------
