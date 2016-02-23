QT -= gui

QT += core network

TARGET = ssq

TEMPLATE = lib

DEFINES += SSQ_LIB_STATIC

# static link
CONFIG += static

HEADERS += \
    sslibQ \
    protocol/ab_protocol.h \
    controller.h \
    export.h \
    tcprelay.h \
    udprelay.h \
    tcpserver.h \
    asyncdns.h \
    cipher/ab_cipher.h \
    protocol/shadowsocks.h \
    utils.h

SOURCES += \
    controller.cpp \
    tcprelay.cpp \
    udprelay.cpp \
    tcpserver.cpp \
    asyncdns.cpp \
    protocol/shadowsocks.cpp \
    utils.cpp

win32 :{
    DESTDIR = $$PWD/../build
}
