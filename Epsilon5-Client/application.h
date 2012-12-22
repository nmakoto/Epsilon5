#pragma once
#include <QApplication>
#include <QDateTime>
//#include "maindisplay.h"
#include "network.h"
#include "settings.h"
#include "gamemodel.h"
#include "gamewindow.h"
//------------------------------------------------------------------------------
class TGameModel;
class TGameWindow;
//------------------------------------------------------------------------------
enum EGameState {
    ST_MainMenu,
    ST_Connecting,
    ST_LoadingMap,
    ST_SelectingResp,
    ST_InGame
};
//------------------------------------------------------------------------------
class TApplication : public QApplication
{
    Q_OBJECT
public:
    TApplication(int& argc, char* argv[]);
    ~TApplication();
    bool Init();

//    inline TMainDisplay* GetMainDisplay() {
//        return MainDisplay;
//    }
    inline TGameWindow* GetGameView() {
        return GameWindow;
    }
    inline TNetwork* GetNetwork() {
        return Network;
    }
    inline TSettings* GetSettings() {
        return Settings;
    }
    inline TGameModel* GetModel() const {
        return GameModel;
    }
    inline EGameState GetState() {
        return State;
    }

public slots:
    void SetMainMenuState();
    void SetConnectingState();
    void SetLoadingMapState();
    void SetSelectingRespawnState();
    void SetInGameState();

#ifdef QT_DEBUG
    void ToggleRespawnFrame();
#endif

    void PrepareMap(const Epsilon5::PlayerInfo& info);
    void UpdateWorld();
    void GameClose();

private:
    TSettings* Settings;
    TGameModel* GameModel;
    TGameWindow* GameWindow;
    TNetwork* Network;
//    TMainDisplay* MainDisplay;
    EGameState State;
};
//------------------------------------------------------------------------------
