#pragma once
#include <QObject>
#include <QPoint>
#include <QSize>
#include "../utils/usettings.h"
//------------------------------------------------------------------------------
class TSettings: public QObject
{
    Q_OBJECT
public:
    explicit TSettings(QObject* parent);
    ~TSettings();

    QString GetNickname();
    QString GetServerAddr();
    quint16 GetServerPort();
    bool GetWindowFullscreen();
    int GetWindowPosX() const;
    int GetWindowPosY() const;
    int GetWindowWidth() const;
    int GetWindowHeight() const;
    QPoint GetWindowPos() const;
    QSize GetWindowSize() const;

    void SetWindowFullscreen(bool value = true);
    void SetWindowPosX(int value);
    void SetWindowPosY(int value);
    void SetWindowWidth(int value);
    void SetWindowHeight(int value);

    void Save(bool keepOrigin = true);
    void Load();

private:
    utils::USettings* Settings;
};
//------------------------------------------------------------------------------
