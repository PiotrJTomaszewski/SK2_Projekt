#include "tcpClient.h"
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include "server_client_messages.h"


TcpClient::TcpClient() : QObject(){
    this->receive_buffer = new CircularBuffer(64);
    setConnectionStatus(NOT_CONNECTED);
    this->tcp_socket = new QTcpSocket();
    connect(this->tcp_socket, &QAbstractSocket::connected, this, &TcpClient::server_connected);
    connect(this->tcp_socket, &QAbstractSocket::disconnected, this, &TcpClient::server_disconnected);
    connect(this->tcp_socket, &QIODevice::readyRead, this, &TcpClient::readData);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(this->tcp_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), this, &TcpClient::displayError);
}

TcpClient::~TcpClient() {
    delete this->receive_buffer;
    delete this->tcp_socket;
}

void TcpClient::connectToServer(QString address, int port) {
    // Clear the data receive buffer
    this->receive_buffer->clear();
    this->address = address;
    this->port = port;
    this->tcp_socket->connectToHost(address, static_cast<quint16>(port));
}

void TcpClient::disconnectFromServer() {
    sendMessage(SCMSG_GOODBYE);
    this->tcp_socket->close();
}

TcpClient::CONNECTION_STATUS TcpClient::getConnectionStatus() {
    return this->connection_status;
}

void TcpClient::setConnectionStatus(CONNECTION_STATUS new_status) {
    this->connection_status = new_status;
    TcpClient::setConnectionStatusSignal(new_status);
}

void TcpClient::movePiece(int from_field, int to_field) {
    sendMessage(SCMSG_MOVE_PIECE, from_field, to_field);
}

void TcpClient::sendMessage(SERVER_CLIENT_MESSAGE message_code, int param1, int param2) {
    char message[SCMSG_MESSAGE_LENGTH+1];
    // Create a buffer. It will hold the part of the message that still needs to be send
    char message_buffer[SCMSG_MESSAGE_LENGTH+1];
    sprintf(message, "%02d %02d %02d\n", message_code, param1, param2);
    strcpy(message_buffer, message);
    size_t bytes_left_to_send = SCMSG_MESSAGE_LENGTH;
    int bytes_sent = 0;
    long long n;
    while (bytes_left_to_send > 0) {
        n = tcp_socket->write(message_buffer);
        if (n == -1) {
            QMessageBox msg_box;
            msg_box.setText("Error while sending data to the server!");
            msg_box.exec();
            break;
        }
        bytes_left_to_send -= static_cast<size_t>(n);
        bytes_sent += n;
        // If the whole message still wasn't sent to the server put the remaining part into the buffer
        if (bytes_left_to_send > 0) {
            strncpy(message_buffer, message + bytes_sent, bytes_left_to_send);
        }
    }
}

void TcpClient::readData() {
    // Get all available data from the server
    QByteArray available_data = tcp_socket->readAll();
    this->receive_buffer->writeBytes(available_data);
    // While there are enough bytes in the buffer
    while (this->receive_buffer->getToRead() >= SCMSG_MESSAGE_LENGTH) {
        // Copy one message from buffer to the local one
        char local_buf[SCMSG_MESSAGE_LENGTH + 1];
        bool message_read = false;
        int read_bytes = 0;
        while (!message_read) {
            local_buf[read_bytes] = this->receive_buffer->readByte();
            if (local_buf[read_bytes] == '\n') message_read = true;
            ++read_bytes;
        }
        // Parse the message and act accordingly
        SERVER_CLIENT_MESSAGE message_code;
        int tmp;
        int param1;
        int param2;
        QMessageBox msg_box;
        if (read_bytes >= SCMSG_MESSAGE_LENGTH) {
            sscanf(local_buf, "%02d %02d %02d\n", &tmp, &param1, &param2);
            message_code = static_cast<SERVER_CLIENT_MESSAGE>(tmp);
            switch(message_code) {
            case SCMSG_WAITING_FOR_OPPONENT:
                this->setConnectionStatus(IN_ROOM);
                debugSignal("Waiting for the opponent");
                break;
            case SCMSG_GAME_START:
                if (param1 == GLOBAL::COLOR::LIGHT) {
                    this->setConnectionStatus(IN_GAME_LIGHT);
                    startGameSignal(GLOBAL::COLOR::LIGHT);
                    this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                    debugSignal("Game starting. You are playing light");
                } else {
                    this->setConnectionStatus(IN_GAME_DARK);
                    startGameSignal(GLOBAL::COLOR::DARK);
                    this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                    debugSignal("Game starting. You are playing dark");
                }
                break;
            case SCMSG_GAME_ERROR:
                this->lastGameError = static_cast<GLOBAL::GAME_ERROR>(param1);
                this->gameErrorSignal(static_cast<GLOBAL::GAME_ERROR>(param1));
                debugSignal(QStringLiteral("Game error %1").arg(param1));
                break;
            case SCMSG_NEW_TURN:
                if (param1 == GLOBAL::COLOR::LIGHT) {
                this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_LIGHT);
                debugSignal("Light player turn");
                } else {
                    this->gameStatusSignal(GLOBAL::GAME_STATUS::TURN_DARK);
                    debugSignal("Dark player turn");
                }
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
            case SCMSG_GAME_END:
                switch(param1) {
                case GLOBAL::COLOR::NO_COLOR:
                    this->gameStatusSignal(GLOBAL::GAME_STATUS::TIE);
                    debugSignal("A tie!");
                    break;
                case GLOBAL::COLOR::LIGHT:
                    this->gameStatusSignal(GLOBAL::GAME_STATUS::LIGHT_WON);
                    debugSignal("Light player won!");
                    break;
                case GLOBAL::COLOR::DARK:
                    this->gameStatusSignal(GLOBAL::GAME_STATUS::DARK_WON);
                    debugSignal("Dark player won!");
                }
                this->endGameSignal();
                break;
            case SCMSG_OPPONENT_LEFT:
                this->gameStatusSignal(GLOBAL::GAME_STATUS::OPPONENT_LEFT);
                this->endGameSignal();
                debugSignal("Your opponent left");
                break;
            case SCMSG_CRITICAL_ERROR:
                msg_box.setText("A critical error has occurred on the server side!\nYou will most likely get disconnected now!");
                msg_box.exec();
                break;
            default:
                debugSignal(QStringLiteral("Unknown command %1").arg(message_code));
                break;
            }
        }
    }
}

void TcpClient::server_connected() {
    debugSignal("Server connected");
    this->setConnectionStatus(CONNECTED);
}

void TcpClient::server_disconnected() {
    debugSignal("Server disconnected");
    this->setConnectionStatus(NOT_CONNECTED);
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

GLOBAL::GAME_ERROR TcpClient::getLastGameError() {
    return this->lastGameError;
}
