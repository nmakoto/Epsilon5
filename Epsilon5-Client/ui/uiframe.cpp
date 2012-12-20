#include "uiframe.h"
//------------------------------------------------------------------------------
TUiFrame::TUiFrame(QObject* parent)
    : QObject(parent)
    , Background(new QImage())
{
}
//------------------------------------------------------------------------------
TUiFrame::~TUiFrame()
{
    delete Background;
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackground(const QImage &image, bool resizeFrame)
{
    *Background = image;
    if( !resizeFrame )
        return;
    FrameRect.setSize(Background->size());
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackground(const QImage &image, quint8 alpha, bool resizeFrame)
{
    *Background = GetTransparentImage(image, alpha).toImage();
    if( !resizeFrame )
        return;
    FrameRect.setSize(Background->size());
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackgroundScaled(const QImage &image)
{
    *Background = image.scaled(FrameRect.width(), FrameRect.height());
}
//------------------------------------------------------------------------------
void TUiFrame::SetBackgroundScaled(const QImage &image, quint8 alpha)
{
    *Background = GetTransparentImage(image.scaled(
            FrameRect.width(), FrameRect.height()), alpha).toImage();
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
