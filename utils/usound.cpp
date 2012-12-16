#include <QDebug>
#include <QFile>
#include "../utils/usound.h"
//------------------------------------------------------------------------------
using namespace utils;
//------------------------------------------------------------------------------
namespace {
//------------------------------------------------------------------------------
bool checkALError() {
    ALenum errorCode = alGetError();
    if( errorCode == AL_NO_ERROR )
        return true;
    qDebug( "ALError: %s", alGetString(errorCode));
    return false;
}
//------------------------------------------------------------------------------
bool checkALCError(ALCdevice* device) {
    ALenum errorCode = alcGetError(device);
    if( errorCode == AL_NO_ERROR )
        return true;

    qDebug( "ALCError: %s", alcGetString(device, errorCode));
    return false;
}
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
USound::USound(QObject* parent)
    : QObject(parent)
    , mDevice(0)
    , mContext(0)
    , mSourceId(0)
    , mLooped(0)
{
}
//------------------------------------------------------------------------------
USound::USound(const USound &sound)
    : QObject(sound.parent())
    , mDevice(sound.mDevice)
    , mContext(sound.mContext)
    , mSourceId(sound.mSourceId)
    , mLooped(sound.mLooped)
{
}
//------------------------------------------------------------------------------
USound::~USound()
{
    alDeleteBuffers(1, &mSourceId);
}
//------------------------------------------------------------------------------
USound& USound::operator =(const USound& sound)
{
    mDevice = sound.mDevice;
    mContext = sound.mContext;
    mSourceId = sound.mSourceId;
    mLooped = sound.mLooped;
    return *this;
}
//------------------------------------------------------------------------------
void USound::init(ALCdevice *device, ALCcontext *context)
{
    mDevice = device;
    mContext = context;
}
//------------------------------------------------------------------------------
bool USound::isValid() const
{
    return mDevice && mContext;
}
//------------------------------------------------------------------------------
bool USound::loadWavFile(const QString &fileName)
{
    if( !isValid() ) {
        qDebug() << Q_FUNC_INFO << ":: sound object not valid";
        return false;
    }

    TSoundInfo buffer;
    ALvoid *data;
    ALsizei size;
    ALboolean loop;

    buffer.fileName = fileName.trimmed();
    for( auto it = mContainer.constBegin(); it != mContainer.constEnd(); ++it) {
        if( (*it).fileName == buffer.fileName ) {
            buffer = mContainer[(*it).ID];
            alSourcei(mSourceId, AL_BUFFER, buffer.ID);
            return true;
        }
    }

    alGenBuffers(1, &buffer.ID);
    if( !checkALError() )
        return false;

    alutLoadWAVFile((ALbyte*) qPrintable(buffer.fileName), &buffer.format,
            &data, &size, &buffer.rate, &loop);
    if( !checkALError() )
        return false;

    alBufferData(buffer.ID, buffer.format, data, size, buffer.rate);
    if( !checkALError() )
        return false;

    alutUnloadWAV(buffer.format, data, size, buffer.rate);
    if( !checkALError() )
        return false;

    mContainer[buffer.ID] = buffer;
    alSourcei(mSourceId, AL_BUFFER, buffer.ID);
    return true;
}
//------------------------------------------------------------------------------
bool USound::openFile(const QString &fileName, bool looped, bool streamed)
{
    if( !QFile::exists(fileName) )
        return false;

    mLooped = looped;
    mStreamed = streamed;

    alGenSources(1, &mSourceId);
    if( !checkALError() )
        return false;

    ALfloat listenerPosition[] = {0.0, 0.0, 0.0};
    ALfloat listenerVelocity[] = {0.0, 0.0, 0.0};

    alSourcef(mSourceId, AL_PITCH, 1.0f);
    alSourcef(mSourceId, AL_GAIN, 1.0f);
    alSourcefv(mSourceId, AL_POSITION, listenerPosition);
    alSourcefv(mSourceId, AL_VELOCITY, listenerVelocity);
    alSourcei(mSourceId, AL_LOOPING, mLooped);

    if( fileName.right(4).toLower() == ".wav" )
        return loadWavFile(fileName);

    return false;
}
//------------------------------------------------------------------------------
void USound::play()
{
    if( !isValid() )
        return;

    alSourcePlay(mSourceId);
}
//------------------------------------------------------------------------------
void USound::stop()
{
    if( !isValid() )
        return;

    alSourceStop(mSourceId);
}
//------------------------------------------------------------------------------
void USound::close()
{
    if( !isValid() )
        return;

    alSourceStop(mSourceId);
    if (alIsSource(mSourceId))
        alDeleteSources(1, &mSourceId);
}
//------------------------------------------------------------------------------
void USound::update()
{
}
//------------------------------------------------------------------------------
void USound::move(qreal x, qreal y, qreal z)
{
    ALfloat pos[3] = { (float)x, (float)y, (float)z };
    alSourcefv(mSourceId, AL_POSITION, pos);
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
    for( auto it = mSounds.begin(); it != mSounds.end(); ++it)
        (*it).close();

    destroyOpenAL();
}
//------------------------------------------------------------------------------
void USoundContainer::initOpenAL()
{
    ALfloat listenerPosition[] = { 0.0, 0.0, 2.0 };
    ALfloat listenerVelocity[] = { 0.0, 0.0, 0.0 };
    ALfloat listenerOrientation[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

    mDevice = alcOpenDevice(NULL);
    if( !mDevice )
    {
        qDebug("Default sound device not present");
        return;
    }

    mContext = alcCreateContext(mDevice, NULL);
    if( !checkALCError(mDevice) )
        return;

    alcMakeContextCurrent(mContext);

    alListenerfv(AL_POSITION, listenerPosition);
    alListenerfv(AL_VELOCITY, listenerVelocity);
    alListenerfv(AL_ORIENTATION, listenerOrientation);
}
//------------------------------------------------------------------------------
void USoundContainer::destroyOpenAL()
{
    alcMakeContextCurrent(NULL);
    if( mContext )
        alcDestroyContext(mContext);
    if( mDevice )
        alcCloseDevice(mDevice);
}
//------------------------------------------------------------------------------
void USoundContainer::addSound(USound &sound)
{
    sound.init(mDevice, mContext);
    sound.setParent(this);
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
