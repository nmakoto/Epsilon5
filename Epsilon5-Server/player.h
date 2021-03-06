#pragma once

#include <QTime>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "dynamicobject.h"
#include "maps.h"

class TBullet;

struct TFireInfo {
    double X, Y, Vx, Vy, Angle;
    Epsilon5::Weapon Weapon;
    bool PrimaryAttack;
    size_t PlayerId;
    ETeam Team;
};

class TPlayer : public TDynamicObject
{
    Q_OBJECT
public:
    TPlayer(size_t id, ETeam team, TMaps* maps, QObject *parent = 0);
    inline size_t GetId() { return Id; }
    inline size_t GetHP() { return HP; }
    void Hit(size_t playerId, quint8 ffMode = 100);
    void ApplyCustomPhysics();
    void SetNickname(const QString& nickName);
    inline QString GetNickname() {
        return NickName;
    }
    inline void SetPing(int ping) {
        Ping = ping;
    }
    inline int GetPing() {
        return Ping;
    }
    inline ETeam GetTeam() {
        return Team;
    }
    inline bool GetTeamBool() {
        return Team == T_One;
    }

signals:
    void Death(size_t id);
    void Killed(size_t playerId);
    void Fire(TFireInfo& fireInfo);

public slots:
    void ApplyControl(const Epsilon5::Control &control);

private:
    b2Vec2 Force;
    size_t Id;
    QString NickName;
    TMaps* Maps;
    size_t HP;
    TObjectInfo CollisionInfo;
    ETeam Team;
    int Ping;
};
