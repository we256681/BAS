QT       -= gui
TARGET = moduledll
TEMPLATE = lib
SOURCES += moduledll.cpp 
HEADERS += moduledll.h


INCLUDEPATH += $(BAS_PATH)/include
LIBS += -L$(BAS_PATH)/lib

LIBS += -llibcurl
