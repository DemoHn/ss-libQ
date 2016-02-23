#ifndef SHADOWSOCKS_H
#define SHADOWSOCKS_H

#include "protocol/ab_protocol.h"
#include <QJsonDocument>
#include <QJsonObject>
class Shadowsocks : public AbstractProtocol
{
public:
    Shadowsocks(
            const QString &local_address,
            const QString &server_address,
            const quint16 local_port,
            const quint16 server_port,
            const int timeout,
            const QString &password,
            const QString &method
            );

    Shadowsocks(const QJsonObject &config);

protected:
    QHostAddress getLocalAddress();
    QHostAddress getServerAddress();
    quint16 getLocalPort();
    quint16 getServerPort();

    QByteArray pack(const QByteArray &ip_address, const quint16 port, const QByteArray &in);
    AbstractProtocol::Package  unpack(const QByteArray &in);

    QByteArray encrypt(QByteArray &data);
    QByteArray decrypt(QByteArray &data);

    QString getProtocolName();
private:
    QString local_address;
    quint16 local_port;
    QString server_address;
    quint16 server_port;
    int timeout;
    QString password;
    QString method;
};

#endif // SHADOWSOCKS_H
