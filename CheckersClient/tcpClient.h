#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QString>
#include <QObject>
#include <QtNetwork>
#include "globals.h"
#include "circularbuffer.h"
#include "server_client_messages.h"

class TcpClient : public QObject{
    Q_OBJECT
public:
    TcpClient();
    virtual ~TcpClient();
    enum CONNECTION_STATUS {NOT_CONNECTED, CONNECTED, IN_ROOM, IN_GAME_LIGHT, IN_GAME_DARK, CONNECTION_ERROR};
    void connectToServer(QString address, int port);
    void disconnectFromServer();
    CONNECTION_STATUS getConnectionStatus();
    void setConnectionStatus(CONNECTION_STATUS new_status);
    void movePiece(int from_field, int to_field);
    GLOBAL::GAME_ERROR getLastGameError();

private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
    QTcpSocket *tcp_socket;
    GLOBAL::GAME_ERROR lastGameError;
    CircularBuffer *receive_buffer;
    // Sends a given message to the game server
    void sendMessage(SERVER_CLIENT_MESSAGE message, int param1=0, int param2=0);

private slots:
    void readData();
    void server_connected();
    void server_disconnected();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void setConnectionStatusSignal(CONNECTION_STATUS new_status);
    void startGameSignal(GLOBAL::COLOR color);
    void endGameSignal();
    void gameStatusSignal(GLOBAL::GAME_STATUS status);
    void gameErrorSignal(GLOBAL::GAME_ERROR error);
    void gamePieceMovedSignal(int from_field, int to_field);
    void gamePromotePieceSignal(int field);
    void debugSignal(QString message);
};

#endif // TCPCLIENT_H
