#include "gamemodel.h"
//------------------------------------------------------------------------------
TGameModel::TGameModel(TApplication* parent)
    : QObject(parent)
    , WorldMap(new TMap(this))
    , PlayerId(0)
{
    connect(WorldMap, SIGNAL(MapLoaded()), SIGNAL(MapLoaded()));

    PlayerControl.set_angle(0);
    PlayerControl.mutable_keystatus()->set_keyattack1(false);
    PlayerControl.mutable_keystatus()->set_keyattack2(false);
    PlayerControl.mutable_keystatus()->set_keyleft(false);
    PlayerControl.mutable_keystatus()->set_keyright(false);
    PlayerControl.mutable_keystatus()->set_keyup(false);
    PlayerControl.mutable_keystatus()->set_keydown(false);
    PlayerControl.set_weapon(Epsilon5::Pistol);
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
Epsilon5::Control* TGameModel::GetPlayerControl()
{
    return &PlayerControl;
}
//------------------------------------------------------------------------------
void TGameModel::Update()
{
}
//------------------------------------------------------------------------------
