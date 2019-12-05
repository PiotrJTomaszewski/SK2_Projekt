#include "tcpserverconnection.h"
#include <QStringList>

TcpServerConnection::TcpServerConnection()
{

}

TcpServerConnection::~TcpServerConnection() {}

bool TcpServerConnection::connect(QString address, int port) {
    this->address = address;
    this->port = port;
    this->connection_status = CONNECTED;
    return true;
}

QStringList TcpServerConnection::getAvailableRooms() {
    return QStringList();
}

ServerConnection::CONNECTION_STATUS TcpServerConnection::getConnectionStatus() {
    return this->connection_status;
}
