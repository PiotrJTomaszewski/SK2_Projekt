#include "tcpClient.h"
#include <QMessageBox>
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
}

void TcpClient::disconnectFromServer() {
    this->tcp_socket->disconnectFromHost();
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
            switch(message_code) {
            case SCMSG_WELCOME:
                this->setConnectionStatus(CONNECTED);
                debugSignal("Connected to server");
                break;
            case SCMSG_WAITING_FOR_OPPONENT:
                this->setConnectionStatus(IN_ROOM);
                debugSignal("Waiting for the opponent");
                break;
            case SCMSG_GAME_START_YOU_PLAY_LIGHT:
                this->setConnectionStatus(IN_GAME_LIGHT);
                startGameSignal(GLOBAL::COLOR::LIGHT);
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Game starting. You are playing light");
                break;
            case SCMSG_GAME_START_YOU_PLAY_DARK:
                this->setConnectionStatus(IN_GAME_DARK);
                startGameSignal(GLOBAL::COLOR::DARK);
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Game starting. You are playing dark");
                break;
            case SCMSG_GAME_ERROR:
                this->lastGameError = static_cast<GLOBAL::GAME_ERROR>(param1);
                this->gameErrorSignal(static_cast<GLOBAL::GAME_ERROR>(param1));
                debugSignal(QStringLiteral("Game error %1").arg(param1));
                break;
            case SCMSG_LIGHT_TURN:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Light player turn");
                break;
            case SCMSG_DARK_TURN:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_DARK);
                debugSignal("Dark player turn");
                break;
            case SCMSG_PIECE_MOVED:
                this->gamePieceMovedSignal(param1, param2);
                debugSignal(QStringLiteral("Piece moved from %1 to %1").arg(param1, param2));
                break;
            case SCMSG_PIECE_CAPTURED:
                this->gamePieceMovedSignal(param1, -1);
                debugSignal(QStringLiteral("Piece captured from %1").arg(param1));
                break;
            case SCMSG_PROMOTE:
                gamePromotePieceSignal(param1);
                debugSignal(QStringLiteral("Promoting ").arg(param1));
                break;
            case SCMSG_GAME_END_LIGHT_WON:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::LIGHT_WON);
                this->endGameSignal();
                debugSignal("Light player won!");
                break;
            case SCMSG_GAME_END_DARK_WON:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::DARK_WON);
                this->endGameSignal();
                debugSignal("Dark player won!");
                break;
            case SCMSG_GAME_END_TIE:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TIE);
                this->endGameSignal();
                break;
            case SCMSG_OPPONENT_LEFT:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::OPPONENT_LEFT);
                this->endGameSignal();
                debugSignal("Your opponent left");
                break;
            default:
                debugSignal(QStringLiteral("Unknown command %1").arg(message_code));
                break;
            }
        }
    }
}

void TcpClient::server_connected() {
    this->setConnectionStatus(CONNECTED);
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError) {
    QMessageBox msg_box;
    switch(socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        msg_box.setText("Connection error!\nConnection refused!");
        msg_box.exec();
        break;
    case QAbstractSocket::RemoteHostClosedError:
//        msg_box.setText("Connection error!\nRemote host closed!");
//        msg_box.exec();
        break;
    case QAbstractSocket::HostNotFoundError:
        msg_box.setText("Connection error!\nHost not found!");
        msg_box.exec();
        break;
    default:
        msg_box.setText(QStringLiteral("Connection error!\n Error code %1").arg(socketError));
        msg_box.exec();
        break;
    }

}

void TcpClient::server_disconnected() {
    this->setConnectionStatus(NOT_CONNECTED);
}

GLOBAL::GAME_ERROR TcpClient::getLastGameError() {
    return this->lastGameError;
}
