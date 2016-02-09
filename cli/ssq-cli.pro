QT -= gui
QT += core network

TARGET = ssq-cli

TEMPLATE = app

HEADERS += client.h \
           config.h \
           getoption.h \
           main.h

SOURCES +=  client.cpp \
            config.cpp \
            getoption.cpp \
            main.cpp
