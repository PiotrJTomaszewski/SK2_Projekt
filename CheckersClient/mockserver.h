#ifndef MOCKSERVER_H
#define MOCKSERVER_H
#include <QString>
#include "room.h"
#include "serverconnection.h"


class MockServer: public ServerConnection {
public:
    MockServer();
    ~MockServer();
    CONNECTION_STATUS connectToServer(QString address, int port);
    void disconnectFromServer();
    std::vector<Room> *getAvailableRooms();
    CONNECTION_STATUS joinRoom(int room_id);
    CONNECTION_STATUS getConnectionStatus();
    void setConnectionStatus(CONNECTION_STATUS new_status);
    void createRoom(QString room_name);
    void movePiece(int from_field, int to_field);

private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
    std::vector<Room> *available_rooms;
};

#endif // MOCKSERVER_H
