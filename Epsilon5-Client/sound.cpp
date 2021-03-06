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
    , AmbientSounds(new TSoundObject(this))
    , LastPlayerPos()
{
    SoundContainer->addSound(MenuSounds);
    SoundContainer->addSound(BackgroundMusic);
    SoundContainer->addSound(PlayerSounds);
    SoundContainer->addSound(AmbientSounds);

    lastState = Application->GetState();

    MenuSounds->openFile("sounds/hit-01.wav", "menu-click");
    BackgroundMusic->openFile(
        "sounds/JewelBeat-Game_Adventure-mono.wav", "music-menu", true);
    BackgroundMusic->openFile(
        "sounds/JewelBeat-Great_Escape-mono.wav", "music-battle00", true);
    PlayerSounds->openFile("sounds/walking-in-snow-mono.wav", "walk", true);
    PlayerSounds->openFile("sounds/machinegun-mono.wav", "mgun");
    PlayerSounds->openFile("sounds/shotgun-mono.wav", "sgun");
    PlayerSounds->openFile("sounds/pistol-mono.wav", "pgun");
    AmbientSounds->openFile("sounds/wind-loop-mono.wav", "wind", true);

    BackgroundMusic->setPosition(0, 0, 2, "music-menu");
    BackgroundMusic->setPosition(0, 0, 20, "music-battle00");
    PlayerSounds->setPosition(0, 0, 4, QStringList() << "mgun" << "sgun" << "pgun");
    PlayerSounds->setPitch(1.2f, "walk");
    AmbientSounds->setPosition(0, 0, 5, "wind");

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
    CurrentWorld = Application->GetModel()->GetWorld();
}
//------------------------------------------------------------------------------
void TSound::MenuItemClicked()
{
    MenuSounds->play("menu-click");
}
//------------------------------------------------------------------------------
void TSound::timerEvent(QTimerEvent* event)
{
    // TODO: Refactor this function then
    Q_UNUSED(event);
    if (lastState != Application->GetState()) {
        if (Application->GetState() == ST_MainMenu) {
            BackgroundMusic->pause(CurrentMusic.name);
            BackgroundMusic->play("music-menu");
            AmbientSounds->pause("wind");
            CurrentMusic = BackgroundMusic->sourceInfo("music-menu");
        } else if (Application->GetState() == ST_InGame) {
            BackgroundMusic->pause(CurrentMusic.name);
//            BackgroundMusic->play("music-battle00");
            CurrentMusic = BackgroundMusic->sourceInfo("music-battle00");
            AmbientSounds->play("wind");
        }
    }

    if (CurrentWorld) {
        const Epsilon5::Control& control = Application->GetMainDisplay()->GetControl();
        auto it = CurrentWorld->players().begin();
        for ( ; it != CurrentWorld->players().end(); ++it) {
            const Epsilon5::Player& player = (*it);
            if ((size_t)player.id() == Application->GetModel()->GetPlayerId()) {
                // player walking
                QPoint delta = QPoint(player.x(), player.y()) - LastPlayerPos;
                if (abs(delta.x()) > 2 || abs(delta.y()) > 2) {
                    PlayerSounds->play("walk");
                } else if (delta.x() == 0 && delta.y() == 0) {
                    PlayerSounds->pause("walk");
                }
                LastPlayerPos = QPoint(player.x(), player.y());

                // player bullets
                // TODO: rewrite this code -- it's durty hack
                for (int i = 0; i != CurrentWorld->bullets_size(); i++) {
                    const Epsilon5::Bullet& bullet = CurrentWorld->bullets(i);

                    if(bullet.team() == player.team() && control.keystatus().keyattack1() )
                        if( abs(bullet.x() - player.x()) < 50 - 20*(control.weapon() == Epsilon5::Pistol)
                                && abs(bullet.y() - player.y()) < 50 - 20*(control.weapon() == Epsilon5::Pistol) )
                        {
                            if( control.weapon() == Epsilon5::Pistol ) {
                                PlayerSounds->play("pgun", true);
                            }
                            else if( control.weapon() == Epsilon5::Machinegun )
                                PlayerSounds->play("mgun");
                            else if( control.weapon() == Epsilon5::Shotgun )
                                PlayerSounds->play("sgun", true);
                        }
                }
            }
        }
    }


    // Update streamed sound objects (only OGG with streaming option)
    if (!CurrentMusic.name.isEmpty()) {
        BackgroundMusic->update(CurrentMusic.name);
    }
}
//------------------------------------------------------------------------------
