QT -= gui
QT += websockets

CONFIG += c++11 json
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    taskmanager.cpp \
    apiexternal.cpp \
    task.cpp \
    taskpersist.cpp \
    logger.cpp \
    apiscript.cpp \
    stream.cpp \
    schedulepredict.cpp \
    manualactions.cpp \
    runmanager.cpp \
    webapplication.cpp \
    schedulerinstaller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(httpserver/httpserver.pri)

HEADERS += \
    taskmanager.h \
    apiexternal.h \
    task.h \
    taskpersist.h \
    logger.h \
    apiscript.h \
    stream.h \
    schedulepredict.h \
    manualactions.h \
    runmanager.h \
    webapplication.h \
    schedulerinstaller.h



win32:LIBS += -lAdvapi32


QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
