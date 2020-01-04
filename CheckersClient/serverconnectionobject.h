#ifndef SERVERCONNECTIONOBJECT_H
#define SERVERCONNECTIONOBJECT_H
#include "tcpClient.h"

/**
 * @brief A singleton class providing access to the server connection
 */
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
