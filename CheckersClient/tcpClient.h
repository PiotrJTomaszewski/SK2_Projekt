#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QString>
#include <QObject>
#include <QtNetwork>
#include "globals.h"

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

private slots:
    void readData();
    void server_connected();
    void server_disconnected();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void setConnectionStatusSignal(CONNECTION_STATUS new_status);
    void startGameSignal(GLOBAL::COLOR color);
    void gameStatusSignal(GLOBAL::GAME_STATUS status);
    void gameErrorSignal(GLOBAL::GAME_ERROR error);
    void gamePieceMovedSignal(int from_field, int to_field);
    void gamePromotePieceSignal(int field);
};

#endif // TCPCLIENT_H
