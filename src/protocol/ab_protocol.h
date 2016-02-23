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
#include <QHostAddress>

class AbstractProtocol {

public:
    struct Package{
        qint8 ip_protocol; // IP protocol (IPV4 = 4, IPV6 = 6)
        QByteArray ip_address; // IP address (IPV4 or IPV6 ?)
        quint16 port;
        QByteArray content; // package content (decrypted)
    };
    virtual QByteArray pack(const QByteArray &ip_address,const quint16 port,const QByteArray &in) = 0;
    virtual Package unpack(const QByteArray &in) = 0;

    virtual QByteArray encrypt(QByteArray &data) = 0;
    virtual QByteArray decrypt(QByteArray &data) = 0;

    virtual QString getProtocolName() = 0;

    //params
    virtual QHostAddress getLocalAddress() = 0;
    virtual QHostAddress getServerAddress() = 0;

    virtual quint16 getLocalPort() = 0;
    virtual quint16 getServerPort() = 0;

    //virtual ~AbstractProtocol();
 //   virtual bool init() = 0;
};

#endif // PROTOCOL_H

