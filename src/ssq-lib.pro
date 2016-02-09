QT -= gui

QT += core network

TARGET = ssq-lib

TEMPLATE = lib

DEFINES += SSQ_LIB_STATIC

HEADERS += \
    protocol/ab_protocol.h \
    controller.h \
    export.h \
    tcprelay.h \
    udprelay.h \
    tcpserver.h \
    cipher/ab_cipher.h

SOURCES += \
    controller.cpp \
    tcprelay.cpp \
    udprelay.cpp \
    tcpserver.cpp

