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
//    connect(server_connection, SIGNAL(server_connection->setConnectionStatusSignal()), this, SLOT(this->showConnectionStatus()))
    showConnectionStatus(server_connection->getConnectionStatus());
//    local_room_list = new std::vector<Room>();
}

ConnectionDialog::~ConnectionDialog()
{
//    std::vector<Room>().swap(*local_room_list); // Clear rooms vector
//    delete local_room_list;
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
//        ui->refreshButton->setEnabled(false);
//        ui->roomsList->setEnabled(false);
//        ui->joinRoomButton->setEnabled(false);
//        ui->createRoomButton->setEnabled(false);
        ui->connectionProgress->setValue(0);
        break;
    case TcpClient::CONNECTED:
        ui->connectionStatus->setStyleSheet("QLabel { color : green; }");
        ui->connectionStatus->setText("You are connected");
        ui->serverAddressText->setEnabled(false);
        ui->serverPortBox->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
//        ui->refreshButton->setEnabled(true);
//        ui->roomsList->setEnabled(true);
//        ui->joinRoomButton->setEnabled(true);
//        ui->createRoomButton->setEnabled(true);
        ui->connectionProgress->setValue(50);
        break;
    case TcpClient::IN_ROOM:
        ui->connectionStatus->setStyleSheet("QLabel { color : green; }");
        ui->connectionStatus->setText("Waiting for the opponent");
        ui->serverAddressText->setEnabled(false);
        ui->serverPortBox->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
//        ui->refreshButton->setEnabled(false);
//        ui->roomsList->setEnabled(false);
//        ui->joinRoomButton->setEnabled(false);
//        ui->createRoomButton->setEnabled(false);
        ui->connectionProgress->setValue(100);
        break;
    case TcpClient::IN_GAME_LIGHT: case TcpClient::IN_GAME_DARK:
        ui->connectionStatus->setStyleSheet("QLabel { color : green; }");
        ui->connectionStatus->setText("You are in game");
        ui->serverAddressText->setEnabled(false);
        ui->serverPortBox->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
//        ui->refreshButton->setEnabled(false);
//        ui->roomsList->setEnabled(false);
//        ui->joinRoomButton->setEnabled(false);
//        ui->createRoomButton->setEnabled(false);
        ui->connectionProgress->setValue(100);
        break;
    default:
        break;
    }
}

void ConnectionDialog::on_connectButton_clicked()
{
    QString address = ui->serverAddressText->toPlainText();
    int port = ui->serverPortBox->value();
    this->server_connection->connectToServer(address, port);
//    showConnectionStatus(status);
//    if (status == TcpClient::CONNECTED)
//        fillRoomsList();
}

void ConnectionDialog::fillRoomsList() {
//    ui->roomsList->clear();
//    local_room_list->clear();
//    for (auto const& room: *server_connection->getAvailableRooms()) {
//        local_room_list->push_back(Room(room));
//    }
//    for (auto &room: *local_room_list) {
//        ui->roomsList->addItem(&room);
//    }
}

void ConnectionDialog::on_refreshButton_clicked() {
    fillRoomsList();
}

void ConnectionDialog::on_joinRoomButton_clicked() {
//    Room *selected_room = dynamic_cast<Room*>(ui->roomsList->currentItem());
//    if (selected_room != nullptr) {
//        int selected_room_id = selected_room->getId();
//        ServerConnection::CONNECTION_STATUS status = this->server_connection->joinRoom(selected_room_id);
////        showConnectionStatus(status);
//    }
    // TODO: Add error handling
}

void ConnectionDialog::on_createRoomButton_clicked()
{
//    CreateRoomDialog create_room_dialog(this);
//    create_room_dialog.exec();
}

void ConnectionDialog::on_disconnectButton_clicked()
{
    server_connection->disconnectFromServer();
}
