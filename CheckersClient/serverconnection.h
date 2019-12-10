#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <QString>
#include <vector>
#include "room.h"

class ServerConnection : public QObject {
    Q_OBJECT
public:
    ServerConnection();
    virtual ~ServerConnection() = 0;
    enum CONNECTION_STATUS {NOT_CONNECTED, CONNECTED, IN_ROOM, IN_GAME, CONNECTION_ERROR};
    virtual CONNECTION_STATUS connectToServer(QString address, int port) = 0;
    virtual std::vector<Room> *getAvailableRooms() = 0;
    virtual CONNECTION_STATUS joinRoom(int room_id) = 0;
    virtual CONNECTION_STATUS getConnectionStatus() = 0;
    virtual void setConnectionStatus(CONNECTION_STATUS new_status) = 0;
    virtual void createRoom(QString room_name) = 0;

private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
    std::vector<Room> *available_rooms;

signals:
    void setConnectionStatusSignal(CONNECTION_STATUS new_status);
};

#endif // SERVERCONNECTION_H
