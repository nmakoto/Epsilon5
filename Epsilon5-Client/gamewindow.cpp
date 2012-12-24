#include <qglobal.h>
#include <QDebug>
#include <QGLWidget>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsTextItem>
#include <QFont>

#ifdef Q_OS_LINUX
#include <linux/input.h>
#endif

#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "ui/objectitem.h"
#include "ui/uimenu.h"
#include "ui/uistatistic.h"
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
TGameWindow::TGameWindow(TApplication* app, QWidget* parent)
    : QGraphicsView(parent)
    , Application(app)
    , Render(new QGLWidget)
//    , Render(new QGLWidget(
//            QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel | QGL::Rgba)))
    , GameScene(new QGraphicsScene(this))
    , MenuScene(new QGraphicsScene(this))
    , Images(new TImageStorage(this))
    , Objects(new TObjects(this))
    , CurrentMap(NULL)
    , CurrentWorld(NULL)
    , PlayerControl(NULL)
    , LoadingItemText(NULL)
    , MainMenu(new ui::UIMenu)
    , Fps(0)
{
    setViewport(Render);
    // Setup window frame
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);
    setMinimumSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
    resize(Application->GetSettings()->GetWindowSize());
    move(Application->GetSettings()->GetWindowPos());
    if (Application->GetSettings()->GetWindowFullscreen()) {
        showFullScreen();
    }

    connect(MainMenu, SIGNAL(clicked(QString)), SLOT(menuItemClicked(QString)));

    PlayerControl = Application->GetModel()->GetPlayerControl();
    MainMenu->setParent(this);

    ui::UIStatistic* statistic = new ui::UIStatistic;
    MenuScene->addItem(statistic);
//    statistic->setParent(this);
//    statistic->resize(,1000);
//    qDebug() << statistic->rect();
//    statistic->setPos(0, 300);

    LoadingItemText = MenuScene->addText("", QFont("Ubuntu", 28));
    LoadingItemText->setHtml("<span style='background:rgba(0,0,0,0.6);"
            "color:#eee;border-radius:40px;'>" + tr("Loading...") + "</span>");
    LoadingItemText->hide();
    startTimer(DEFAULT_UPDATE_VIEW_TIME);
}
//------------------------------------------------------------------------------
TGameWindow::~TGameWindow()
{
    Application->GetSettings()->SetWindowFullscreen(isFullScreen());
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
    show();
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
void TGameWindow::SetMovementKeysState(bool state, const QKeyEvent* event)
{
#ifdef Q_OS_UNIX
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
#ifdef Q_OS_WIN
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
void TGameWindow::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        PlayerControl->mutable_keystatus()->set_keyattack1(true);
    } else {
        PlayerControl->mutable_keystatus()->set_keyattack2(true);
    }
}
//------------------------------------------------------------------------------
void TGameWindow::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        PlayerControl->mutable_keystatus()->set_keyattack1(false);
    } else {
        PlayerControl->mutable_keystatus()->set_keyattack2(false);
    }
}
//------------------------------------------------------------------------------
void TGameWindow::keyPressEvent(QKeyEvent* event)
{
    SetMovementKeysState(true, event);

    switch (event->key()) {
    case '1':
        PlayerControl->set_weapon(Epsilon5::Pistol);
        break;
    case '2':
        PlayerControl->set_weapon(Epsilon5::Machinegun);
        break;
    case '3':
        PlayerControl->set_weapon(Epsilon5::Shotgun);
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
        emit QuitAction();
        close();
        break;
    case Qt::Key_F1:
//        emit ToggleRespawnFrameAction();
        break;
    case Qt::Key_F2:
        break;
#endif
    case Qt::Key_Escape:
        emit MainMenuAction();
        break;
    case Qt::Key_Tab:
//        ShowStats = false;
        break;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TGameWindow::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
    calcFps();
}
//------------------------------------------------------------------------------
void TGameWindow::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if( LoadingItemText )
        LoadingItemText->setPos(rect().bottomRight()
                - LoadingItemText->boundingRect().bottomRight());
}
//------------------------------------------------------------------------------
void TGameWindow::drawBackground(QPainter* painter, const QRectF& rect)
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
    const quint8 SCENE_BORDER_SIZE = 4;
    painter->setPen(QPen(QBrush(Qt::black), SCENE_BORDER_SIZE));
    painter->drawRect(QRect(
            image->rect().topLeft() - image->rect().center(),
            image->size()));
    painter->setPen(oldPen);
}
//------------------------------------------------------------------------------
void TGameWindow::drawForeground(QPainter* painter, const QRectF& rect)
{
    DrawText(painter, QPoint(rect.topLeft().toPoint().x(),
            rect.topLeft().y() + 10),
            QString("FPS: ").append(QString::number(Fps)), 10);
}
//------------------------------------------------------------------------------
void TGameWindow::DrawText(QPainter* painter, const QPoint& pos,
        const QString& text, int fontSizePt)
{
    // Helvetica font present on all Systems
    QFont oldFont = painter->font();
    QPen oldPen = painter->pen();
    painter->setFont(QFont("Helvetica", fontSizePt));
    painter->setPen(Qt::black);
    painter->drawText(pos.x() + 1, pos.y() + 1, text);
    painter->setPen(Qt::darkGray);
    painter->drawText(pos.x(), pos.y(), text);
    painter->setPen(oldPen);
    painter->setFont(oldFont);
}
//------------------------------------------------------------------------------
void TGameWindow::calcFps()
{
    static int frames = 0;
    static QTime lasttime = QTime::currentTime();

    const QTime& time = QTime::currentTime();
    if (lasttime.msecsTo(time) >= 1000) {
        Fps = frames;
        frames = 0;
        lasttime = time;
    }
    ++frames;
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
        close();
    }
    if (name == "connect") {
        emit ConnectAction();
    }
}
//------------------------------------------------------------------------------
void TGameWindow::ShowMainMenu()
{
    setScene(MenuScene);
    setSceneRect(MainMenu->rect());
    centerOn(MainMenu);
    MainMenu->show();
    MainMenu->update(rect());
}
//------------------------------------------------------------------------------
void TGameWindow::ShowConnecting()
{
    setScene(MenuScene);
    setSceneRect(rect());
    MainMenu->hide();
    LoadingItemText->show();
}
//------------------------------------------------------------------------------
void TGameWindow::ShowLoading()
{
    setScene(MenuScene);
    setSceneRect(rect());
    MainMenu->hide();
    LoadingItemText->show();
}
//------------------------------------------------------------------------------
void TGameWindow::ShowInGame()
{
    if( !CurrentWorld ) {
        emit MainMenuAction();
        return;
    }

    LoadingItemText->hide();
    setScene(GameScene);
    GameScene->clear();

    // Add objects
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

    // Add bullets
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

    // Add players
    for (int i = 0; i != CurrentWorld->players_size(); i++) {
        const Epsilon5::Player& player = CurrentWorld->players(i);
        if ((size_t)player.id() == Application->GetModel()->GetPlayerId()) {
            img = &Images->GetImage("player");
            setSceneRect(player.x() - width() / 2, player.y() - height() / 2,
                    width(), height());
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
            img = &Images->GetImage("flag_t1");
        } else if (RespPoints[i].Team == T_Second) {
            img = &Images->GetImage("flag_t2");
        } else {
            img = &Images->GetImage("flag_tn");
        }
        QPoint currentRespPos(RespPoints[i].X, RespPoints[i].Y);
        TObjectItem* item = new TObjectItem(QPixmap::fromImage(*img));
        GameScene->addItem(item);
        item->setPos(currentRespPos.x(), currentRespPos.y());
    }

    // Update player crosshair position
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    double angle = getAngle(cursorPos - rect().center());
    PlayerControl->set_angle(angle);
}
//------------------------------------------------------------------------------
