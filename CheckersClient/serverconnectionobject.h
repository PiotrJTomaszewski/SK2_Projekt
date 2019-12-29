#ifndef SERVERCONNECTIONOBJECT_H
#define SERVERCONNECTIONOBJECT_H
#include "tcpClient.h"

class ServerConnectionObject
{
public:
    ServerConnectionObject();
    ~ServerConnectionObject();
    static TcpClient* getServerConnection();
private:
    static TcpClient *server_connection;
};

#endif // SERVERCONNECTIONOBJECT_H
