#ifndef TCPSERVERCONNECTION_H
#define TCPSERVERCONNECTION_H

#include "serverconnection.h"

class TcpServerConnection : public ServerConnection {
public:
    TcpServerConnection();
    ~TcpServerConnection();
    bool connect(QString address, int port);
    QStringList getAvailableRooms();
    CONNECTION_STATUS getConnectionStatus();
private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
};

#endif // TCPSERVERCONNECTION_H
