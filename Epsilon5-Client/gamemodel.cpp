#include "gamemodel.h"
//------------------------------------------------------------------------------
TGameModel::TGameModel(TApplication* parent)
    : QObject(parent)
    , WorldMap(new TMap(this))
{
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
void TGameModel::LoadMap(QString mapName)
{
    WorldMap->LoadMap(mapName);
}
//------------------------------------------------------------------------------
const TMap* TGameModel::GetMap() const
{
    return WorldMap;
}
//------------------------------------------------------------------------------
