#ifndef MOCKSERVER_H
#define MOCKSERVER_H
#include <QString>
#include "room.h"
#include "serverconnection.h"


class MockServer: public ServerConnection {
public:
    MockServer();
    ~MockServer();
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

#endif // MOCKSERVER_H
