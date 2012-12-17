#include <QDebug>
#include "../utils/usoundcontainer.h"
//------------------------------------------------------------------------------
using namespace utils;
//------------------------------------------------------------------------------
namespace
{
//------------------------------------------------------------------------------
bool checkALCError(ALCdevice* device)
{
    ALenum errorCode = alcGetError(device);
    if (errorCode == AL_NO_ERROR) {
        return true;
    }

    qDebug("ALC Error: %s", alcGetString(device, errorCode));
    return false;
}
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
USoundContainer::USoundContainer(QObject* parent)
    : QObject(parent)
    , mDevice(0)
    , mContext(0)
{
    initOpenAL();
}
//------------------------------------------------------------------------------
USoundContainer::~USoundContainer()
{
    for (auto it = mSounds.begin(); it != mSounds.end(); ++it) {
        (*it)->close();
    }
    destroyOpenAL();
}
//------------------------------------------------------------------------------
void USoundContainer::initOpenAL()
{
    ALfloat listenerPosition[] = { 0.0, 0.0, 2.0 };
    ALfloat listenerVelocity[] = { 0.0, 0.0, 0.0 };
    ALfloat listenerOrientation[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

#ifdef USE_ALUT
    if (!alutInitWithoutContext(NULL, NULL)) {
        qDebug() << Q_FUNC_INFO << ":: ALUT initialization failed";
        return;
    }
#endif
    mDevice = alcOpenDevice(NULL);
    if (!mDevice) {
        qDebug() << Q_FUNC_INFO << ":: default sound device not present";
        return;
    }

    mContext = alcCreateContext(mDevice, NULL);
    if (!checkALCError(mDevice)) {
        return;
    }

    alcMakeContextCurrent(mContext);

    alListenerfv(AL_POSITION, listenerPosition);
    alListenerfv(AL_VELOCITY, listenerVelocity);
    alListenerfv(AL_ORIENTATION, listenerOrientation);
}
//------------------------------------------------------------------------------
void USoundContainer::destroyOpenAL()
{
#ifdef USE_ALUT
    alutExit();
#endif
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}
//------------------------------------------------------------------------------
void USoundContainer::addSound(USound* sound)
{
    sound->init(mDevice, mContext);
    sound->setParent(this);
    mSounds.append(sound);
}
//------------------------------------------------------------------------------
int USoundContainer::count() const
{
    return mSounds.count();
}
//------------------------------------------------------------------------------
bool USoundContainer::isValid() const
{
    return mDevice && mContext;
}
//------------------------------------------------------------------------------
