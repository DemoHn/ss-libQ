QT -= gui
QT += core network

TARGET = ssq-cli

TEMPLATE = app

DEFINES += SSQ_LIB_STATIC

CONFIG  += console

HEADERS += client.h \
           config.h \
           getoption.h \
           main.h \
    server.h

SOURCES +=  client.cpp \
            config.cpp \
            getoption.cpp \
            main.cpp \
    server.cpp

INCLUDEPATH += $$PWD/../src

LIBS += -L$$PWD/../build \
        -lssq

win32 :{
    DESTDIR = $$PWD/../build
}
