#ifndef AB_PROTOCOL_H
#define AB_PROTOCOL_H

/* Protocol Abstract Class
 *
 * This abstract class rules an interface for every protocol (including SS, SSR, etc.)
 *
 * This interface class contains configurations, pack & unpack methods and bytes to send / recv.
 *
 * Nigshoxiz
 * 2016.2.9
*/

#include <QJsonDocument>

class AbstractProtocol {
    struct Package{
        qint8 ip_protocol; // IP protocol (IPV4 = 4, IPV6 = 6)
        QByteArray ip_address; // IP address (IPV4 or IPV6 ?)
        quint16 port;
        QByteArray content; // package content (decrypted)
    };

public:
    virtual QByteArray pack(const QByteArray &ip_address,const uint16 port,const QByteArray &in) = 0;
    virtual Package unpack(const QByteArray &in) = 0;
    virtual void setProfile(const QJsonDocument &profile) = 0;
    virtual QJsonDocument & getProfile() = 0;
    virtual QString getProtocol() = 0;
    virtual void setProtocol(const QString protocol_string) = 0;

    virtual bool init() = 0;
};

#endif // PROTOCOL_H

