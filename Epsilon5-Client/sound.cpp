#include <QDebug>
#include "sound.h"
//------------------------------------------------------------------------------
TSound::TSound(TApplication* app, QObject* parent)
    : QObject(parent)
    , Application(app)
    , SoundContainer(new TSoundContainer(this))
    , MenuSounds(new TSoundObject(this))
    , BackgroundMusic(new TSoundObject(this))
{
    SoundContainer->addSound(MenuSounds);
    SoundContainer->addSound(BackgroundMusic);

    lastState = Application->GetState();

    MenuSounds->openFile("sounds/hit-01.wav", "menu-click");
    BackgroundMusic->openFile("sounds/JewelBeat-Game_Adventure-mono.wav", "music-menu", true);
    BackgroundMusic->openFile("sounds/JewelBeat-Great_Escape-mono.wav", "music-battle00", true);

    qDebug() << BackgroundMusic->sourceInfo("music-menu").position[0]
        << BackgroundMusic->sourceInfo("music-menu").position[1]
        << BackgroundMusic->sourceInfo("music-menu").position[2];

    BackgroundMusic->setPosition(0,0,2, "music-menu");
    BackgroundMusic->setPosition(0,0,0, "music-battle00");

    CurrentMusic = BackgroundMusic->sourceInfo("music-menu");

    startTimer(30);
}
//------------------------------------------------------------------------------
TSound::~TSound()
{
}
//------------------------------------------------------------------------------
void TSound::Init()
{
}
//------------------------------------------------------------------------------
void TSound::UpdateSounds()
{
}
//------------------------------------------------------------------------------
void TSound::MenuItemClicked()
{
    MenuSounds->play("menu-click");
}
//------------------------------------------------------------------------------
void TSound::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if( lastState != Application->GetState() ) {
        if( Application->GetState() == ST_MainMenu ) {
            BackgroundMusic->pause(CurrentMusic.name);
            BackgroundMusic->play("music-menu");
            CurrentMusic = BackgroundMusic->sourceInfo("music-menu");
        } else if( Application->GetState() == ST_InGame ) {
            BackgroundMusic->pause(CurrentMusic.name);
            BackgroundMusic->play("music-battle00");
            CurrentMusic = BackgroundMusic->sourceInfo("music-battle00");
        }
    }

    if( !CurrentMusic.name.isEmpty() )
        BackgroundMusic->update(CurrentMusic.name);
}
//------------------------------------------------------------------------------
