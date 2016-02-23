#include "shadowsocks.h"

Shadowsocks::Shadowsocks(
        const QString &local_address,
        const QString &server_address,
        const quint16 local_port,
        const quint16 server_port,
        const int timeout,
        const QString &password,
        const QString &method):
        local_address(local_address),
        local_port(local_port),
        server_port(server_port),
        server_address(server_address),
        timeout(timeout),
        password(password),
        method(method)
{

}

Shadowsocks::Shadowsocks(const QJsonObject &config) :
    timeout(100),
    local_port(1080),
    local_address("127.0.0.1")
{
    if(!config["server"].isNull())
    {
        server_address = config.value("server").toString();
    }

    if(!config["server_port"].isNull())
    {
        server_port = static_cast<quint16>(config.value("server_port").toInt());
    }

    if(!config["local_address"].isNull())
    {
        local_address = config.value("local_address").toString();
    }

    if(!config["local_port"].isNull())
    {
        local_port = static_cast<quint16>(config.value("local_port").toInt());
    }

    if(!config["method"].isNull())
    {
        method = config.value("method").toString();
    }

    if(!config["timeout"].isNull())
    {
        timeout = config.value("timeout").toInt();
    }

    if(!config["password"].isNull())
    {
        password = config.value("password").toString();
    }
}

// inherited methods
QHostAddress Shadowsocks::getLocalAddress()
{
    QHostAddress local_addr(local_address);
    return local_addr;
}

QHostAddress Shadowsocks::getServerAddress()
{
    QHostAddress server_addr(server_address);
    return server_addr;
}

quint16 Shadowsocks::getLocalPort()
{
    return local_port;
}

quint16 Shadowsocks::getServerPort()
{
    return server_port;
}

QString Shadowsocks::getProtocolName()
{
    return "shadowsocks";
}

QByteArray Shadowsocks::pack(const QByteArray &ip_address, const quint16 port, const QByteArray &in)
{
    QByteArray a;
    return a;
}

AbstractProtocol::Package Shadowsocks::unpack(const QByteArray &in)
{
    Package p;
    return p;
}

QByteArray Shadowsocks::encrypt(QByteArray &data)
{
    QByteArray buf(data);
    return buf;
}

QByteArray Shadowsocks::decrypt(QByteArray &data)
{
    QByteArray buf(data);
    return buf;
}




