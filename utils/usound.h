#pragma once
#include <QObject>
#include <QVector>
#include <QHash>
#include <AL/alut.h>
#ifdef USE_OGGVORBIS
#include <QFile>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#endif
//------------------------------------------------------------------------------
namespace utils
{
//------------------------------------------------------------------------------
class USoundContainer;
//------------------------------------------------------------------------------
struct TSoundInfo {
    ALuint ID;
    ALsizei rate;
    ALenum format;
    QString fileName;
};
//------------------------------------------------------------------------------
class USound : public QObject
{
    Q_OBJECT
    typedef QHash<ALuint, TSoundInfo> TSoundInfoHash;
public:
    explicit USound(QObject* parent = 0);
    USound(const USound& sound);
    ~USound();
    USound& operator=(const USound& sound);

    void init(ALCdevice* device, ALCcontext* context);
    bool isValid() const;

    bool openFile(const QString& fileName, bool looped = false, bool streamed = false);
    void play();
    void pause();
    void stop();
    void close();
    void update();
    void move(qreal x, qreal y, qreal z = 0.0);
    bool isPlaying() const;
    bool isPaused() const;
    bool isStoped() const;

private:
    bool loadWavFile(const QString& fileName);
#ifdef USE_OGGVORBIS
    bool ReadOggBlock(ALuint bufferId, size_t size);
    bool loadOggFile(const QString& fileName, bool streamed = false);
#endif
    bool checkSourceState(ALint state) const;

private:
    TSoundInfoHash mBuffers;
    ALCdevice* mDevice;
    ALCcontext* mContext;
    ALuint mSourceId;
#ifdef USE_OGGVORBIS
    OggVorbis_File* mOggVorbisFile;
    vorbis_comment* mVorbisComment;
    vorbis_info* mVorbisInfo;
    QFile mOggFile;
#endif
    bool mLooped;
    bool mStreamed;
};
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
