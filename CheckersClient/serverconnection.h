#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <QString>

class ServerConnection {
public:
    ServerConnection();
    virtual ~ServerConnection() = 0;
    enum CONNECTION_STATUS {NOT_CONNECTED, CONNECTED, IN_ROOM, IN_GAME, CONNECTION_ERROR};
    virtual bool connect(QString address, int port) = 0;
    virtual QStringList getAvailableRooms() = 0;
    virtual CONNECTION_STATUS getConnectionStatus() = 0;

private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
};

#endif // SERVERCONNECTION_H
