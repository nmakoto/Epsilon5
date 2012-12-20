#pragma once
#include <QObject>
//------------------------------------------------------------------------------
class TFrameContainer : public QObject
{
    Q_OBJECT
public:
    explicit TFrameContainer(QObject *parent = 0);

    void addWidget(QWidget* widget);

protected:
    bool eventFilter(QObject* object, QEvent* event);

private:
    QList<QWidget*> Frames;
};
//------------------------------------------------------------------------------
