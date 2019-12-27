#include "tcpserverconnection.h"
#include <QStringList>

TcpServerConnection::TcpServerConnection()
{

}

TcpServerConnection::~TcpServerConnection() {}

ServerConnection::CONNECTION_STATUS TcpServerConnection::connect(QString address, int port) {
    return CONNECTED;
}

std::vector<Room> *TcpServerConnection::getAvailableRooms() {
}

ServerConnection::CONNECTION_STATUS TcpServerConnection::joinRoom(int room_id) {
    return this->connection_status;
}

ServerConnection::CONNECTION_STATUS TcpServerConnection::getConnectionStatus() {
    return this->connection_status;
}
