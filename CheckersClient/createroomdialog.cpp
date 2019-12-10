#include "createroomdialog.h"
#include "ui_createroomdialog.h"

CreateRoomDialog::CreateRoomDialog(ServerConnection *server_connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateRoomDialog)
{
    this->server_connection = server_connection;
    ui->setupUi(this);
    ui->errorText->hide();
}

CreateRoomDialog::~CreateRoomDialog()
{
    delete ui;
}

void CreateRoomDialog::on_createRoomButton_clicked()
{
    QString room_name = ui->roomName->toPlainText();
    room_name.replace(';', ',');
    if (room_name != "") {
        this->server_connection->createRoom(room_name);
        ui->errorText->hide();
        // TODO: Error handling
    }
    else {
        ui->errorText->setText("Error: Room name cannot be empty!");
        ui->errorText->show();
    }
}
