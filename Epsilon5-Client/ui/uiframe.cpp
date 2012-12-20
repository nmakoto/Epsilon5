#include <QDebug>
#include <QPaintEvent>
#include <QMouseEvent>
#include "uiframe.h"
//------------------------------------------------------------------------------
TUiFrame::TUiFrame(QWidget* parent)
    : QWidget(0) //parent)
    , Background(new QImage())
{
    setFixedSize(0, 0);
    setAttribute(Qt::WA_DontShowOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_OutsideWSRange);
//    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_NoMousePropagation);
//    setAttribute(Qt::WA_MouseNoMask);
//    setAttribute(Qt::WA_MouseTracking);
//    setAttribute(Qt::WA_NoChildEventsForParent);
    setAutoFillBackground(false);
}
//------------------------------------------------------------------------------
TUiFrame::~TUiFrame()
{
    delete Background;
}
//------------------------------------------------------------------------------
bool TUiFrame::eventFilter(QObject* object, QEvent* event)
{
    Q_UNUSED(object);
    if( event->type() == QEvent::MouseButtonPress)
    {
    }
    return false;
}
//------------------------------------------------------------------------------
void TUiFrame::paintEvent(QPaintEvent* event)
{
    event->accept();
    if( !paintingActive() )
        return;
}
//------------------------------------------------------------------------------
void TUiFrame::mousePressEvent(QMouseEvent *event)
{
    QPoint pp = mapFromGlobal(event->pos());
    if( !rect().contains(pp) ) {
        return;
    }
    qDebug() << rect() << pos();
    qDebug("%d x %d | %d x %d", event->pos().x(), event->pos().y(), pp.x(), pp.y());
    event->accept();
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackground(const QImage &image, bool resizeFrame)
{
    *Background = image;
    if( !resizeFrame )
        return;
    setFixedSize(Background->size());
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackground(const QImage &image, quint8 alpha, bool resizeFrame)
{
    *Background = GetTransparentImage(image, alpha).toImage();
    if( !resizeFrame )
        return;
    setFixedSize(Background->size());
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackgroundScaled(const QImage &image)
{
    *Background = image.scaled(width(), height());
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackgroundScaled(const QImage &image, quint8 alpha)
{
    *Background = GetTransparentImage(image.scaled(width(),
            height()), alpha).toImage();
}
//------------------------------------------------------------------------------
QPixmap TUiFrame::GetTransparentImage(const QImage& image, quint8 alpha)
{
    QPixmap transparent(image.size());
    transparent.fill(Qt::transparent);
    QPainter p;
    p.begin(&transparent);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap(0, 0, QPixmap::fromImage(image));
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(transparent.rect(), QColor(0, 0, 0, alpha));
    p.end();
    return transparent;
}
//------------------------------------------------------------------------------
