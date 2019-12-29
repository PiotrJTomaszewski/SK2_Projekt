#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QtNetwork>
#include "room.h"
#include "globals.h"

class TcpClient: public QObject
{
    Q_OBJECT
public:
    TcpClient();
//    ~TcpClient();
    virtual ~TcpClient() = 0;
    enum CONNECTION_STATUS {NOT_CONNECTED, CONNECTED, IN_ROOM, IN_GAME, CONNECTION_ERROR};
    void connectToServer(QString address, int port);
    void disconnectFromServer();
    CONNECTION_STATUS getConnectionStatus();
    void setConnectionStatus(CONNECTION_STATUS new_status);
    void movePiece(int from_field, int to_field);
private:
    QTcpSocket *tcp_socket;
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
    void readData();
//private slots:
//    void displayError(QAbstractSocket::SocketError socketError);
signals:
    void setConnectionStatusSignal(CONNECTION_STATUS new_status);
    void startGameSignal(GLOBAL::COLOR color);
};

#endif // TCPCLIENT_H
