#include <qglobal.h>
#include <QDebug>
#include <QGLWidget>
#include <QKeyEvent>
#include <QFont>

#include "imagestorage.h"
#include "objects.h"
#include "battlefieldscene.h"
#include "menuscene.h"
#include "gamewindow.h"
//------------------------------------------------------------------------------
#define RENDER_WITH_SAMPLEBUFFERS 0
//------------------------------------------------------------------------------
const quint16 BASE_WINDOW_WIDTH = 800;
const quint16 BASE_WINDOW_HEIGHT = 600;
const quint32 DEFAULT_UPDATE_VIEW_TIME = 20;
//------------------------------------------------------------------------------
namespace
{
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
}
//------------------------------------------------------------------------------
TGameWindow::TGameWindow(TApplication* app, QWidget* parent)
    : QGraphicsView(parent)
    , Application(app)
#if RENDER_WITH_SAMPLEBUFFERS
    , Render(new QGLWidget(
            QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel | QGL::Rgba)))
#else
    , Render(new QGLWidget)
#endif
    , ResImages(new TImageStorage(this))
    , ResObjects(new TObjects(this))
    , BattlefieldScene(new TBattlefieldScene(this))
    , MenuScene(new TMenuScene(this))
    , LastFps(0)
    , LastPing(0)
    , LastState(ST_MainMenu)
{
    setViewport(Render);
    Render->setParent(this);

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

    setScene(MenuScene);

    connect(MenuScene, SIGNAL(ActionConnect()), SIGNAL(ActionConnect()));
    connect(MenuScene, SIGNAL(ActionQuit()), SIGNAL(ActionQuit()));
    connect(BattlefieldScene, SIGNAL(ActionMainMenu()), SIGNAL(ActionMainMenu()));
    connect(BattlefieldScene, SIGNAL(ActionQuit()), SIGNAL(ActionQuit()));

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
    ResImages->LoadAll();
    ResObjects->LoadObjects("objects/objects.txt");

    MenuScene->Init(Application, ResImages, ResObjects);
    BattlefieldScene->Init(Application, ResImages, ResObjects);

    this->show();
}
//------------------------------------------------------------------------------
void TGameWindow::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
    switch (event->key()) {
    case Qt::Key_F11:
        setWindowState(windowState() ^ Qt::WindowFullScreen);
        return;
#ifdef QT_DEBUG
    case Qt::Key_F12:
        emit ActionQuit();
        return;
#endif
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TGameWindow::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
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
    // Update screen rectangle for UI elements reposition
    BattlefieldScene->SetUiRect(rect());
    MenuScene->SetUiRect(rect());
}
//------------------------------------------------------------------------------
void TGameWindow::drawBackground(QPainter* painter, const QRectF& rect)
{
    QGraphicsView::drawBackground(painter, rect);
}
//------------------------------------------------------------------------------
void TGameWindow::drawForeground(QPainter* painter, const QRectF& rect)
{
    QGraphicsView::drawForeground(painter, rect);
#ifdef QT_DEBUG
    DrawText(painter, QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - 10),
            QString("FPS: ").append(QString::number(LastFps)));
    DrawText(painter, QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - 24),
            QString("Ping: ").append(QString::number(LastPing)));
    if( Application->GetState() == ST_InGame ) {
        DrawText(painter,
                QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - 36),
                QString("Objects:").append(
                    QString::number(BattlefieldScene->items().count())));
    }
#endif
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
        LastFps = frames;
        frames = 0;
        lasttime = time;
    }
    ++frames;
}
//------------------------------------------------------------------------------
void TGameWindow::timerEvent(QTimerEvent*)
{
    if( LastState != Application->GetState() ) {
        LastState = Application->GetState();
        PrepareView(LastState);
    }

    quint32 ping = Application->GetModel()->GetPlayerPing();
    if( ping )
        LastPing = ping;

    UpdateView(LastState);
}
//------------------------------------------------------------------------------
void TGameWindow::PrepareView(EGameState state)
{
    switch( state ) {
    case ST_MainMenu:
        MenuScene->PrepareScene(TMenuScene::MS_MainMenu);
        this->setScene(MenuScene);
        this->setSceneRect(MenuScene->sceneRect());
        centerOn(MenuScene->sceneRect().center());
        return;
    case ST_Connecting:
    case ST_LoadingMap:
        MenuScene->PrepareScene(TMenuScene::MS_Loading);
        this->setScene(MenuScene);
        centerOn(MenuScene->sceneRect().center());
        return;
    case ST_InGame:
        BattlefieldScene->PrepareScene();
        this->setScene(BattlefieldScene);
        centerOn(BattlefieldScene->sceneRect().center());
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TGameWindow::UpdateView(EGameState state)
{
    QPoint cursorPos;
    QRectF rt;
    switch( state ) {
    case ST_MainMenu:
    case ST_Connecting:
    case ST_LoadingMap:
        MenuScene->update(rect());
        return;
    case ST_InGame:
        cursorPos = mapFromGlobal(QCursor::pos());
        BattlefieldScene->SetPlayerAngle(::getAngle(cursorPos - rect().center()));
        BattlefieldScene->UpdateScene();
        centerOn(BattlefieldScene->GetPlayerPos());
        setSceneRect(QRectF(BattlefieldScene->GetPlayerPos() - rect().center(),
                rect().size()));
        return;
    case ST_SelectingResp:
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void TGameWindow::ShowMainMenu()
{
    PrepareView(ST_MainMenu);
}
//------------------------------------------------------------------------------
void TGameWindow::ShowConnecting()
{
    PrepareView(ST_Connecting);
}
//------------------------------------------------------------------------------
void TGameWindow::ShowLoading()
{
    PrepareView(ST_LoadingMap);
}
//------------------------------------------------------------------------------
void TGameWindow::ShowInGame()
{
    PrepareView(ST_InGame);
}
//------------------------------------------------------------------------------
