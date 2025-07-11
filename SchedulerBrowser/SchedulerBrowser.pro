TEMPLATE = app
CONFIG += windows
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp \
    handler.cpp

QMAKE_LFLAGS += /LARGEADDRESSAWARE

INCLUDEPATH += $(BAS_PATH_WORKER)/include

LIBS += -L$(BAS_PATH_WORKER)/lib -llibcef -llibcef_dll_wrapper -lAdvapi32 -luser32 -lPsapi -lshell32 -lDbgHelp -lgdi32

QMAKE_CXXFLAGS_RELEASE += /MT

QMAKE_CXXFLAGS_DEBUG += /MTd

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

HEADERS += \
    app.h \
    handler.h
