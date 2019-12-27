#include "serverconnectionobject.h"
#include "mockserver.h"

ServerConnection* ServerConnectionObject::server_connection = nullptr;

ServerConnectionObject::ServerConnectionObject() {
    server_connection = new MockServer();
}

ServerConnectionObject::~ServerConnectionObject() {
    delete server_connection;
}

ServerConnection* ServerConnectionObject::getServerConnection() {
    return server_connection;
}
