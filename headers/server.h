#ifndef SERVER_H
#define SERVER_H

#include "headers.h"
#include "compositor.h"
#include "socket.h"

// Unix Sockets for IPC

class Server : public QLocalServer
{
public:
    Server(Compositor *_compositor); // Constructor

    Compositor *compositor; // Reference to the Compositor
    QList<Socket*> sockets; // List of connected clients

    void clientDisconnected(QLocalSocket *client);
    void sendMessage(QLocalSocket *to,MessageType type,QByteArray *data);

public slots:
    void clientConnected();
};

#endif // SERVER_H
