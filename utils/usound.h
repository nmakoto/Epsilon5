#pragma once
#include <QObject>
#include <QVector>
#include <QHash>
#include <AL/alut.h>
//------------------------------------------------------------------------------
namespace utils {
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
    void stop();
    void close();
    void update();
    void move(qreal x, qreal y, qreal z = 0.0);
    bool isPlaying() const;
    bool isPaused() const;
    bool isStoped() const;

private:
    bool loadWavFile(const QString& fileName);
    bool checkSourceState(ALint state) const;

private:
    TSoundInfoHash mContainer;
    ALCdevice* mDevice;
    ALCcontext* mContext;
    ALuint mSourceId;
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
