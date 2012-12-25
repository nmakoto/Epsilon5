#pragma once
#include <QObject>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "../Epsilon5-Proto/defines.h"
#include "application.h"
#include "map.h"
//------------------------------------------------------------------------------
enum ETeam {
    T_One,
    T_Second,
    T_Neutral
};
//------------------------------------------------------------------------------
struct TRespPoint {
    int X;
    int Y;
    ETeam Team;
};
//------------------------------------------------------------------------------
struct TPlayerStat {
    size_t Id;
    size_t Score;
    size_t Deaths;
    size_t Kills;
};
//------------------------------------------------------------------------------
class TGameModel : public QObject
{
    Q_OBJECT
public:
    typedef QVector<TPlayerStat> TStatVector;
public:
    explicit TGameModel(TApplication* parent);
    void Init();
    void Update();

    const Epsilon5::World& GetWorld() const;
    Epsilon5::World* GetWorld();

    void LoadMap(const QString& mapName);
    const TMap* GetMap() const;
    QString GetCurrentMapName() const;

    size_t GetPlayerId() const;
    void SetPlayerInfo(const Epsilon5::PlayerInfo& info);
    const Epsilon5::PlayerInfo* GetPlayerInfo() const;
    quint32 GetPlayerPing() const;

    Epsilon5::Control* GetPlayerControl();

signals:
    void MapLoaded();

private:
    Epsilon5::World GameWorld;
    TMap* WorldMap;
    size_t PlayerId;
    Epsilon5::PlayerInfo PlayerInfo;
    Epsilon5::Control PlayerControl;
    TStatVector Statistic;
};
//------------------------------------------------------------------------------
