#include "gamemodel.h"
//------------------------------------------------------------------------------
TGameModel::TGameModel(TApplication* parent)
    : QObject(parent)
    , WorldMap(new TMap(this))
    , PlayerId(0)
    , FullWorldState(false)
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
// TODO: This is ugly. Rewrite.
quint32 TGameModel::GetPlayerPing() const
{
    for (int i = 0; i != GameWorld.players_size(); i++) {
        const Epsilon5::Player& player = GameWorld.players(i);
        if ((size_t)player.id() != PlayerId) {
            continue;
        }
        if (player.has_ping()) {
            return player.ping();
        }
    }
    return 0;
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
void TGameModel::SetFullWorldState(bool value)
{
    FullWorldState = value;
}
//------------------------------------------------------------------------------
bool TGameModel::GetFullWorldState() const
{
    return FullWorldState;
}
//------------------------------------------------------------------------------
