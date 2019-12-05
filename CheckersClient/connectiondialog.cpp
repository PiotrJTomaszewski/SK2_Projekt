#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    setConnectionStatus(false);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::setConnectionStatus(bool is_connected) {
    if (is_connected) {
        ui->connectionStatus->setStyleSheet("QLabel { color : green; }");
        ui->connectionStatus->setText("You are connected");
        ui->serverAddressText->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->disckonnectButton->setEnabled(true);
        ui->refreshButton->setEnabled(true);
        ui->roomsList->setEnabled(true);
        ui->createRoomButton->setEnabled(true);
    }
    else {
        ui->connectionStatus->setStyleSheet("QLabel { color : red; }");
        ui->connectionStatus->setText("You are not connected!");
        ui->serverAddressText->setEnabled(true);
        ui->connectButton->setEnabled(true);
        ui->disckonnectButton->setEnabled(false);
        ui->refreshButton->setEnabled(false);
        ui->roomsList->setEnabled(false);
        ui->createRoomButton->setEnabled(false);
    }
}
