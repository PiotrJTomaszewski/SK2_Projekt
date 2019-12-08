#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "serverconnection.h"

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(ServerConnection *server_connection, QWidget *parent = nullptr);
    void showConnectionStatus(ServerConnection::CONNECTION_STATUS connection_status);
    void fillRoomsList();
    ~ConnectionDialog();

private slots:
    void on_connectButton_clicked();

    void on_refreshButton_clicked();

    void on_joinRoomButton_clicked();

private:
    Ui::ConnectionDialog *ui;
    ServerConnection *server_connection;
    std::vector<Room> *local_room_list;
};

#endif // CONNECTIONDIALOG_H
