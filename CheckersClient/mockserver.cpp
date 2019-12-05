#include "mockserver.h"
#include <QStringList>


MockServer::MockServer() {
    this->connection_status = NOT_CONNECTED;
}

MockServer::~MockServer() {}

bool MockServer::connect(QString address, int port) {
    this->address = address;
    this->port = port;
    this->connection_status = CONNECTED;
    return true;
}

QStringList MockServer::getAvailableRooms() {
    QStringList rooms;
    rooms.append("1: test");
    rooms.append("2: test2");
    return rooms;
}

ServerConnection::CONNECTION_STATUS MockServer::getConnectionStatus() {
    return this->connection_status;
}
