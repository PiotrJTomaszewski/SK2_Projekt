#ifndef MOCKSERVER_H
#define MOCKSERVER_H
#include "serverconnection.h"
#include <QString>

class MockServer: public ServerConnection {
public:
    MockServer();
    ~MockServer();
    bool connect(QString address, int port);
    QStringList getAvailableRooms();
    CONNECTION_STATUS getConnectionStatus();
private:
    CONNECTION_STATUS connection_status;
    QString address;
    int port;
};

#endif // MOCKSERVER_H
