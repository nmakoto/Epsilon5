#include "respawnframe.h"
//------------------------------------------------------------------------------
TRespawnFrame::TRespawnFrame(QObject* parent)
    : QObject(parent)
    , Background(new QImage())
{
}
//------------------------------------------------------------------------------
TRespawnFrame::~TRespawnFrame()
{
    delete Background;
}
//------------------------------------------------------------------------------
void TRespawnFrame::Paint(QPainter& painter)
{
    if( !painter.isActive() )
        return;

    painter.drawImage(FrameRect, *Background);
    QPen oldPen = painter.pen();
    painter.setPen(QPen(QBrush(Qt::black), 4));
    painter.drawRect(FrameRect);
    painter.setPen(oldPen);
}
//------------------------------------------------------------------------------
void TRespawnFrame::SetBackground(const QImage &image, bool resizeFrame)
{
    *Background = image;
    if( !resizeFrame )
        return;
    FrameRect.setSize(Background->size());
}
//------------------------------------------------------------------------------
void TRespawnFrame::SetBackgroundScaled(const QImage &image)
{
    *Background = image.scaled(FrameRect.width(), FrameRect.height());
}
//------------------------------------------------------------------------------
