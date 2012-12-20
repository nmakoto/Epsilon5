#include <QPaintEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/uibutton.h"
#include "ui/respawnframe.h"
//------------------------------------------------------------------------------
TRespawnFrame::TRespawnFrame(QWidget* parent)
    : TUiFrame(parent)
    , CloseButton(new TUiButton(this))
{
    CloseButton->setText("Close");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addStretch();
    buttonLayout->addWidget(CloseButton);
    buttonLayout->addStretch();
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

    painter.drawImage(QRect(pos(),rect().size()), *Background);
    QPen oldPen = painter.pen();
    painter.setPen(QPen(QBrush(Qt::black), 4));
    painter.drawRect(QRect(pos(),rect().size()));
    painter.setPen(oldPen);

    render(&painter, pos());
}
//------------------------------------------------------------------------------
