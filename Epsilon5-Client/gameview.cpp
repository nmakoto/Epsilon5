#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGLWidget>
#include <QVBoxLayout>
#include "gameview.h"
//------------------------------------------------------------------------------
TGameView::TGameView(TApplication* app, QWidget* parent)
    : QWidget(parent, Qt::Dialog)
    , Application(app)
    , Render(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel | QGL::Rgba)))
    , BattlefieldScene(new QGraphicsScene(this))
    , MenuScene(new QGraphicsScene(this))
    , RenderView(new QGraphicsView(BattlefieldScene, this))
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setMargin(0);
    layout->addWidget(RenderView);

    MenuScene->setSceneRect(0, 0, width(), height());
    BattlefieldScene->setSceneRect(0, 0, 4000, 3000);

    MenuScene->addRect(0,0,100,100, QPen(QBrush(Qt::darkGray), 4));

    RenderView->setViewport(Render);
    RenderView->setFrameStyle(QFrame::NoFrame);
    Render->setFixedSize(BattlefieldScene->width(),BattlefieldScene->height());

    RenderView->show();
    Render->show();
    this->show();
}
//------------------------------------------------------------------------------
TGameView::~TGameView()
{
    delete Render;
}
//------------------------------------------------------------------------------
void TGameView::keyReleaseEvent(QKeyEvent *event)
{
    switch( event->key() ) {
    case Qt::Key_F1: {
        RenderView->setScene(MenuScene);
        return;
    }
    case Qt::Key_F2: {
        RenderView->setScene(BattlefieldScene);
        return;
    }
    case Qt::Key_F11: {
        this->setWindowState(windowState() ^ Qt::WindowFullScreen);
        return;
    }
    case Qt::Key_F12: {
        this->close();
        return;
    }
    default:
        break;
    }
}
//------------------------------------------------------------------------------
