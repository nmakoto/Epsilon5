#include <QPaintEvent>
#include "ui/uibutton.h"
//------------------------------------------------------------------------------
TUiButton::TUiButton(QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(120, 40);
    setStyleSheet("* { background: transparent; border: 2px solid black; }");
}
//------------------------------------------------------------------------------
