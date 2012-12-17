#include <QDebug>
#include <QStringList>
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
    qDebug() << Q_FUNC_INFO << ":: AL Error:" << alGetString(errorCode);
    return false;
}
//------------------------------------------------------------------------------
bool checkALCError(ALCdevice* device)
{
    ALenum errorCode = alcGetError(device);
    if (errorCode == AL_NO_ERROR) {
        return true;
    }
    qDebug() << Q_FUNC_INFO << ":: ALC Error:" << alcGetString(device, errorCode);
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
    qDebug() << Q_FUNC_INFO << ":: ALUT Error:" << alutGetErrorString(errorCode);
    return false;
}
#endif
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
{
}
//------------------------------------------------------------------------------
USound::~USound()
{
    for( auto it = mSources.begin(); it != mSources.end(); ++it)
        alDeleteBuffers(1, &((*it).sourceId));
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
bool USound::loadWavFile(const QString& fileName, const QString& name)
{
    if (!isValid()) {
        qDebug() << Q_FUNC_INFO << ":: sound object not valid";
        return false;
    }

    ALuint sourceId = mSources[name].sourceId;
    TBufferInfo buffer;
    buffer.sourceId = sourceId;
    buffer.fileName = fileName.trimmed();

    // Find buffer with already loaded file (with same filename)
    for (auto it = mBuffers.constBegin(); it != mBuffers.constEnd(); ++it) {
        if ((*it).fileName == buffer.fileName) {
            buffer = mBuffers[(*it).bufferId];
            alSourcei(sourceId, AL_BUFFER, buffer.bufferId);
            mSources[name].bufferId = buffer.bufferId;
            return true;
        }
    }

    ALint id = alutCreateBufferFromFile(qPrintable(buffer.fileName));
    if (!checkALUTError() || id == AL_NONE) {
        return false;
    }

    buffer.bufferId = id;
    mSources[name].bufferId = id;
    mBuffers[buffer.bufferId] = buffer;
    alSourcei(sourceId, AL_BUFFER, buffer.bufferId);
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
bool USound::loadOggFile(const QString& fileName, const QString& name, bool streamed)
{
    if (!isValid()) {
        qDebug() << Q_FUNC_INFO << ":: sound object not valid";
        return false;
    }

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
    ALuint sourceId = mSources[name].sourceId;
    if (!streamed) {
        for (auto it = mBuffers.constBegin(); it != mBuffers.constEnd(); ++it) {
            if ((*it).fileName == fileName) {
                bufferId = (*it).bufferId;
                break;
            }
        }
        blockSize = ov_pcm_total(mOggVorbisFile, -1) * 4;
    } else {
        blockSize = BUFFER_BLOCK_SIZE;
        buffersCount  = BUFFERS_COUNT;
        alSourcei(sourceId, AL_LOOPING, AL_FALSE);
    }

    mVorbisComment = ov_comment(mOggVorbisFile, -1);
    mVorbisInfo = ov_info(mOggVorbisFile, -1);

    TBufferInfo buffer;
    buffer.sourceId = sourceId;
    buffer.rate = mVorbisInfo->rate;
    buffer.fileName = fileName;
    buffer.format = (mVorbisInfo->channels == 1)
                    ? AL_FORMAT_MONO16
                    : AL_FORMAT_STEREO16;

    if (!streamed && bufferId) {
        alSourcei(sourceId, AL_BUFFER, mBuffers[bufferId].bufferId);
        return true;
    }

    for (int i = 0; i < buffersCount; i++) {
        alGenBuffers(1, &buffer.bufferId);
        if (!checkALError()) {
            return false;
        }
        mBuffers[buffer.bufferId] = buffer;

        ReadOggBlock(buffer.bufferId, blockSize);
        if (!checkALError()) {
            return false;
        }

        if (streamed) {
            alSourceQueueBuffers(sourceId, 1, &buffer.bufferId);
            if (!checkALError()) {
                return false;
            }
        } else {
            alSourcei(sourceId, AL_BUFFER, buffer.bufferId);
        }
    }
    return true;
}
#endif
//------------------------------------------------------------------------------
bool USound::openFile(const QString& fileName, const QString& name,
        bool looped, bool streamed)
{
    if (!QFile::exists(fileName)) {
        qDebug() << Q_FUNC_INFO << ":: sound file not exists:" << fileName;
        return false;
    }

    TSourceInfo sourceInfo;
    if( mSources.contains(name) ) {
        sourceInfo = mSources[name];
        close(name);
    } else {
        alGenSources(1, &sourceInfo.sourceId);
        if (!checkALError()) {
            return false;
        }
    }

    sourceInfo.looped = looped;
    sourceInfo.streamed = streamed;
    sourceInfo.name = name;

    alSourcef(sourceInfo.sourceId, AL_PITCH, sourceInfo.pitch);
    alSourcef(sourceInfo.sourceId, AL_GAIN, sourceInfo.gain);
    alSourcefv(sourceInfo.sourceId, AL_POSITION, sourceInfo.position);
    alSourcefv(sourceInfo.sourceId, AL_VELOCITY, sourceInfo.velocity);
    alSourcei(sourceInfo.sourceId, AL_LOOPING, sourceInfo.looped);

    mSources[name] = sourceInfo;

    QString fileExt = fileName.right(4).toLower();
#ifdef USE_ALUT
    if (fileExt == ".wav") {
        return loadWavFile(fileName, name);
    }
#endif
#ifdef USE_OGGVORBIS
    if (fileExt == ".ogg") {
        return loadOggFile(fileName, name, streamed);
    }
#endif
    qDebug() << Q_FUNC_INFO << ":: unsupported format" << fileExt;
    return false;
}
//------------------------------------------------------------------------------
void USound::play(const QString& name)
{
    if (!checkForValidName(name))
        return;
    alSourcePlay(mSources[name].sourceId);
}
//------------------------------------------------------------------------------
void USound::pause(const QString& name)
{
    if (!checkForValidName(name))
        return;
    alSourcePause(mSources[name].sourceId);
}
//------------------------------------------------------------------------------
void USound::stop( const QString& name)
{
    if (!checkForValidName(name))
        return;
    alSourceStop(mSources[name].sourceId);
}
//------------------------------------------------------------------------------
void USound::close(const QString& name)
{
    if (!checkForValidName(name))
        return;

    ALuint sourceId = mSources[name].sourceId;
    alSourceStop(sourceId);
    if (alIsSource(sourceId)) {
        alDeleteSources(1, &sourceId);
    }
    mSources.remove(name);
#ifdef USE_OGGVORBIS
    if (!mOggVorbisFile) {
        ov_clear(mOggVorbisFile);
        delete mOggVorbisFile;
    }
#endif
}
//------------------------------------------------------------------------------
void USound::update(const QString& name)
{
#ifdef USE_OGGVORBIS
    if (!checkForValidName(name))
        return;

    int processed = 0;
    ALuint bufferId;
    ALuint sourceId = mSources[name].sourceId;

    alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &processed);
    while (processed--) {
        alSourceUnqueueBuffers(sourceId, 1, &bufferId);
        if (!checkALError()) {
            return;
        }

        if (ReadOggBlock(bufferId, BUFFER_BLOCK_SIZE) != 0) {
            alSourceQueueBuffers(sourceId, 1, &bufferId);
            if (!checkALError()) {
                return;
            }
            continue;
        }

        ov_pcm_seek(mOggVorbisFile, 0);
        alSourceQueueBuffers(sourceId, 1, &bufferId);
        if (!checkALError()) {
            return;
        }

        if (!mSources[name].looped) {
            stop(name);
        }
    }
#endif
}
//------------------------------------------------------------------------------
void USound::setPosition(qreal x, qreal y, qreal z, const QString& name)
{
    if (!checkForValidName(name))
        return;
    ALfloat pos[3] = { (float)x, (float)y, (float)z };
    alSourcefv(mSources[name].sourceId, AL_POSITION, pos);
}
//------------------------------------------------------------------------------
void USound::setVelocity(qreal x, qreal y, qreal z, const QString& name)
{
    if (!checkForValidName(name))
        return;
    ALfloat pos[3] = { (float)x, (float)y, (float)z };
    alSourcefv(mSources[name].sourceId, AL_VELOCITY, pos);
}
//------------------------------------------------------------------------------
void USound::position(qreal& x, qreal& y, qreal& z, const QString& name)
{
    if (!checkForValidName(name))
        return;
    x = mSources[name].position[0];
    y = mSources[name].position[1];
    z = mSources[name].position[2];
}
//------------------------------------------------------------------------------
void USound::velocity(qreal& x, qreal& y, qreal& z, const QString& name)
{
    if (!checkForValidName(name))
        return;
    x = mSources[name].velocity[0];
    y = mSources[name].velocity[1];
    z = mSources[name].velocity[2];
}
//------------------------------------------------------------------------------
bool USound::checkSourceState(ALuint sourceId, ALint state) const
{
    ALint currentState;
    alGetSourcei(sourceId, AL_SOURCE_STATE, &currentState);
    return currentState == state;
}
//------------------------------------------------------------------------------
bool USound::isPlaying(const QString& name) const
{
    if (!checkForValidName(name))
        return false;
    return checkSourceState(mSources[name].sourceId, AL_PLAYING);
}
//------------------------------------------------------------------------------
bool USound::isPaused(const QString& name) const
{
    if (!checkForValidName(name))
        return false;
    return checkSourceState(mSources[name].sourceId, AL_PAUSED);
}
//------------------------------------------------------------------------------
bool USound::isStoped(const QString& name) const
{
    if (!checkForValidName(name))
        return false;
    return checkSourceState(mSources[name].sourceId, AL_STOPPED);
}
//------------------------------------------------------------------------------
QStringList USound::keys()
{
    return QStringList(mSources.keys());
}
//------------------------------------------------------------------------------
bool USound::checkForValidName(const QString &name) const
{
    if( name.isEmpty() ) {
        return false;
    }
    if (!isValid()) {
        qDebug() << Q_FUNC_INFO << ":: not valid sound object";
        return false;
    }
    if( !mSources.contains(name) ) {
        qDebug() << Q_FUNC_INFO << ":: sound source with name" << name
                << "not found";
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
TSourceInfo USound::sourceInfo(const QString& name) const
{
    if( !checkForValidName(name) )
        return TSourceInfo();
    return mSources[name];
}
//------------------------------------------------------------------------------
