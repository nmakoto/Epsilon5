#pragma once
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QTime>
#include "../Epsilon5-Proto/Epsilon5.pb.h"
#include "../Epsilon5-Proto/defines.h"
//------------------------------------------------------------------------------
// 500 ms for showing message
const quint16 DEFAULT_SERVER_TIMEOUT_MESSAGE = 500;
// 4 sec for auto-disconnect
const quint16 DEFAULT_SERVER_TIMEOUT = 4000;
//------------------------------------------------------------------------------
class TApplication;
//------------------------------------------------------------------------------
class TNetwork : public QObject
{
    Q_OBJECT
public:
    explicit TNetwork(TApplication* parent = 0);
    void Init();
    inline size_t GetId() {
        return Id;
    }
    inline bool IsServerAlive() {
        return LastPacketReceived.elapsed() < DEFAULT_SERVER_TIMEOUT_MESSAGE;
    }

public slots:
    void Start();
    void Stop();

signals:
    void WorldReceived();
    void LoadMap(QString map);
    void Disconnected();

private slots:
    void OnDataReceived();
    void OnError(QAbstractSocket::SocketError socketError);
    void OnConnected();

private:
    void SendControls(size_t packetnumber);
    void SendPlayerAuth();
    void Send(const QByteArray& data, EPacketType packetType);

private:
    void timerEvent(QTimerEvent* event);

private:
    TApplication* Application;
    QUdpSocket* Socket;
    Epsilon5::World* CurrentWorld;
    size_t Id;
    EPlayerStatus Status;
    QTime LastPacketReceived;
};
//------------------------------------------------------------------------------
