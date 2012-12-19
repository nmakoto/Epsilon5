#include "application.h"
//------------------------------------------------------------------------------
TApplication::TApplication(int& argc, char* argv[])
    : QApplication(argc, argv)
    , GameModel(new TGameModel(this))
    , MainDisplay(new TMainDisplay(this))
    , Network(new TNetwork(this))
    , Settings(new TSettings(this))
    , State(ST_MainMenu)
{
    connect(Network, SIGNAL(WorldReceived()), MainDisplay, SLOT(RedrawWorld()));
    connect(Network, SIGNAL(Disconnected()), SLOT(Disconnected()));
    connect(Network, SIGNAL(LoadMap(QString)), GameModel, SLOT(LoadMap(QString)));
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
    GameModel->Init();
    Network->Init();
    MainDisplay->Init();
    MainDisplay->show();
    if (Settings->GetWindowFullscreen()) {
        MainDisplay->toggleFullscreen();
    }
    return true;
}
//------------------------------------------------------------------------------
