#include "gamewindow.h"
#include "application.h"
//------------------------------------------------------------------------------
TApplication::TApplication(int& argc, char* argv[])
    : QApplication(argc, argv)
    , Settings(new TSettings(this))
    , GameModel(new TGameModel(this))
    , GameWindow(new TGameWindow(this))
    , Network(new TNetwork(this))
//    , MainDisplay(new TMainDisplay(this))
    , State(ST_MainMenu)
{
    connect(Network, SIGNAL(WorldReceived()), SLOT(UpdateWorld()));
    connect(Network, SIGNAL(Disconnected()), SLOT(SetMainMenuState()));
    connect(Network, SIGNAL(PlayerInfoReceived(const Epsilon5::PlayerInfo&)),
            SLOT(PrepareMap(const Epsilon5::PlayerInfo&)));

//    connect(MainDisplay, SIGNAL(QuitAction()), SLOT(GameClose()));
//    connect(MainDisplay, SIGNAL(MainMenuAction()), SLOT(SetMainMenuState()));
//    connect(MainDisplay, SIGNAL(RespawnSelectedAction()), SLOT(SetInGameState()));
#ifdef QT_DEBUG
//    connect(MainDisplay, SIGNAL(ToggleRespawnFrameAction()), SLOT(ToggleRespawnFrame()));
#endif
    connect(GameWindow, SIGNAL(QuitAction()), SLOT(GameClose()));

//    connect(GameModel, SIGNAL(MapLoaded()), SLOT(SetSelectingRespawnState()));
    connect(GameModel, SIGNAL(MapLoaded()), SLOT(SetInGameState()));
}
//------------------------------------------------------------------------------
TApplication::~TApplication()
{
    // Delete MainDisplay here because it has no any QWidget parent
//    delete MainDisplay;
    delete GameWindow;
}
//------------------------------------------------------------------------------
bool TApplication::Init()
{
    GameModel->Init();
    Network->Init();
//    MainDisplay->Init();
//    MainDisplay->show();
    GameWindow->Init();

    SetConnectingState();
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
//    MainDisplay->PrepareMapDraw();
    GameWindow->PrepareView();
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
//    MainDisplay->close();
}
//------------------------------------------------------------------------------
#ifdef QT_DEBUG
void TApplication::ToggleRespawnFrame()
{
    if(State == ST_InGame) {
        SetSelectingRespawnState();
        return;
    }
    if(State == ST_SelectingResp) {
        SetInGameState();
        return;
    }
}
#endif
//------------------------------------------------------------------------------
