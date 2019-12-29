#include "tcpClient.h"
#include <QStringList>
#include <QDebug>


TcpClient::TcpClient() : QObject(){
    setConnectionStatus(NOT_CONNECTED);
    this->tcp_socket = new QTcpSocket();
    connect(this->tcp_socket, &QAbstractSocket::connected, this, &TcpClient::server_connected);
    connect(this->tcp_socket, &QAbstractSocket::disconnected, this, &TcpClient::server_disconnected);
    connect(this->tcp_socket, &QIODevice::readyRead, this, &TcpClient::readData);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(this->tcp_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), this, &TcpClient::displayError);
}

TcpClient::~TcpClient() {
    delete this->tcp_socket;
}

void TcpClient::connectToServer(QString address, int port) {
    this->address = address;
    this->port = port;
    this->tcp_socket->connectToHost(address, static_cast<quint16>(port));
//    setConnectionStatus(CONNECTED);
//    return CONNECTED;
}

void TcpClient::disconnectFromServer() {
    qInfo("Disconnected");
    setConnectionStatus(NOT_CONNECTED);
}


TcpClient::CONNECTION_STATUS TcpClient::getConnectionStatus() {
    return this->connection_status;
}

void TcpClient::setConnectionStatus(CONNECTION_STATUS new_status) {
    this->connection_status = new_status;
    TcpClient::setConnectionStatusSignal(new_status);
}

void TcpClient::movePiece(int from_field, int to_field) {
    char message[20];
    sprintf(message, "move_piece_%d_%d;", from_field, to_field);
    tcp_socket->write(message);
    qInfo() << "Move piece from: " << from_field << " to: " << to_field;
}

void TcpClient::readData() {
    QString data = tcp_socket->readAll();
    QStringList splitted_data = data.split(';');
    for (int i=0; i<splitted_data.size(); ++i) {
        qInfo() << splitted_data[i];
        if (splitted_data[i] == "welcome") {
            this->setConnectionStatus(CONNECTED);
        }
        else if (splitted_data[i] == "waiting_for_opponent") {
            this->setConnectionStatus(IN_ROOM);
        }
        else if (splitted_data[i] == "play_light") {
            this->setConnectionStatus(IN_GAME_LIGHT);
            startGameSignal(GLOBAL::COLOR::LIGHT);
        }
        else if (splitted_data[i] == "play_dark") {
            this->setConnectionStatus(IN_GAME_DARK);
            startGameSignal(GLOBAL::COLOR::DARK);
        }
    }
}

void TcpClient::server_connected() {
    this->setConnectionStatus(CONNECTED);
    qInfo() << "Server connected";
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError) {
    qInfo() << socketError;
}

void TcpClient::server_disconnected() {
    qInfo() << "Server disconnected";
    this->setConnectionStatus(NOT_CONNECTED);
}
