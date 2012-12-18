#pragma once
#include <QObject>
#include "../utils/usettings.h"

class TSettings : public QObject
{
    Q_OBJECT
public:
    explicit TSettings(QObject* parent);

    QString GetServerAddress();
    quint16 GetServerPort();
    quint8 GetGameplayFriendlyFire();

    void Load();

private:
    utils::USettings* Settings;
};
