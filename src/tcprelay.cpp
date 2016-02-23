#include "tcprelay.h"
#include <cstdlib>
#include "utils.h"

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
        server_port    = protocol->getServerPort();
        server_address = protocol->getServerAddress();

    }

    QHostAddress google_dns("8.8.8.8");
    DNSResolver = new AsyncDNS(google_dns);
    //bind events
    connect(server, SIGNAL(newConnection()), this, SLOT(handleLocalConnection()));
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
    if(is_local)
    {
        return server->listen(local_address, local_port);
    }else{
        return server->listen(server_address, server_port);
    }
}

void TCPRelay::close()
{
    if(stage == DESTROYED)
    {
        return ;
    }

    local->close();
    remote->close();
    DNSResolver->deleteLater();
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

    QByteArray dat = sock->readAll();

    const QByteArray accept = Utils::pack("BB", 5, 0);
    const QByteArray reject = Utils::pack("BB", 0, 91);

    if(dat.length() == 0)
    {
        qDebug() << "empty request data , bye =w=";
    }

    if(!is_local)
    {
        //decrypt data
    }

    // stages
    if(stage == STREAM)
    {

    }
    else if(is_local && stage == INIT)
    {
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

        char _ver = dat[0];

        //if not socks5
        if(_ver != SOCKS5_VER){
            qDebug() << "not socks5 protocol!";
            sock->write(reject);
            sock->close();
        }else{
            sock->write(accept);
            stage = ADDR;
            return ;
        }
    }
    else if(stage == CONNECTING)
    {
        handleStageConnecting();
    }
    else if( (is_local && stage == ADDR) || (!is_local && stage == INIT) )
    {
        handleStageAddr(dat);
    }
//    sock->close();
}

// private methods
void TCPRelay::handleStageAddr(QByteArray &data)
{
    if(is_local)
    {
        int cmd = static_cast<int>(data[1]);

        if(cmd == CMD_UDP_ASSOC)
        {
            QByteArray CR = build_connect_reply();
            local->write(CR);
            stage = UDP_ASSOC;
            return ;
        }else if(cmd == CMD_TCP_CONNECT){
            /*TCP/IP stream connection*/
            data.mid(3);
        }else{
            qDebug() << "unknown command " + QString::number(cmd);
            close();
            return ;
        }
    }

    // parse header
    SocksHeader header;
    parseSocksHeader(data, &header);

    stage = DNS;
    if(is_local){
        const QByteArray DNS_response = Utils::pack("BBBBIBB", 5, 0, 0, 1, 0, 16, 16);
        local->write(DNS_response);
        //encrypt the DNS request

        QByteArray dataToSend = protocol->encrypt(data);

        qDebug() << "- data to send : " << dataToSend;
        createRemoteSocket(server_address, server_port, dataToSend);

    }
}

void TCPRelay::handleStageConnecting()
{
    // TODO
}

void TCPRelay::createRemoteSocket(QHostAddress address, quint16 port, QByteArray &data)
{
    remote = new QTcpSocket(this);
    qDebug() << "address: " << address;

    remote->connectToHost(address, port);

    qDebug() << "new remote start";

    connect(remote, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(showCurrentState(QAbstractSocket::SocketState)));
    connect(remote, SIGNAL(readyRead()), this, SLOT(onRemoteRead()));
    connect(remote, SIGNAL(connected()), this, SLOT(onRemoteWrite()));

    send_data = data;
}

void TCPRelay::onRemoteWrite()
{
    qDebug() << "write data to remote";
    qDebug() << "write data: " << send_data;
    if(send_data.length() > 0){
        remote->write(send_data);
        send_data.clear();
        remote->close();
    }
}

// debug
void TCPRelay::showCurrentState(QAbstractSocket::SocketState state)
{
    qDebug() << "new state: " << state << "\n";
}

QByteArray TCPRelay::build_connect_reply()
{
/*  connect reply format
    +----+-----+-------+------+----------+----------+
    |VER | REP |  RSV  | ATYP | DST.ADDR | DST.PORT |
    +----+-----+-------+------+----------+----------+
    | 1  |  1  | X'00' |  1   | Variable |    2     |
    +----+-----+-------+------+----------+----------+
*/
    QHostAddress addr = local->localAddress();
    quint16 port      = local->localPort();

    uint atyp;

    //ipv4
    if(addr.protocol() == QAbstractSocket::IPv4Protocol)
    {
        atyp = IPV4;
        return Utils::pack("BBBBIH", 5, 0, 0, atyp, addr.toIPv4Address(), port);
    }else{ // ipv6
        atyp = IPV6;
        Q_IPV6ADDR ipv6_addr = addr.toIPv6Address();
        QByteArray ipv6_addr_buf(reinterpret_cast<char*>(ipv6_addr.c), 16);

        return Utils::pack("BBBBxH", 5, 0, 0, atyp, ipv6_addr_buf, port);
    }
}

void TCPRelay::parseSocksHeader(const QByteArray &data, SocksHeader * s_header)
{
    int type = data[0];

    /* struct variables*/
    char    *atyp        = &(s_header->atyp);
    QString *dest_addr   = &(s_header->dest_addr);
    quint16 *dest_port   = &(s_header->dest_port);
    uint    *header_len  = &(s_header->header_length);

    QByteArray ip_buf;
    if(type == HOST)
    {
        if(data.length() > 2)
        {
            *header_len = Utils::unpack("BB[&2]H", data, atyp, dest_addr, dest_port);
        }
    }else if(type == IPV4)
    {
        if(data.length() >= 7)
        {
            *header_len = Utils::unpack("B[4]H", data, atyp, &ip_buf, dest_port);
            *dest_addr = Utils::parseIP(ip_buf);
        }

    }else if(type == IPV6)
    {
       // IPV6 TODO
    }
}

void TCPRelay::onRemoteRead()
{
    qDebug() << remote->readAll();
    qDebug() << "wtf remote";
    remote->close();
}
