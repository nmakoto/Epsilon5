#include "application.h"
//------------------------------------------------------------------------------
TApplication::TApplication(int& argc, char* argv[])
    : QApplication(argc, argv)
    , MainDisplay(new TMainDisplay(this))
    , Network(new TNetwork(this))
    , Settings(new TSettings(this))
    , State(ST_MainMenu)
{
    connect(Network, SIGNAL(WorldReceived()), MainDisplay, SLOT(RedrawWorld()));
    connect(Network, SIGNAL(Disconnected()), SLOT(Disconnected()));
}
//------------------------------------------------------------------------------
TApplication::~TApplication()
{
    // Delete MainDisplay here because it has no any QWidget parent
    delete MainDisplay;
}
//------------------------------------------------------------------------------
bool TApplication::Init()
{
    MainDisplay->Init();
    MainDisplay->show();
    if (Settings->GetWindowFullscreen()) {
        MainDisplay->toggleFullscreen();
    }
    return true; // TODO: normal initialisation
}
//------------------------------------------------------------------------------
