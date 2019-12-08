#include "mockserver.h"
#include <QStringList>


MockServer::MockServer() {
    this->connection_status = NOT_CONNECTED;
    this->available_rooms = new std::vector<Room>;
}

MockServer::~MockServer() {
    std::vector<Room>().swap(*available_rooms); // Clear rooms vector
    delete available_rooms;
}

ServerConnection::CONNECTION_STATUS MockServer::connect(QString address, int port) {
    this->address = address;
    this->port = port;
    this->connection_status = CONNECTED;
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
    this->connection_status = IN_ROOM;
    return this->connection_status;
}


ServerConnection::CONNECTION_STATUS MockServer::getConnectionStatus() {
    return this->connection_status;
}
