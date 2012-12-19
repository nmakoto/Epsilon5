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
    connect(Network, SIGNAL(Disconnected()), SLOT(SetMainMenuState()));
    connect(Network, SIGNAL(PlayerInfoReceived(const Epsilon5::PlayerInfo&)),
            SLOT(PrepareMap(const Epsilon5::PlayerInfo&)));

    connect(MainDisplay, SIGNAL(QuitAction()), SLOT(GameClose()));
    connect(MainDisplay, SIGNAL(MainMenuAction()), SLOT(SetMainMenuState()));
    connect(MainDisplay, SIGNAL(RespawnSelectedAction()), SLOT(SetInGameState()));

    connect(GameModel, SIGNAL(MapLoaded()), SLOT(SetSelectingRespawnState()));
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
void TApplication::SetMainMenuState()
{
    qDebug() << Q_FUNC_INFO;
    State = ST_MainMenu;
    Network->Disconnect();
}
//------------------------------------------------------------------------------
void TApplication::SetConnectingState()
{
    qDebug() << Q_FUNC_INFO;
    State = ST_Connecting;
    Network->Connect();
}
//------------------------------------------------------------------------------
void TApplication::SetLoadingMapState()
{
    qDebug() << Q_FUNC_INFO;
    State = ST_LoadingMap;
    GameModel->LoadMap(GameModel->GetCurrentMapName());
}
//------------------------------------------------------------------------------
void TApplication::SetSelectingRespawnState()
{
    qDebug() << Q_FUNC_INFO;
    State = ST_SelectingResp;
}
//------------------------------------------------------------------------------
void TApplication::SetInGameState()
{
    qDebug() << Q_FUNC_INFO;
    State = ST_InGame;
}
//------------------------------------------------------------------------------
void TApplication::PrepareMap(const Epsilon5::PlayerInfo& info)
{
    GameModel->SetPlayerInfo(info);
    SetLoadingMapState();
}
//------------------------------------------------------------------------------
void TApplication::UpdateWorld()
{
}
//------------------------------------------------------------------------------
void TApplication::GameClose()
{
    SetMainMenuState();
    MainDisplay->close();
}
//------------------------------------------------------------------------------
