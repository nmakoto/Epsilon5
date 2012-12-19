#include "gamemodel.h"
//------------------------------------------------------------------------------
TGameModel::TGameModel(TApplication* parent)
    : QObject(parent)
    , WorldMap(new TMap(this))
    , PlayerId(0)
{
    connect(WorldMap, SIGNAL(MapLoaded()), SIGNAL(MapLoaded()));
}
//------------------------------------------------------------------------------
void TGameModel::Init()
{
}
//------------------------------------------------------------------------------
const Epsilon5::World& TGameModel::GetWorld() const
{
    return GameWorld;
}
//------------------------------------------------------------------------------
Epsilon5::World* TGameModel::GetWorld()
{
    return &GameWorld;
}
//------------------------------------------------------------------------------
void TGameModel::LoadMap(const QString& mapName)
{
    WorldMap->LoadMap(mapName);
}
//------------------------------------------------------------------------------
const TMap* TGameModel::GetMap() const
{
    return WorldMap;
}
//------------------------------------------------------------------------------
size_t TGameModel::GetPlayerId() const
{
    return PlayerId;
}
//------------------------------------------------------------------------------
void TGameModel::SetPlayerInfo(const Epsilon5::PlayerInfo& info)
{
    PlayerInfo = info;
    PlayerId = info.id();
}
//------------------------------------------------------------------------------
const Epsilon5::PlayerInfo* TGameModel::GetPlayerInfo() const
{
    return &PlayerInfo;
}
//------------------------------------------------------------------------------
QString TGameModel::GetCurrentMapName() const
{
    return PlayerInfo.map().c_str();
}
//------------------------------------------------------------------------------
