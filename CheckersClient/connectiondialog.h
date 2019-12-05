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
    void setConnectionStatus(ServerConnection::CONNECTION_STATUS is_connected);
    ~ConnectionDialog();

private:
    Ui::ConnectionDialog *ui;
    ServerConnection *server_connection;
};

#endif // CONNECTIONDIALOG_H
