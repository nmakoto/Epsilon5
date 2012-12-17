#pragma once
#include <QObject>
#include <QVector>
#include <QHash>
#include <AL/al.h>
#include <AL/alc.h>

#ifdef USE_ALUT
#include <AL/alut.h>
#endif

#ifdef USE_OGGVORBIS
#include <QFile>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#endif

#include "../utils/usoundcontainer.h"
//------------------------------------------------------------------------------
namespace utils
{
//------------------------------------------------------------------------------
class USoundContainer;
//------------------------------------------------------------------------------
struct TBufferInfo {
    ALuint bufferId;
    ALuint sourceId;
    ALsizei rate;
    ALenum format;
    QString fileName;
};
//------------------------------------------------------------------------------
struct TSourceInfo {
    ALuint sourceId;
    ALuint bufferId;
    ALfloat pitch;
    ALfloat gain;
    ALfloat position[3];
    ALfloat velocity[3];
    bool looped;
    bool streamed;

    TSourceInfo()
        : sourceId(0)
        , pitch(1.0f)
        , gain(1.0f)
        , looped(false)
        , streamed(false)
    {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
        velocity[0] = 0.0f;
        velocity[1] = 0.0f;
        velocity[2] = 0.0f;
    }
};
//------------------------------------------------------------------------------
class USound : public QObject
{
    Q_OBJECT
    typedef QHash<ALuint, TBufferInfo> TBufferInfoHash;
    typedef QHash<QString, TSourceInfo> TSourceInfoHash;
public:
    explicit USound(QObject* parent = 0);
//    USound(const USound& sound);
    ~USound();
//    USound& operator=(const USound& sound);

    void init(ALCdevice* device, ALCcontext* context);
    bool isValid() const;
    QStringList keys();

    bool openFile(const QString& fileName, const QString& name,
            bool looped = false, bool streamed = false);
    void play(const QString& name);
    void pause(const QString& name);
    void stop(const QString& name);
    void close(const QString& name);
    void update(const QString& name);
    void move(qreal x, qreal y, qreal z, const QString&);
    bool isPlaying(const QString& name) const;
    bool isPaused(const QString& name) const;
    bool isStoped(const QString& name) const;

private:
#ifdef USE_ALUT
    bool loadWavFile(const QString& fileName, const QString& name);
#endif
#ifdef USE_OGGVORBIS
    bool ReadOggBlock(ALuint bufferId, size_t size);
    bool loadOggFile(const QString& fileName, const QString& name,
            bool streamed = false);
#endif
    bool checkSourceState(ALuint sourceId, ALint state) const;

private:
    TBufferInfoHash mBuffers;
    TSourceInfoHash mSources;
    ALCdevice* mDevice;
    ALCcontext* mContext;
//    ALuint mSourceId;
#ifdef USE_OGGVORBIS
    OggVorbis_File* mOggVorbisFile;
    vorbis_comment* mVorbisComment;
    vorbis_info* mVorbisInfo;
    QFile mOggFile;
#endif
//    bool mLooped;
//    bool mStreamed;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------

