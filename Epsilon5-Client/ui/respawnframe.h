#pragma once
#include <QObject>
#include "ui/uiframe.h"
//------------------------------------------------------------------------------
class TRespawnFrame : public TUiFrame
{
    Q_OBJECT
public:
    explicit TRespawnFrame(QObject* parent = 0);
    ~TRespawnFrame();

    void Paint(QPainter& painter);


};
//------------------------------------------------------------------------------
