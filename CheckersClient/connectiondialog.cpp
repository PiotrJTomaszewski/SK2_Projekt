#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(ServerConnection *server_connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    this->server_connection = server_connection;
    ui->setupUi(this);
    setConnectionStatus(server_connection->getConnectionStatus());
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::setConnectionStatus(ServerConnection::CONNECTION_STATUS is_connected) {
    switch(is_connected) {
    case ServerConnection::NOT_CONNECTED:
        ui->connectionStatus->setStyleSheet("QLabel { color : red; }");
        ui->connectionStatus->setText("You are not connected!");
        ui->serverAddressText->setEnabled(true);
        ui->serverPortBox->setEnabled(true);
        ui->connectButton->setEnabled(true);
        ui->disckonnectButton->setEnabled(false);
        ui->refreshButton->setEnabled(false);
        ui->roomsList->setEnabled(false);
        ui->createRoomButton->setEnabled(false);
        ui->connectionProgress->setValue(0);
        break;
    default:
        ui->connectionStatus->setStyleSheet("QLabel { color : green; }");
        ui->connectionStatus->setText("You are connected");
        ui->serverAddressText->setEnabled(false);
        ui->serverPortBox->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->disckonnectButton->setEnabled(true);
        ui->refreshButton->setEnabled(true);
        ui->roomsList->setEnabled(true);
        ui->createRoomButton->setEnabled(true);
        ui->connectionProgress->setValue(100);
        break;
    }
}
