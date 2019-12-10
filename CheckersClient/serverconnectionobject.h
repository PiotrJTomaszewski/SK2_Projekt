#ifndef SERVERCONNECTIONOBJECT_H
#define SERVERCONNECTIONOBJECT_H
#include "serverconnection.h"

class ServerConnectionObject
{
public:
    ServerConnectionObject();
    ~ServerConnectionObject();
    static ServerConnection* getServerConnection();
private:
    static ServerConnection *server_connection;
};

#endif // SERVERCONNECTIONOBJECT_H
