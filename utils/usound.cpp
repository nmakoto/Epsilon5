#include <QDebug>
#include <QFile>
#include "../utils/usound.h"
//------------------------------------------------------------------------------
using namespace utils;
//------------------------------------------------------------------------------
const quint32 BUFFER_BLOCK_SIZE = 1024 * 1024;
const quint32 BUFFERS_COUNT = 5;
//------------------------------------------------------------------------------
namespace
{
//------------------------------------------------------------------------------
bool checkALError()
{
    ALenum errorCode = alGetError();
    if (errorCode == AL_NO_ERROR) {
        return true;
    }
    qDebug("AL Error: %s", alGetString(errorCode));
    return false;
}
//------------------------------------------------------------------------------
#ifdef USE_ALUT
bool checkALUTError()
{
    ALenum errorCode = alutGetError();
    if (errorCode == AL_NO_ERROR) {
        return true;
    }
    qDebug("ALUT Error: %s", alutGetErrorString(errorCode));
    return false;
}
#endif
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
#ifdef USE_OGGVORBIS
//------------------------------------------------------------------------------
size_t oggRead(void* ptr, size_t size, size_t nmemb, void* datasource)
{
    QFile* file = reinterpret_cast<QFile*>(datasource);
    return file->read((char*)ptr, size * nmemb);
}
//------------------------------------------------------------------------------
int oggSeek(void* datasource, ogg_int64_t offset, int whence)
{
    QFile* file = reinterpret_cast<QFile*>(datasource);
    if (whence == SEEK_SET) {
        return file->seek(offset) - 1;
    }
    if (whence == SEEK_CUR) {
        return file->seek(file->pos() + offset) - 1;
    }
    if (whence == SEEK_END) {
        return file->seek(file->size() + offset) - 1;
    }
    return -1;
}
//------------------------------------------------------------------------------
long oggTell(void* datasource)
{
    QFile* file = reinterpret_cast<QFile*>(datasource);
    return file->pos();
}
//------------------------------------------------------------------------------
int oggClose(void* datasource)
{
    Q_UNUSED(datasource);
    return 0;
}
//------------------------------------------------------------------------------
#endif
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
USound::USound(const USound& sound)
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
void USound::init(ALCdevice* device, ALCcontext* context)
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
#ifdef USE_ALUT
bool USound::loadWavFile(const QString& fileName)
{
    if (!isValid()) {
        qDebug() << Q_FUNC_INFO << ":: sound object not valid";
        return false;
    }

    TSoundInfo buffer;
    buffer.fileName = fileName.trimmed();
    for (auto it = mBuffers.constBegin(); it != mBuffers.constEnd(); ++it) {
        if ((*it).fileName == buffer.fileName) {
            buffer = mBuffers[(*it).ID];
            alSourcei(mSourceId, AL_BUFFER, buffer.ID);
            return true;
        }
    }

    ALint id = alutCreateBufferFromFile(qPrintable(buffer.fileName));
    if (!checkALUTError() || id == AL_NONE) {
        return false;
    }

    buffer.ID = id;
    mBuffers[buffer.ID] = buffer;
    alSourcei(mSourceId, AL_BUFFER, buffer.ID);
    return true;
}
#endif
//------------------------------------------------------------------------------
#ifdef USE_OGGVORBIS
bool USound::ReadOggBlock(ALuint bufferId, size_t size)
{
    char* data;

    if (size < 1) {
        return false;
    }
    data = new char[size];

    int currentSection;
    int readSize;
    size_t readSizeTotal = 0;
    while ((size_t)readSizeTotal < size) {
        readSize = ov_read(mOggVorbisFile,
                   data + readSizeTotal, size - readSizeTotal,
                   0, 2, 1, &currentSection);

        if (readSize == 0) {
            break;
        }
        if (readSize < 0) {
            qDebug() << Q_FUNC_INFO << ":: stream error";
            break;
        }
        readSizeTotal += readSize;
    }
    if (readSizeTotal > 0) {
        alBufferData(bufferId, mBuffers[bufferId].format, (void*)data,
                     readSizeTotal, mBuffers[bufferId].rate);
        checkALError();
    }
    delete [] data;
    return (readSize > 0);
}
//------------------------------------------------------------------------------
bool USound::loadOggFile(const QString& fileName, bool streamed)
{
    ov_callbacks cb;
    cb.close_func = oggClose;
    cb.read_func = oggRead;
    cb.seek_func = oggSeek;
    cb.tell_func = oggTell;

    mOggVorbisFile = new OggVorbis_File;
    mOggFile.setFileName(fileName);
    mOggFile.open(QFile::ReadOnly);

    if (ov_open_callbacks(&mOggFile, mOggVorbisFile, NULL, -1, cb) < 0) {
        return false;
    }

    int blockSize = 1;
    int buffersCount = 1;
    ALuint bufferId = 0;
    if (!streamed) {
        for (auto it = mBuffers.constBegin(); it != mBuffers.constEnd(); ++it) {
            if ((*it).fileName == fileName) {
                bufferId = (*it).ID;
            }
        }
        blockSize = ov_pcm_total(mOggVorbisFile, -1) * 4;
    } else {
        blockSize = BUFFER_BLOCK_SIZE;
        buffersCount  = BUFFERS_COUNT;
        alSourcei(mSourceId, AL_LOOPING, AL_FALSE);
    }

    mVorbisComment = ov_comment(mOggVorbisFile, -1);
    mVorbisInfo = ov_info(mOggVorbisFile, -1);

    TSoundInfo buffer;
    buffer.rate = mVorbisInfo->rate;
    buffer.fileName = fileName;
    buffer.format = (mVorbisInfo->channels == 1)
                    ? AL_FORMAT_MONO16
                    : AL_FORMAT_STEREO16;

    if (!streamed && bufferId) {
        alSourcei(mSourceId, AL_BUFFER, mBuffers[bufferId].ID);
        return true;
    }

    for (int i = 0; i < buffersCount; i++) {
        alGenBuffers(1, &buffer.ID);
        if (!checkALError()) {
            return false;
        }
        mBuffers[buffer.ID] = buffer;

        ReadOggBlock(buffer.ID, blockSize);
        if (!checkALError()) {
            return false;
        }

        if (streamed) {
            alSourceQueueBuffers(mSourceId, 1, &buffer.ID);
            if (!checkALError()) {
                return false;
            }
        } else {
            alSourcei(mSourceId, AL_BUFFER, buffer.ID);
        }
    }
    return true;
}
#endif
//------------------------------------------------------------------------------
bool USound::openFile(const QString& fileName, bool looped, bool streamed)
{
    if (!QFile::exists(fileName)) {
        qDebug() << Q_FUNC_INFO << ":: sound file not exists:" << fileName;
        return false;
    }

    mLooped = looped;
    mStreamed = streamed;

    alGenSources(1, &mSourceId);
    if (!checkALError()) {
        return false;
    }

    ALfloat listenerPosition[] = {0.0, 0.0, 0.0};
    ALfloat listenerVelocity[] = {0.0, 0.0, 0.0};

    alSourcef(mSourceId, AL_PITCH, 1.0f);
    alSourcef(mSourceId, AL_GAIN, 1.0f);
    alSourcefv(mSourceId, AL_POSITION, listenerPosition);
    alSourcefv(mSourceId, AL_VELOCITY, listenerVelocity);
    alSourcei(mSourceId, AL_LOOPING, mLooped);

    QString fileExt = fileName.right(4).toLower();
#ifdef USE_ALUT
    if (fileExt == ".wav") {
        return loadWavFile(fileName);
    }
#endif
#ifdef USE_OGGVORBIS
    if (fileExt == ".ogg") {
        return loadOggFile(fileName, streamed);
    }
#endif
    qDebug() << Q_FUNC_INFO << ":: unsupported format" << fileExt;
    return false;
}
//------------------------------------------------------------------------------
void USound::play()
{
    if (!isValid()) {
        return;
    }
    alSourcePlay(mSourceId);
}
//------------------------------------------------------------------------------
void USound::pause()
{
    if (!isValid()) {
        return;
    }
    alSourcePause(mSourceId);
}
//------------------------------------------------------------------------------
void USound::stop()
{
    if (!isValid()) {
        return;
    }
    alSourceStop(mSourceId);
}
//------------------------------------------------------------------------------
void USound::close()
{
    if (!isValid()) {
        return;
    }
    alSourceStop(mSourceId);
    if (alIsSource(mSourceId)) {
        alDeleteSources(1, &mSourceId);
    }
#ifdef USE_OGGVORBIS
    if (!mOggVorbisFile) {
        ov_clear(mOggVorbisFile);
        delete mOggVorbisFile;
    }
#endif
}
//------------------------------------------------------------------------------
void USound::update()
{
#ifdef USE_OGGVORBIS
    if (!mStreamed) {
        return;
    }

    int processed = 0;
    ALuint bufferId;

    alGetSourcei(mSourceId, AL_BUFFERS_PROCESSED, &processed);
    while (processed--) {
        alSourceUnqueueBuffers(mSourceId, 1, &bufferId);
        if (!checkALError()) {
            return;
        }

        if (ReadOggBlock(bufferId, BUFFER_BLOCK_SIZE) != 0) {
            alSourceQueueBuffers(mSourceId, 1, &bufferId);
            if (!checkALError()) {
                return;
            }
            continue;
        }

        ov_pcm_seek(mOggVorbisFile, 0);
        alSourceQueueBuffers(mSourceId, 1, &bufferId);
        if (!checkALError()) {
            return;
        }

        if (!mLooped) {
            stop();
        }
    }
#endif
}
//------------------------------------------------------------------------------
void USound::move(qreal x, qreal y, qreal z)
{
    ALfloat pos[3] = { (float)x, (float)y, (float)z };
    alSourcefv(mSourceId, AL_POSITION, pos);
}
//------------------------------------------------------------------------------
bool USound::checkSourceState(ALint state) const
{
    ALint currentState;
    alGetSourcei(mSourceId, AL_SOURCE_STATE, &currentState);
    return currentState == state;
}
//------------------------------------------------------------------------------
bool USound::isPlaying() const
{
    return checkSourceState(AL_PLAYING);
}
//------------------------------------------------------------------------------
bool USound::isPaused() const
{
    return checkSourceState(AL_PAUSED);
}
//------------------------------------------------------------------------------
bool USound::isStoped() const
{
    return checkSourceState(AL_STOPPED);
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
