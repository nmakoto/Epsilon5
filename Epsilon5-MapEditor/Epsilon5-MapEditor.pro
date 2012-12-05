QT += core gui

TARGET = Epsilon5-MapEditor
TEMPLATE = app

SOURCES += \
    ../utils/uexception.cpp \
    ../utils/usettings.cpp \
    ../utils/ucast.cpp \
    main.cpp \
    mainwindow.cpp \
#    createmapdialog.cpp \
#    mapcreator.cpp \
#    graphicsview.cpp \
#    mapitem.cpp \
#    utils.cpp \
#    openmapdialog.cpp \
#    mapitemsignal.cpp \
    settings.cpp \
    application.cpp \
    ui/configurationdialog.cpp \
    ui/objectseditorform.cpp \
    imagecache.cpp \
    containers/object.cpp \
    containers/mapscontainer.cpp \
    containers/container.cpp \
    containers/objectscontainer.cpp \
    containers/respawnscontainer.cpp

HEADERS += \
    ../utils/uexception.h \
    ../utils/usettings.h \
    ../utils/ucast.h \
    mainwindow.h \
#    createmapdialog.h \
#    mapcreator.h \
#    graphicsview.h \
#    mapitem.h \
#    utils.h \
#    openmapdialog.h \
#    mapitemsignal.h \
    application.h \
    settings.h \
    global.h \
    ui/configurationdialog.h \
    ui/objectseditorform.h \
    imagecache.h \
    containers/object.h \
    containers/object.h \
    containers/mapscontainer.h \
    containers/container.h \
    containers/objectscontainer.h \
    containers/respawnscontainer.h

FORMS += \
#    createmapdialog.ui \
#    openmapdialog.ui \
    ui/configurationdialog.ui \
    ui/objectseditorform.ui

QMAKE_CXXFLAGS += -std=c++0x
# -mfpmath=sse
# -Ofast -flto -funroll-loops
