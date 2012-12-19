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

    const TMap* GetMap() const;
//    TMap* GetMap();

public slots:
    void LoadMap(QString mapName);

private:
    Epsilon5::World GameWorld;
    TMap* WorldMap;
};
//------------------------------------------------------------------------------
