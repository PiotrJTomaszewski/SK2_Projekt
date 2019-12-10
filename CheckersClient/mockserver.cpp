#include "mockserver.h"
#include <QStringList>


MockServer::MockServer() : ServerConnection() {
    setConnectionStatus(NOT_CONNECTED);
    this->available_rooms = new std::vector<Room>;
}

MockServer::~MockServer() {
    std::vector<Room>().swap(*available_rooms); // Clear rooms vector
    delete available_rooms;
}

ServerConnection::CONNECTION_STATUS MockServer::connectToServer(QString address, int port) {
    this->address = address;
    this->port = port;
    setConnectionStatus(CONNECTED);
    return CONNECTED;
}

std::vector<Room> *MockServer::getAvailableRooms() {
    available_rooms->clear();
    available_rooms->push_back(Room(1, "asadd"));
    available_rooms->push_back(Room(2, "asd"));
    return available_rooms;
}

ServerConnection::CONNECTION_STATUS MockServer::joinRoom(int room_id) {
    qInfo("Selected room %d", room_id);
    setConnectionStatus(IN_ROOM);
    return this->connection_status;
}


ServerConnection::CONNECTION_STATUS MockServer::getConnectionStatus() {
    return this->connection_status;
}

void MockServer::setConnectionStatus(CONNECTION_STATUS new_status) {
    this->connection_status = new_status;
    ServerConnection::setConnectionStatusSignal(new_status);
}

void MockServer::createRoom(QString room_name) {

}
