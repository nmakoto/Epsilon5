#include <QDebug>
#include <QtGlobal>
#include <QDesktopWidget>

#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
#include <X11/extensions/Xrandr.h>
#endif

#ifdef Q_OS_WIN32
#include <Windows.h>
DEVMODE devMode;
#endif

#include "../utils/ufullscreenwrapper.h"
//------------------------------------------------------------------------------
using namespace utils;
//------------------------------------------------------------------------------
#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
Rotation originRotation = RR_Rotate_0;
short originSizeId = 0;
#endif
//------------------------------------------------------------------------------
UFullscreenWrapper::UFullscreenWrapper(QWidget* parent)
    : mParent(parent)
{
#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
    Display* dpy = XOpenDisplay(NULL);
    int event_base;
    int error_base;
    int major_version;
    int minor_version;
    mExtensionFound = XRRQueryExtension(dpy, &event_base, &error_base) &&
            XRRQueryVersion(dpy, &major_version, &minor_version);
#endif
}
//------------------------------------------------------------------------------
UFullscreenWrapper::DisplayModes UFullscreenWrapper::enumModes()
{
    DisplayModes displayModes;
#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
    if (mExtensionFound) {
        int num_modes;
        Display* dpy = XOpenDisplay(NULL);
        XRRScreenSize* xrrs = XRRSizes(dpy, 0, &num_modes);

        for (int i = 0; i < num_modes; ++i) {
            displayModes.append(DisplayMode(xrrs[i].width, xrrs[i].height));
        }
    }
#endif
    return displayModes;
}
//------------------------------------------------------------------------------
bool UFullscreenWrapper::changeToMode(int width, int height)
{
    return changeToMode(DisplayMode(width, height));
}
//------------------------------------------------------------------------------
#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
int UFullscreenWrapper::findModeId(int width, int height)
{
    const DisplayModes& dms = enumModes();
    for (int i = 0; i < dms.count(); ++i) {
        if (dms.at(i).width() == width && dms.at(i).height() == height) {
            return i;
        }
    }
    return -1;
}
#endif
//------------------------------------------------------------------------------
bool UFullscreenWrapper::changeToMode(const DisplayMode& mode)
{
#ifdef Q_OS_WIN32
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
    devMode.dmPelsWidth = (unsigned long) mode.width();
    devMode.dmPelsHeight = (unsigned long) mode.height();
    if (ChangeDisplaySettings(&devMode, CDS_TEST) != DISP_CHANGE_SUCCESSFUL) {
        return false;
    }
    if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        return false;
    }
#endif
#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
    if (mExtensionFound) {
        Display* dpy = XOpenDisplay(NULL);
        Window root = RootWindow(dpy, 0);
        XRRScreenConfiguration* conf = XRRGetScreenInfo(dpy, root);
        originSizeId = XRRConfigCurrentConfiguration(conf, &originRotation);

        int sizeId = findModeId(mode.width(), mode.height());
        if (sizeId < 0) {
            return false;
        }


        if (BadValue == XRRSetScreenConfig(
                    dpy, conf, root, sizeId, RR_Rotate_0, CurrentTime)
           ) {
            return false;
        }
    }
#endif
    if (!mParent) {
        return true;
    }

    mParent->setWindowState(mParent->windowState() | Qt::WindowFullScreen);
    mParent->setFixedSize(mode.width(), mode.height());
    return true;
}
//------------------------------------------------------------------------------
bool UFullscreenWrapper::restoreMode()
{
#ifdef Q_OS_WIN32
    if (ChangeDisplaySettings(NULL, 0) != DISP_CHANGE_SUCCESSFUL) {
        return false;
    }
#endif
#if defined(Q_OS_UNIX) && defined(USE_XRANDR)
    if (mExtensionFound) {
        Display* dpy = XOpenDisplay(NULL);
        Window root = RootWindow(dpy, 0);
        XRRScreenConfiguration* conf = XRRGetScreenInfo(dpy, root);
        if (BadValue == XRRSetScreenConfig(
                    dpy, conf, root, originSizeId, originRotation, CurrentTime)
           ) {
            return false;
        }
    }
#endif
    if (!mParent) {
        return true;
    }

    mParent->setWindowState(mParent->windowState() & ~Qt::WindowFullScreen);
    mParent->setFixedSize(mParent->baseSize());
    return true;
}
//------------------------------------------------------------------------------
