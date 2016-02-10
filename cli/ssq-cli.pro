QT -= gui
QT += core network

TARGET = ssq-cli

TEMPLATE = app

DEFINES += SSQ_LIB_STATIC

HEADERS += client.h \
           config.h \
           getoption.h \
           main.h

SOURCES +=  client.cpp \
            config.cpp \
            getoption.cpp \
            main.cpp

INCLUDEPATH += $$PWD/../src

LIBS += -L$$PWD/../build \
        -lssq

win32 :{
    DESTDIR = $$PWD/../build
}
