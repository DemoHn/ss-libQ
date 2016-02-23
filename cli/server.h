#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <sslibQ>

#include "config.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    bool start(Config &config);

private:
    Controller *ctl;

private slots:
};

#endif // SERVER_H
