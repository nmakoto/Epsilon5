#include "application.h"
#include "sound.h"

TApplication::TApplication(int& argc, char* argv[])
    : QApplication(argc, argv)
    , Sound(new TSound(this))
    , MainDisplay(this)
    , Network(new TNetwork(this))
    , Settings(new TSettings(this))
    , State(ST_MainMenu)
{
    connect(Network, SIGNAL(WorldReceived()), &MainDisplay, SLOT(RedrawWorld()));
    connect(Network, SIGNAL(WorldReceived()), Sound, SLOT(UpdateSounds()));
    connect(Network, SIGNAL(Disconnected()), SLOT(Disconnected()));
}

bool TApplication::Init() {
    Sound->Init();
    MainDisplay.Init();
    MainDisplay.show();
    if( Settings->GetWindowFullscreen() )
        MainDisplay.toggleFullscreen();
    return true; // TODO: normal initialisation
}
