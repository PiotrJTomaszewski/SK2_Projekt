#include "tcpClient.h"
#include <QStringList>
#include <QDebug>
#include "server_client_messages.h"


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
    char message[SCMSG_MESSAGE_LENGTH+1];
    sprintf(message, "%02d %02d %02d\n", static_cast<int>(SCMSG_MOVE_PIECE), from_field, to_field);
    tcp_socket->write(message);
    qInfo() << "Move piece from: " << from_field << " to: " << to_field;
}

void TcpClient::readData() {
    while (tcp_socket->canReadLine()) {
        SERVER_CLIENT_MESSAGE message_code;
        int tmp;
        int param1;
        int param2;
        QString data_line = tcp_socket->readLine();
        debugSignal(data_line);
        qInfo() << "Message received: " << data_line;
        if (data_line.length() >= SCMSG_MESSAGE_LENGTH) {
            QTextStream myteststream(&data_line);
            myteststream >> tmp >> param1 >> param2;
            message_code = static_cast<SERVER_CLIENT_MESSAGE>(tmp);
            qInfo() << "Message received: " << message_code << " " << param1 << " " << param2;
            switch(message_code) {
            case SCMSG_WELCOME:
                this->setConnectionStatus(CONNECTED);
                debugSignal("Connected to server");
                qInfo() << "Connected to server";
                break;
            case SCMSG_WAITING_FOR_OPPONENT:
                this->setConnectionStatus(IN_ROOM);
                debugSignal("Waiting for the opponent");
                qInfo() << "Waiting for the opponent";
                break;
            case SCMSG_GAME_START_YOU_PLAY_LIGHT:
                this->setConnectionStatus(IN_GAME_LIGHT);
                startGameSignal(GLOBAL::COLOR::LIGHT);
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Game starting. You are playing light");
                qInfo() << "Game starting. You are playing light";
                break;
            case SCMSG_GAME_START_YOU_PLAY_DARK:
                this->setConnectionStatus(IN_GAME_DARK);
                startGameSignal(GLOBAL::COLOR::DARK);
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Game starting. You are playing dark");
                qInfo() << "Game starting. You are playing dark";
                break;
            case SCMSG_GAME_ERROR:
                this->lastGameError = static_cast<GLOBAL::GAME_ERROR>(param1);
                this->gameErrorSignal(static_cast<GLOBAL::GAME_ERROR>(param1));
                debugSignal(QStringLiteral("Game error %1").arg(param1));
                qInfo() << "Game error " << param1;
                break;
            case SCMSG_LIGHT_TURN:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Light player turn");
                qInfo() << "Light player turn";
                break;
            case SCMSG_DARK_TURN:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_DARK);
                debugSignal("Dark player turn");
                qInfo() << "Dark player turn";
                break;
            case SCMSG_PIECE_MOVED:
                this->gamePieceMovedSignal(param1, param2);
                debugSignal(QStringLiteral("Piece moved from %1 to %1").arg(param1, param2));
                qInfo() << "Piece moved from " << param1 << " to " << param2;
                break;
            case SCMSG_PIECE_CAPTURED:
                this->gamePieceMovedSignal(param1, -1);
                debugSignal(QStringLiteral("Piece captured from %1").arg(param1));
                qInfo() << "Piece captured from " << param1;
                break;
            case SCMSG_PROMOTE:
                gamePromotePieceSignal(param1);
                debugSignal(QStringLiteral("Promoting ").arg(param1));
                qInfo() << "Promoting " << param1;
                break;
            case SCMSG_GAME_END_LIGHT_WON:
                debugSignal("Light player won!");
                qInfo() << "Light player won!";
                break;
            case SCMSG_GAME_END_DARK_WON:
                debugSignal("Dark player won!");
                qInfo() << "Dark player won!";
                break;
            case SCMSG_GAME_END_TIE:
                break;
            case SCMSG_OPPONENT_LEFT:
                debugSignal("Your opponent left");
                qInfo() << "Your opponent left";
                break;
            default:
                debugSignal(QStringLiteral("Unknown command %1").arg(message_code));
                qInfo() << "Unknown command " << message_code;
                break;
            }
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

GLOBAL::GAME_ERROR TcpClient::getLastGameError() {
    return this->lastGameError;
}
