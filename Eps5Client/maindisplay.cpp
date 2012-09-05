/**
* @file
* Definition of MainDisplay
*/
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <cmath>
#include "maindisplay.h"
#include "utils.h"

MainDisplay::MainDisplay(QWidget *parent) :
    QWidget(parent),
    _currentFrame(new QImage(800,600,QImage::Format_ARGB32)),
    _controlStatus(new Epsilon5::Control)
{
    _controlStatus->mutable_keystatus()->set_keyup(false);
    _controlStatus->mutable_keystatus()->set_keydown(false);
    _controlStatus->mutable_keystatus()->set_keyright(false);
    _controlStatus->mutable_keystatus()->set_keyleft(false);
    _controlStatus->mutable_keystatus()->set_keyattack1(false);
    _controlStatus->mutable_keystatus()->set_keyattack2(false);
    _controlStatus->set_angle(0);

    _peka = new QImage("peka.png");
    _mad = new QImage("mad.png");

    this->resize(800,600);
}

MainDisplay::~MainDisplay()
{
    delete _peka;
    delete _mad;
    delete _controlStatus;
}

void MainDisplay::start()
{
    //qDebug() <<  this->format().doubleBuffer();
    //startTimer(20); // TODO: Remove MN
}

void MainDisplay::redraw(const DrawableObjects &objects)
{
    _currentFrame->fill(Qt::black);
    QPainter painter(_currentFrame);
    QPen pen(Qt::green);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint gamerPos, cursorPos;

    QImage *tmp;
    //qDebug() << "Objs:" << objects.size();
    for (auto i=objects.begin();i!=objects.end();i++)
    {
        if (i->imageName == "selfPlayer")
        {
            tmp=_mad;
            gamerPos.setX(400+i->x);
            gamerPos.setY(300-i->y);
        }
        else if (i->imageName == "enemyPlayer")
            tmp=_peka;
        else tmp=_peka;

        painter.drawImage(400+i->x,300-i->y, *tmp);
    }

    this->update();

    cursorPos = this->mapFromGlobal(QCursor::pos());

    double angle = getAngle(cursorPos - gamerPos);

    _controlStatus->set_angle(angle);
}

void MainDisplay::timerEvent(QTimerEvent *event)
{
    emit requestRedrawObjects();
}

void MainDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(0, 0, *_currentFrame);
}

void MainDisplay::mousePressEvent(QMouseEvent *)
{
    _controlStatus->mutable_keystatus()->set_keyattack1(true);
}

void MainDisplay::mouseReleaseEvent(QMouseEvent *)
{
    _controlStatus->mutable_keystatus()->set_keyattack1(false);
}

void MainDisplay::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:               
        _controlStatus->mutable_keystatus()->set_keyup(true);
        break;
    case Qt::Key_Down:
        _controlStatus->mutable_keystatus()->set_keydown(true);
        break;
    case Qt::Key_Right:
        _controlStatus->mutable_keystatus()->set_keyright(true);
        break;
    case Qt::Key_Left:
        _controlStatus->mutable_keystatus()->set_keyleft(true);
        break;
    default:
        break;
    }
}

void MainDisplay::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        _controlStatus->mutable_keystatus()->set_keyup(false);
        break;
    case Qt::Key_Down:
        _controlStatus->mutable_keystatus()->set_keydown(false);
        break;
    case Qt::Key_Right:
        _controlStatus->mutable_keystatus()->set_keyright(false);
        break;
    case Qt::Key_Left:
        _controlStatus->mutable_keystatus()->set_keyleft(false);
        break;
    default:
        break;
    }
}
