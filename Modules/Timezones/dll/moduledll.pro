QT       -= gui
TARGET = moduledll
TEMPLATE = lib
SOURCES += moduledll.cpp \
    data-pool.c \
    maxminddb.c \
    regionName.c \
    timeZone.c \
    timezoneoffset.c \
    tmap.cpp \
    countrytolanguage.cpp
HEADERS += moduledll.h \
    data-pool.h \
    maxminddb-compat-util.h \
    maxminddb.h \
    maxminddb_config.h \
    tmap.h \
    countrytolanguage.h

LIBS += -lWs2_32

