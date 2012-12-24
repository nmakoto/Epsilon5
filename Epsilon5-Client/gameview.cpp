#include <QDebug>
#include <QKeyEvent>

#ifdef Q_OS_LINUX
#include <linux/input.h>
#endif

#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "application.h"
#include "objects.h"
#include "imagestorage.h"
#include "map.h"
#include "gameview.h"
//------------------------------------------------------------------------------
TGameView::TGameView(TApplication* app, QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
    , App(app)
    , Fps(0)
    , PlayerControl(NULL)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);
}
//------------------------------------------------------------------------------
void TGameView::SetMovementKeysState(bool state, const QKeyEvent* event)
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
void TGameView::mousePressEvent(QMouseEvent* event)
{
//    qDebug() << Q_FUNC_INFO;
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        PlayerControl->mutable_keystatus()->set_keyattack1(true);
    } else {
        PlayerControl->mutable_keystatus()->set_keyattack2(true);
    }
}
//------------------------------------------------------------------------------
void TGameView::mouseReleaseEvent(QMouseEvent* event)
{
//    qDebug() << Q_FUNC_INFO;
    QGraphicsView::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        PlayerControl->mutable_keystatus()->set_keyattack1(false);
    } else {
        PlayerControl->mutable_keystatus()->set_keyattack2(false);
    }
}
//------------------------------------------------------------------------------
void TGameView::keyPressEvent(QKeyEvent* event)
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
void TGameView::keyReleaseEvent(QKeyEvent* event)
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
void TGameView::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);
    calcFps();
}
//------------------------------------------------------------------------------
void TGameView::drawBackground(QPainter* painter, const QRectF& rect)
{
    const QImage* image = App->GetModel()->GetMap()->GetBackground();
    QRectF drawingRect = QRectF(image->rect().center() + rect.topLeft(), rect.size());

    painter->fillRect(rect, App->GetModel()->GetMap()->GetBackgroundColor());
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
void TGameView::drawForeground(QPainter* painter, const QRectF& rect)
{

    DrawText(painter, QPoint(rect.topLeft().toPoint().x(),
            rect.topLeft().y() + 10),
            QString("FPS: ").append(QString::number(Fps)), 10);
}
//------------------------------------------------------------------------------
void TGameView::DrawText(QPainter* painter, const QPoint& pos,
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
void TGameView::calcFps()
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
