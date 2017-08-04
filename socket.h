#ifndef SOCKET_H
#define SOCKET_H

#include "headers.h"

class Socket : public QObject
{
    Q_OBJECT
public:
    Socket(QLocalSocket *_socket);
    QLocalSocket *socket;
};

#endif // SOCKET_H
