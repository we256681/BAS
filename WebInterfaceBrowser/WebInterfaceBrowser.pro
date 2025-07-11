TEMPLATE = app
CONFIG += windows
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp \
    convertencoding.cpp \
    filesystem.cpp \
    fileutils.cpp \
    handler.cpp \
    TrayIcon.cpp \
    iconlistener.cpp

QMAKE_LFLAGS += /LARGEADDRESSAWARE

INCLUDEPATH += $(BAS_PATH_WORKER)/include

LIBS += -L$(BAS_PATH_WORKER)/lib -llibcef -llibcef_dll_wrapper -lAdvapi32 -luser32 -lPsapi -lshell32 -lDbgHelp -lgdi32 -llibiconv

QMAKE_CXXFLAGS_RELEASE += /MT

QMAKE_CXXFLAGS_DEBUG += /MTd

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

HEADERS += \
    app.h \
    convertencoding.h \
    filesystem.h \
    fileutils.h \
    handler.h \
    TrayIcon.h \
    iconlistener.h \
    apprender.h \
    utf8.h \
    utf8/checked.h \
    utf8/core.h \
    utf8/unchecked.h
