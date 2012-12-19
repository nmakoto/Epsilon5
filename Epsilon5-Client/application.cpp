#include "application.h"
//------------------------------------------------------------------------------
TApplication::TApplication(int& argc, char* argv[])
    : QApplication(argc, argv)
    , Settings(new TSettings(this))
    , GameModel(new TGameModel(this))
    , Network(new TNetwork(this))
    , MainDisplay(new TMainDisplay(this))
    , State(ST_MainMenu)
{
    connect(Network, SIGNAL(WorldReceived()), SLOT(UpdateWorld()));
    connect(Network, SIGNAL(PlayerInfoReceived(Epsilon5::PlayerInfo)),
            SLOT(PrepareMap(Epsilon5::PlayerInfo)));

    connect(MainDisplay, SIGNAL(QuitAction()), SLOT(GameClose()));
    connect(MainDisplay, SIGNAL(MainMenuAction()), SLOT(ShowMainMenu()));
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
    return true;
}
//------------------------------------------------------------------------------
void TApplication::ShowMainMenu()
{
    Network->Disconnect();
    State = ST_MainMenu;
}
//------------------------------------------------------------------------------
void TApplication::GameClose()
{
    Network->Disconnect();
    MainDisplay->close();
}
//------------------------------------------------------------------------------
void TApplication::PrepareMap(Epsilon5::PlayerInfo info)
{
    GameModel->SetPlayerInfo(info);
    GameModel->LoadMap(info.map().c_str());
}
//------------------------------------------------------------------------------
void TApplication::UpdateWorld()
{

}
//------------------------------------------------------------------------------
