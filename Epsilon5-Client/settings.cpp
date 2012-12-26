#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "settings.h"
//------------------------------------------------------------------------------
const char* SETTINGS_FILENAME = "settings.ini";
const char* DEFAULT_PLAYER_NAME = "player";
const char* DEFAULT_SERVER_ADDRESS = "127.0.0.1";
const quint16 DEFAULT_SERVER_PORT = 14567;
const bool DEFAULT_WINDOW_FULLSCREEN = true;
const quint32 DEFAULT_WINDOW_WIDTH = 800;
const quint32 DEFAULT_WINDOW_HEIGHT = 600;
const qint32 DEFAULT_WINDOW_POSX = 100;
const qint32 DEFAULT_WINDOW_POSY = 50;
//------------------------------------------------------------------------------
TSettings::TSettings(QObject* parent)
    : QObject(parent)
    , Settings(new utils::USettings(this))
{
    // Define default parameters
    utils::USettings::TParametersHash params;
    params["nickname"] = QString(DEFAULT_PLAYER_NAME);
    params["server.address"] = QString(DEFAULT_SERVER_ADDRESS);
    params["server.port"] = QString::number(DEFAULT_SERVER_PORT);
    params["window.fullscreen"] = QString::number(DEFAULT_WINDOW_FULLSCREEN);
    params["window.posx"] = QString::number(DEFAULT_WINDOW_POSX);
    params["window.posy"] = QString::number(DEFAULT_WINDOW_POSY);
    params["window.width"] = QString::number(DEFAULT_WINDOW_WIDTH);
    params["window.height"] = QString::number(DEFAULT_WINDOW_HEIGHT);
    Settings->DefineParams(params);
    Load();
}
//------------------------------------------------------------------------------
TSettings::~TSettings()
{
    Save();
}
//------------------------------------------------------------------------------
QString TSettings::GetNickname()
{
    return Settings->GetParameter("nickname");
}
//------------------------------------------------------------------------------
QString TSettings::GetServerAddr()
{
    return Settings->GetParameter("server.address");
}
//------------------------------------------------------------------------------
quint16 TSettings::GetServerPort()
{
    return Settings->GetParameter("server.port");
}
//------------------------------------------------------------------------------
bool TSettings::GetWindowFullscreen()
{
    return Settings->GetParameter("window.fullscreen");
}
//------------------------------------------------------------------------------
void TSettings::SetWindowFullscreen(bool value)
{
    Settings->SetParameter("window.fullscreen", QString::number(value));
}
//------------------------------------------------------------------------------
int TSettings::GetWindowPosX() const
{
    return Settings->GetParameter("window.posx");
}
//------------------------------------------------------------------------------
void TSettings::SetWindowPosX(int value)
{
    Settings->SetParameter("window.posx", QString::number(value));
}
//------------------------------------------------------------------------------
int TSettings::GetWindowPosY() const
{
    return Settings->GetParameter("window.posy");
}
//------------------------------------------------------------------------------
void TSettings::SetWindowPosY(int value)
{
    Settings->SetParameter("window.posy", QString::number(value));
}
//------------------------------------------------------------------------------
int TSettings::GetWindowWidth() const
{
    return Settings->GetParameter("window.width");
}
//------------------------------------------------------------------------------
void TSettings::SetWindowWidth(int value)
{
    Settings->SetParameter("window.width", QString::number(value));
}
//------------------------------------------------------------------------------
int TSettings::GetWindowHeight() const
{
    return Settings->GetParameter("window.height");
}
//------------------------------------------------------------------------------
void TSettings::SetWindowHeight(int value)
{
    Settings->SetParameter("window.height", QString::number(value));
}
//------------------------------------------------------------------------------
QPoint TSettings::GetWindowPos() const
{
    return QPoint(GetWindowPosX(), GetWindowPosY());
}
//------------------------------------------------------------------------------
QSize TSettings::GetWindowSize() const
{
    return QSize(GetWindowWidth(), GetWindowHeight());
}
//------------------------------------------------------------------------------
void TSettings::Load()
{
    try {
        Settings->Load(SETTINGS_FILENAME);
    } catch (const UException& ex) {
        qWarning() << ex.what();
    }
}
//------------------------------------------------------------------------------
void TSettings::Save(bool keepOrigin)
{
    try {
        Settings->Save(SETTINGS_FILENAME, keepOrigin);
    } catch (const UException& ex) {
        qWarning() << ex.what();
    }
}
//------------------------------------------------------------------------------
