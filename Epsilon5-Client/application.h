#pragma once
#include <QApplication>
#include <QDateTime>
#include "maindisplay.h"
#include "network.h"
#include "settings.h"
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
    inline EGameState GetState() {
        return State;
    }
    inline void SetState(EGameState state) {
        State = state;
    }

public slots:
    inline void Disconnected() {
        State = ST_MainMenu;
    }

private:
    TMainDisplay* MainDisplay;
    TNetwork* Network;
    TSettings* Settings;
    EGameState State;
};
//------------------------------------------------------------------------------
