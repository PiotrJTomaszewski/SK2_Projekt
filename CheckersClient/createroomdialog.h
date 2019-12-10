#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>
#include "serverconnection.h"

namespace Ui {
class CreateRoomDialog;
}

class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoomDialog(ServerConnection *server_connection, QWidget *parent = nullptr);
    ~CreateRoomDialog();

private slots:
    void on_createRoomButton_clicked();

private:
    Ui::CreateRoomDialog *ui;
    ServerConnection *server_connection;
};

#endif // CREATEROOMDIALOG_H
