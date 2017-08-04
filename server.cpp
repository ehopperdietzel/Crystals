#include "server.h"

Server::Server(Compositor *_compositor)
{
    compositor = _compositor;
    connect(this,SIGNAL(newConnection()),this,SLOT(clientConnected()));
}

void Server::clientConnected()
{

}
