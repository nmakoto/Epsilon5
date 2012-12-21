#include <QLineEdit>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/uibutton.h"
#include "ui/respawnframe.h"

//------------------------------------------------------------------------------
TRespawnFrame::TRespawnFrame(QWidget* parent, QGLWidget* shared)
    : TUiFrame(parent, shared)
    , CloseButton(new TUiButton(this))
{
    CloseButton->setText("Close");
    QLineEdit* le = new QLineEdit("test string", this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
//    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(le);
    mainLayout->addStretch();
//    buttonLayout->addStretch();
    buttonLayout->addWidget(CloseButton);
    buttonLayout->addStretch();
    connect(CloseButton, SIGNAL(clicked()), this, SLOT(close()));
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
void TRespawnFrame::test()
{
    qDebug("Test message");
}
//------------------------------------------------------------------------------
