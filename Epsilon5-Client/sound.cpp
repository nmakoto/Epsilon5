#include <QDebug>
#include "sound.h"
//------------------------------------------------------------------------------
TSound::TSound(TApplication* app, QObject* parent)
    : QObject(parent)
    , Application(app)
    , CurrentWorld(0)
    , SoundContainer(new TSoundContainer(this))
    , MenuSounds(new TSoundObject(this))
    , BackgroundMusic(new TSoundObject(this))
    , PlayerSounds(new TSoundObject(this))
    , LastPlayerPos()
{
    SoundContainer->addSound(MenuSounds);
    SoundContainer->addSound(BackgroundMusic);
    SoundContainer->addSound(PlayerSounds);

    lastState = Application->GetState();

    MenuSounds->openFile("sounds/hit-01.wav", "menu-click");
    BackgroundMusic->openFile(
            "sounds/JewelBeat-Game_Adventure-mono.wav", "music-menu", true);
    BackgroundMusic->openFile(
            "sounds/JewelBeat-Great_Escape-mono.wav", "music-battle00", true);
    PlayerSounds->openFile("sounds/walking-in-snow-mono.wav", "walk", true);

    BackgroundMusic->setPosition(0,0,2, "music-menu");
    BackgroundMusic->setPosition(0,0,20, "music-battle00");

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
    CurrentWorld = &Application->GetNetwork()->GetWorld();
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

    if( CurrentWorld ) {
        for( auto it = CurrentWorld->players().begin(); it != CurrentWorld->players().end(); ++it ) {
            const Epsilon5::Player &player = (*it);
            if( (size_t)player.id() == Application->GetNetwork()->GetId() ) {
                if (LastPlayerPos.x() != player.x() || LastPlayerPos.y() != player.y()) {
                    LastPlayerPos = QPoint(player.x(), player.y());
                    PlayerSounds->play("walk");
                } else {
                    PlayerSounds->pause("walk");
                }
            }
        }
    }

    // Update streamed sound objects
    if( !CurrentMusic.name.isEmpty() )
        BackgroundMusic->update(CurrentMusic.name);
}
//------------------------------------------------------------------------------
