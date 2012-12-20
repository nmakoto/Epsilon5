#include "respawnframe.h"
//------------------------------------------------------------------------------
TRespawnFrame::TRespawnFrame(QObject* parent)
    : TUiFrame(parent)
{
}
//------------------------------------------------------------------------------
TRespawnFrame::~TRespawnFrame()
{
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
