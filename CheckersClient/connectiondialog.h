#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "tcpClient.h"

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

    void on_disconnectButton_clicked();

public slots:
    void showConnectionStatus(TcpClient::CONNECTION_STATUS connection_status);

private:
    Ui::ConnectionDialog *ui;
    TcpClient *server_connection;
};

#endif // CONNECTIONDIALOG_H
