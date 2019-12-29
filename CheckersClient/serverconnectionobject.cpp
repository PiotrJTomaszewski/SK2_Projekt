#include "serverconnectionobject.h"
#include "tcpClient.h"

TcpClient* ServerConnectionObject::server_connection = nullptr;

ServerConnectionObject::ServerConnectionObject() {
    server_connection = new TcpClient();
}

ServerConnectionObject::~ServerConnectionObject() {
    delete server_connection;
}

TcpClient* ServerConnectionObject::getServerConnection() {
    return server_connection;
}
