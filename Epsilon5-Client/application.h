#pragma once
#include <QApplication>
#include <QDateTime>
#include "maindisplay.h"
#include "network.h"
#include "settings.h"
#include "gamemodel.h"
//------------------------------------------------------------------------------
class TGameModel;
class TSound;
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

    inline TMainDisplay* GetMainDisplay() {
        return MainDisplay;
    }
    inline TNetwork* GetNetwork() {
        return Network;
    }
    inline TSettings* GetSettings() {
        return Settings;
    }
    inline TGameModel* GetModel() {
        return GameModel;
    }
    inline TSound* GetSound() {
        return Sound;
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

    void PrepareMap(const Epsilon5::PlayerInfo& info);
    void UpdateWorld();
    void GameClose();

private:
    TSound* Sound;
    TSettings* Settings;
    TGameModel* GameModel;
    TNetwork* Network;
    TMainDisplay* MainDisplay;
    EGameState State;
};
//------------------------------------------------------------------------------
