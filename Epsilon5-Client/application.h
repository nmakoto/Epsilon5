#pragma once
#include <QApplication>
#include <QDateTime>
#include "maindisplay.h"
#include "network.h"
#include "settings.h"
#include "gamemodel.h"
//------------------------------------------------------------------------------
class TGameModel;
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
    inline EGameState GetState() {
        return State;
    }
    inline void SetState(EGameState state) {
        State = state;
    }

public slots:
    void ShowMainMenu();
    void GameClose();
    void PrepareMap(Epsilon5::PlayerInfo info);
    void UpdateWorld();

private:
    TSettings* Settings;
    TGameModel* GameModel;
    TNetwork* Network;
    TMainDisplay* MainDisplay;
    EGameState State;
};
//------------------------------------------------------------------------------
