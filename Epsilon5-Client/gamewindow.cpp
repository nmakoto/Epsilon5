#include <qglobal.h>
#include <QDebug>
#include <QGLWidget>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "ui/objectitem.h"
#include "ui/uimenu.h"
#include "gameview.h"
#include "map.h"
#include "application.h"
#include "imagestorage.h"
#include "objects.h"
#include "gamewindow.h"
//------------------------------------------------------------------------------
const quint16 BASE_WINDOW_WIDTH = 800;
const quint16 BASE_WINDOW_HEIGHT = 600;
const quint32 DEFAULT_UPDATE_VIEW_TIME = 20;
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
TGameWindow::TGameWindow(TApplication* app)
    : QObject(app)
    , Application(app)
    , Render(new QGLWidget)
//    , Render(new QGLWidget(
//            QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel | QGL::Rgba)))
    , GameScene(new QGraphicsScene(this))
    , MenuScene(new QGraphicsScene(this))
    , GameView(new TGameView(app, GameScene))
    , Images(new TImageStorage(this))
    , Objects(new TObjects(this))
    , CurrentMap(NULL)
    , CurrentWorld(NULL)
    , MainMenu(new ui::UIMenu)
{
    GameView->setViewport(Render);
    GameView->setMinimumSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
    GameView->resize(Application->GetSettings()->GetWindowSize());
    GameView->move(Application->GetSettings()->GetWindowPos());
    if (Application->GetSettings()->GetWindowFullscreen()) {
        GameView->showFullScreen();
    }

    connect(GameView, SIGNAL(MainMenuAction()), SIGNAL(MainMenuAction()));
    connect(GameView, SIGNAL(QuitAction()), SIGNAL(QuitAction()));
    connect(MainMenu, SIGNAL(clicked(QString)), SLOT(menuItemClicked(QString)));

    startTimer(DEFAULT_UPDATE_VIEW_TIME);

    GameView->PlayerControl = Application->GetModel()->GetPlayerControl();
    MainMenu->setParent(GameView);
}
//------------------------------------------------------------------------------
TGameWindow::~TGameWindow()
{
    Application->GetSettings()->SetWindowFullscreen(GameView->isFullScreen());
    delete GameView;
}
//------------------------------------------------------------------------------
void TGameWindow::Init()
{
    Images->LoadAll();
    Objects->LoadObjects("objects/objects.txt");

    MainMenu->addMenuItem("quit",
            QPixmap::fromImage(Images->GetImage("menu-exit")),
            QPixmap::fromImage(Images->GetImage("menu-exit-h")));
    MainMenu->addMenuItem("connect",
            QPixmap::fromImage(Images->GetImage("menu-connect")),
            QPixmap::fromImage(Images->GetImage("menu-connect-h")));

    MenuScene->addItem(MainMenu);
    MainMenu->grabChildrenEvents();

    Render->show();
    GameView->show();
}
//------------------------------------------------------------------------------
void TGameWindow::PrepareView()
{
    CurrentMap = Application->GetModel()->GetMap();
    CurrentWorld = Application->GetModel()->GetWorld();

    GameScene->setSceneRect(
        (qreal)CurrentMap->GetWidth() / -2,
        (qreal)CurrentMap->GetHeight() / -2,
        CurrentMap->GetWidth(), CurrentMap->GetHeight());
}
//------------------------------------------------------------------------------
void TGameWindow::timerEvent(QTimerEvent*)
{
    switch( Application->GetState() ) {
    case ST_LoadingMap:
        ShowLoading();
        return;
    case ST_Connecting:
        ShowConnecting();
        return;
    case ST_InGame:
        ShowInGame();
        return;
    case ST_MainMenu:
    default:
        ShowMainMenu();
    }
}
//------------------------------------------------------------------------------
void TGameWindow::menuItemClicked(const QString& name)
{
    if (name == "quit") {
        emit QuitAction();
        GameView->close();
    }
    if (name == "connect") {
        emit ConnectAction();
    }
}
//------------------------------------------------------------------------------
void TGameWindow::ShowMainMenu()
{
    GameView->setScene(MenuScene);
    GameView->setSceneRect(MenuScene->sceneRect());
    GameView->centerOn(MenuScene->sceneRect().center());
    MainMenu->update(GameView->rect());
}
//------------------------------------------------------------------------------
void TGameWindow::ShowConnecting()
{
    GameView->setScene(MenuScene);
    GameView->centerOn(MenuScene->sceneRect().center());
}
//------------------------------------------------------------------------------
void TGameWindow::ShowLoading()
{
    GameView->setScene(MenuScene);
    GameView->centerOn(MenuScene->sceneRect().center());
}
//------------------------------------------------------------------------------
void TGameWindow::ShowInGame()
{
    if( !CurrentWorld ) {
        emit MainMenuAction();
        return;
    }

    GameView->setScene(GameScene);
    GameScene->clear();

    for (int i = 0; i < CurrentWorld->objects_size(); ++i) {
        const Epsilon5::Object& object = CurrentWorld->objects(i);
        if (object.id() < 0) {
            continue;
        }

        TObjectItem* item = new TObjectItem(
            QPixmap::fromImage(*Objects->GetImageById(object.id())));
        GameScene->addItem(item);
        item->setPos(object.x(), object.y());
        item->setRotationRad(object.angle());
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

        TObjectItem* item = new TObjectItem(QPixmap::fromImage(*img));
        GameScene->addItem(item);
        item->setPos(bullet.x(), bullet.y());
    }

    for (int i = 0; i != CurrentWorld->players_size(); i++) {
        const Epsilon5::Player& player = CurrentWorld->players(i);
        if ((size_t)player.id() == Application->GetModel()->GetPlayerId()) {
            img = &Images->GetImage("player");
            GameView->setSceneRect(
                player.x() - GameView->width() / 2,
                player.y() - GameView->height() / 2,
                GameView->width(),
                GameView->height()
            );
        } else {
            if (player.team()) {
                img = &Images->GetImage("peka_t2");
            } else {
                img = &Images->GetImage("peka_t1");
            }
        }
        TObjectItem* item = new TObjectItem(QPixmap::fromImage(*img));
        GameScene->addItem(item);
        item->setPos(player.x(), player.y());
    }

    QPoint cursorPos = GameView->mapFromGlobal(QCursor::pos());
    double angle = getAngle(cursorPos - GameView->rect().center());
    GameView->PlayerControl->set_angle(angle);
}
//------------------------------------------------------------------------------
