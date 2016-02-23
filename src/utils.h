#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <cstdarg>
#include <QtEndian>
#include <QString>
#include <QList>

class Utils
{
public:
    static QString parseIP(QByteArray &ip_addr);
    static QByteArray pack(const char * fmt, ...);
    static unsigned int unpack(const char * fmt,const QByteArray &buf, ...);
    static unsigned int unpack(const char * fmt, char * data, ...);
};

#endif // UTILS_H
