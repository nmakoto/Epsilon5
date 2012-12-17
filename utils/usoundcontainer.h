#pragma once
#include <QObject>
#include <AL/alc.h>
#include "../utils/usound.h"
//------------------------------------------------------------------------------
namespace utils
{
//------------------------------------------------------------------------------
class USound;
//------------------------------------------------------------------------------
class USoundContainer : public QObject
{
    Q_OBJECT
    typedef QVector<USound*> TSoundVector;
public:
    explicit USoundContainer(QObject* parent = 0);
    ~USoundContainer();

    void addSound(USound* sound);
    int count() const;
    bool isValid() const;

private:
    void initOpenAL();
    void destroyOpenAL();

private:
    TSoundVector mSounds;
    ALCdevice* mDevice;
    ALCcontext* mContext;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
