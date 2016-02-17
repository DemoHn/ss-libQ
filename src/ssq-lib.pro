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
    buffer.h \
    cipher/ab_cipher.h \
    protocol/shadowsocks.h

SOURCES += \
    controller.cpp \
    tcprelay.cpp \
    udprelay.cpp \
    tcpserver.cpp \
    asyncdns.cpp \
    buffer.cpp \
    protocol/shadowsocks.cpp

win32 :{
    DESTDIR = $$PWD/../build
}
