#include "tcpclient.h"
#include <QStringList>
#include <QDebug>


TcpClient::TcpClient() : QObject(nullptr) {
    setConnectionStatus(NOT_CONNECTED);
    this->tcp_socket = new QTcpSocket();
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
//    connect(this->tcp_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), this, &TcpClient::displayError);
//    this->available_rooms = new std::vector<Room>;
}

//TcpClient::~TcpClient() {
////    std::vector<Room>().swap(*available_rooms); // Clear rooms vector
////    delete available_rooms;
//}

void TcpClient::connectToServer(QString address, int port) {
    this->address = address;
    this->port = port;
    this->tcp_socket->connectToHost(address, static_cast<quint16>(port)); // TODO: Read connection state
}

void TcpClient::disconnectFromServer() {
    qInfo("Disconnected");
    setConnectionStatus(NOT_CONNECTED);
}

//std::vector<Room> *MockServer::getAvailableRooms() {
//    available_rooms->clear();
//    available_rooms->push_back(Room(1, "asadd"));
//    available_rooms->push_back(Room(2, "asd"));
//    return available_rooms;
//}

//ServerConnection::CONNECTION_STATUS MockServer::joinRoom(int room_id) {
//    qInfo("Selected room %d", room_id);
//    setConnectionStatus(IN_ROOM);
//    startGameSignal(GLOBAL::COLOR::LIGHT);
//    return this->connection_status;
//}


TcpClient::CONNECTION_STATUS TcpClient::getConnectionStatus() {
    return this->connection_status;
}

void TcpClient::setConnectionStatus(CONNECTION_STATUS new_status) {
    this->connection_status = new_status;
    TcpClient::setConnectionStatusSignal(new_status);
}

//void MockServer::createRoom(QString room_name) {

//}

void TcpClient::movePiece(int from_field, int to_field) {
    qInfo() << "Move piece from: " << from_field << " to: " << to_field;
}

void TcpClient::readData() {

}
