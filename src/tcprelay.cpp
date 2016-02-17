#include "tcprelay.h"
#include <cstdlib>
#include <QtEndian>
TCPRelay::TCPRelay(AbstractProtocol *p,
                   const bool _is_local,
                   QObject *parent) :
                    QObject(parent),
                    is_local(_is_local),
                    protocol(p),
                    stage(INIT)
{
    local  = new QTcpSocket(this);
    remote = new QTcpSocket(this);
    server = new QTcpServer(this);

    if(protocol)
    {
        //protocol params setting
        local_address  = protocol->getLocalAddress();
        local_port     = protocol->getLocalPort();

        // remote <--> proxy server
        remote_port    = protocol->getServerPort();
        remote_address = protocol->getServerAddress();
    }

    //bind events
    connect(server, SIGNAL(newConnection()), this, SLOT(handleLocalConnection()));
    //remote socket events
    connect(remote, SIGNAL(readyRead()),this, SLOT(onRemoteRead()));
}

TCPRelay::~TCPRelay()
{
    // close socket
    TCPRelay::close();

    server->deleteLater();
    local->deleteLater();
    remote->deleteLater();
}

bool TCPRelay::listen()
{
    bool r = server->listen(local_address, local_port);
    return r;
}

void TCPRelay::close()
{
    if(stage == DESTROYED)
    {
        return ;
    }

    local->close();
    remote->close();
    stage = DESTROYED;
}

void TCPRelay::handleLocalConnection()
{
    local = server->nextPendingConnection();

    //local socket events
    connect(local, SIGNAL(readyRead()), this, SLOT(onLocalRead()));
}

// on local read
void TCPRelay::onLocalRead()
{
    QTcpSocket *sock = qobject_cast<QTcpSocket *>(sender());
    // just debug
    QByteArray res = sock->readAll();
    qDebug() << "length: " << res.length() << " ";
    qDebug() << res;

    const static char _accept [] = {5 ,0};
    const static char _reject [] = {0, 91};

    const static QByteArray accept(_accept,2);
    const static QByteArray reject(_reject,2);
    if(res.length() == 0)
    {
        qDebug() << "empty request data , bye =w=";
    }

    if(!is_local && stage == INIT)
    {
        stage = ADDR;
    }else if(stage == INIT) {
        // SOCKS5 Protocol
        /*
         * 01. REQUEST VERSION IDF FROM CLIENT:
         * +----+----------+----------+
           |VER | NMETHODS | METHODS  |
           +----+----------+----------+
           | 1  |    1     | 1 to 255 |
           +----+----------+----------+
         *
         * - RESPONSE
         * +----+--------+
           |VER | METHOD |
           +----+--------+
           | 1  |   1    |
           +----+--------+
         */

        char _ver = res[0];

        //if not socks5
        if(_ver != SOCKS5_VER){
            qDebug() << "not socks5 protocol!";
            sock->write(reject);
            sock->close();
        }else{
            sock->write(accept);
            stage = ADDR;
        }
    }

    if(stage == ADDR)
    {
        handleStageAddr(res);
    }


//    sock->close();
}

void TCPRelay::handleStageAddr(QByteArray &data)
{
    if(is_local)
    {
        int cmd = static_cast<int>(data[1]);

        if(cmd == 3)
        {
            static const char _header [] = {5, 0, 0};
            static const QByteArray header(_header,3);

            QHostAddress addr = local->localAddress();
            quint16 port      = local->localPort();

            QByteArray addr_bin, port_ns;
            char type_c;
            port_ns.resize(2);
            // to byte array
            qToBigEndian(port, reinterpret_cast<uchar*>(port_ns.data()));

            if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
                quint32 ipv4_addr = qToBigEndian(addr.toIPv4Address());
                addr_bin = QByteArray(reinterpret_cast<char*>(&ipv4_addr), 4);
                type_c = 1;
            }else{
                type_c = 4;
                Q_IPV6ADDR ipv6_addr = addr.toIPv6Address();
                addr_bin = QByteArray(reinterpret_cast<char*>(ipv6_addr.c), 16);
            }
            QByteArray b;
            b = header + type_c + addr_bin + port_ns;
            qDebug() << b;
            local->write(header + type_c + addr_bin + port_ns);
            stage = UDP_ASSOC;
            return ;
        }else if(cmd == 1){
            data = data.mid(3);
        }else{
            qDebug() << "unknown command " + QString::number(cmd);
            close();
            return ;
        }
    }

    // parse header
    int header_length = 0;
    stage = DNS;
    if(is_local){
        static const char _response [] = { 5 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 16 , 16 };
        static const QByteArray response(_response, 10);

        local->write(response);
    }
}

void TCPRelay::parseSocksHeader(const QByteArray &data, QString &dest_addr, quint16 port, int &header_length)
{
    char atyp = data[0];
    int addrtype = static_cast<int>(atyp & 0x0f);
    header_length = 0;

    // address type
    // HOST
    if(addrtype == 3){
        if(data.length() > 2){
            quint8 addrlen = static_cast<quint8>(data[1]);
            if(data.size() >= 2 + addrlen){
                port = qFromBigEndian(*reinterpret_cast<const quint16 *>(data.data() + 2 + addrlen));

                dest_addr = (data.mid(2, addrlen));
            }
        }
    }else if(addrtype == 1){
        if(data.length() >= 7)
        {
            dest_addr =  (qFromBigEndian(*reinterpret_cast<const quint32 *> (data.data() + 1)));
            if(!dest_addr.isNull())
            {
                header_length = 7;
                port = qFromBigEndian(*reinterpret_cast<const quint16 *>(data.data() + 5));
            }

        }
    // IPV6 TODO
    }else if(addrtype == 4){

    }
}

void TCPRelay::onRemoteRead()
{
    qDebug() << "wtf remote";
}
