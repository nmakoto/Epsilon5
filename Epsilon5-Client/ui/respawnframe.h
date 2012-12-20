#pragma once
#include <QObject>
#include "ui/uiframe.h"
//------------------------------------------------------------------------------
class TUiButton;
class QPushButton;
//------------------------------------------------------------------------------
class TRespawnFrame : public TUiFrame
{
    Q_OBJECT
public:
    explicit TRespawnFrame(QWidget* parent = 0);
    ~TRespawnFrame();

    void Paint(QPainter& painter);

private:
    TUiButton* CloseButton;
};
//------------------------------------------------------------------------------
