#include "headers/socket.h"

Socket::Socket(QLocalSocket *_socket)
{
    socket = _socket;
    connect(socket,SIGNAL(readyRead()),this,SLOT(newMessage()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconn()));
}

void Socket::newMessage()
{
    messageIn();
}

void Socket::disconn()
{
    disconnected();
}
