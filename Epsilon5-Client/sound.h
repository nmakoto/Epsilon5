#pragma once
#include <QObject>
#include "../utils/usound.h"
#include "../utils/usoundcontainer.h"
#include "application.h"
//------------------------------------------------------------------------------
typedef utils::TSourceInfo TSoundInfo;
typedef utils::USound TSoundObject;
typedef utils::USoundContainer TSoundContainer;
//------------------------------------------------------------------------------
class TSound : public QObject
{
    Q_OBJECT
public:
    explicit TSound(TApplication* app, QObject* parent);
    ~TSound();

    void Init();

public slots:
    void UpdateSounds();
    void MenuItemClicked();

protected:
    void timerEvent(QTimerEvent* event);

private:
    TApplication* Application;
    const Epsilon5::World* CurrentWorld;
    TSoundContainer* SoundContainer;
    TSoundObject* MenuSounds;
    TSoundObject* BackgroundMusic;
    TSoundObject* PlayerSounds;
    TSoundObject* AmbientSounds;
    TSoundInfo CurrentMusic;
    EGameState lastState;
    QPoint LastPlayerPos;
};
//------------------------------------------------------------------------------
