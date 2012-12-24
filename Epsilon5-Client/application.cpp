#include <QDebug>
#include "gamewindow.h"
#include "application.h"
//------------------------------------------------------------------------------
TApplication::TApplication(int& argc, char* argv[])
    : QApplication(argc, argv)
    , Settings(new TSettings(this))
    , GameModel(new TGameModel(this))
    , GameWindow(new TGameWindow(this))
    , Network(new TNetwork(this))
    , State(ST_MainMenu)
{
    connect(Network, SIGNAL(WorldReceived()), SLOT(UpdateWorld()));
    connect(Network, SIGNAL(Disconnected()), SLOT(SetMainMenuState()));
    connect(Network, SIGNAL(PlayerInfoReceived(const Epsilon5::PlayerInfo&)),
            SLOT(PrepareMap(const Epsilon5::PlayerInfo&)));

    connect(GameWindow, SIGNAL(MainMenuAction()), SLOT(SetMainMenuState()));
    connect(GameWindow, SIGNAL(ConnectAction()), SLOT(SetConnectingState()));
    connect(GameWindow, SIGNAL(QuitAction()), SLOT(GameClose()));

    connect(GameModel, SIGNAL(MapLoaded()), SLOT(SetInGameState()));
}
//------------------------------------------------------------------------------
TApplication::~TApplication()
{
    // Delete GameWindow here because it has no any QWidget parent
    delete GameWindow;
}
//------------------------------------------------------------------------------
bool TApplication::Init()
{
    GameModel->Init();
    Network->Init();
    GameWindow->Init();

//    SetConnectingState();
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
}
//------------------------------------------------------------------------------
#ifdef QT_DEBUG
void TApplication::ToggleRespawnFrame()
{
    if (State == ST_InGame) {
        SetSelectingRespawnState();
        return;
    }
    if (State == ST_SelectingResp) {
        SetInGameState();
        return;
    }
}
#endif
//------------------------------------------------------------------------------
