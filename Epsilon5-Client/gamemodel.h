#pragma once
#include <QObject>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "../Epsilon5-Proto/defines.h"
#include "application.h"
#include "map.h"
//------------------------------------------------------------------------------
class TGameModel : public QObject
{
    Q_OBJECT
public:
    explicit TGameModel(TApplication* parent);
    void Init();

    const Epsilon5::World& GetWorld() const;
    Epsilon5::World* GetWorld();

    void LoadMap(const QString& mapName);
    const TMap* GetMap() const;

    size_t GetPlayerId() const;
    void SetPlayerInfo(const Epsilon5::PlayerInfo& info);
    const Epsilon5::PlayerInfo* GetPlayerInfo() const;

public slots:

private:
    Epsilon5::World GameWorld;
    TMap* WorldMap;
    size_t PlayerId;
    Epsilon5::PlayerInfo PlayerInfo;
};
//------------------------------------------------------------------------------
