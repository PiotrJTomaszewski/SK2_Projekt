#ifndef TCPSERVERCONNECTION_H
#define TCPSERVERCONNECTION_H

#include "serverconnection.h"
#include "room.h"


class TcpServerConnection : public ServerConnection {
public:
    TcpServerConnection();
    ~TcpServerConnection();
    CONNECTION_STATUS connect(QString address, int port);
    std::vector<Room> *getAvailableRooms();
    virtual CONNECTION_STATUS joinRoom(int room_id);
    CONNECTION_STATUS getConnectionStatus();
private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
    std::vector<Room> *available_rooms;
};

#endif // TCPSERVERCONNECTION_H
