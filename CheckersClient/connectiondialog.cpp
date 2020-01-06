#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "serverconnectionobject.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    this->server_connection = ServerConnectionObject::getServerConnection();
    ui->setupUi(this);
    connect(server_connection, &TcpClient::setConnectionStatusSignal, this, &ConnectionDialog::showConnectionStatus);
    showConnectionStatus(this->server_connection->getConnectionStatus());
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::showConnectionStatus(TcpClient::CONNECTION_STATUS connection_status) {
    switch(connection_status) {
    case TcpClient::NOT_CONNECTED:
        ui->connectionStatus->setStyleSheet("QLabel { color : red; }");
        ui->connectionStatus->setText("You are not connected!");
        ui->serverAddressText->setEnabled(true);
        ui->serverPortBox->setEnabled(true);
        ui->connectButton->setEnabled(true);
        ui->disconnectButton->setEnabled(false);
        break;
    default:
        ui->connectionStatus->setStyleSheet("QLabel { color : green; }");
        ui->connectionStatus->setText("You are connected");
        ui->serverAddressText->setEnabled(false);
        ui->serverPortBox->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
        break;
    }
}

void ConnectionDialog::on_connectButton_clicked()
{
    QString address = ui->serverAddressText->toPlainText();
    int port = ui->serverPortBox->value();
    this->server_connection->connectToServer(address, port);
}

void ConnectionDialog::on_disconnectButton_clicked()
{
    this->server_connection->disconnectFromServer();
}
