#pragma once
#include <QApplication>
#include <QDateTime>
#include "network.h"
#include "settings.h"
#include "gamemodel.h"
//------------------------------------------------------------------------------
enum EGameState {
    ST_MainMenu,
    ST_Connecting,
    ST_LoadingMap,
    ST_SelectingResp,
    ST_InGame
};
//------------------------------------------------------------------------------
class TGameModel;
class TGameWindow;
//------------------------------------------------------------------------------
class TApplication : public QApplication
{
    Q_OBJECT
public:
    TApplication(int& argc, char* argv[]);
    ~TApplication();

    bool Init();

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
    inline EGameState GetState() const {
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
    EGameState State;
};
//------------------------------------------------------------------------------
