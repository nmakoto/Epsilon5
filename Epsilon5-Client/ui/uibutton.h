#pragma once
#include <QPushButton>
//------------------------------------------------------------------------------
class TUiButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TUiButton(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent* event);
//    bool event(QEvent *event);
};
//------------------------------------------------------------------------------
