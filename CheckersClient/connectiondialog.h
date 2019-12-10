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
    explicit ConnectionDialog(QWidget *parent = nullptr);
    void fillRoomsList();
    ~ConnectionDialog();

private slots:
    void on_connectButton_clicked();

    void on_refreshButton_clicked();

    void on_joinRoomButton_clicked();

    void on_createRoomButton_clicked();

    void on_disconnectButton_clicked();

public slots:
    void showConnectionStatus(ServerConnection::CONNECTION_STATUS connection_status);

private:
    Ui::ConnectionDialog *ui;
    ServerConnection *server_connection;
    std::vector<Room> *local_room_list;
};

#endif // CONNECTIONDIALOG_H
