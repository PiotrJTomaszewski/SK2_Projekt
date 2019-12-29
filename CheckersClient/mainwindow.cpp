#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"
#include "serverconnectionobject.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->actionGameRoom, &QAction::triggered, this, &MainWindow::connectAndJoinRoom);
    server_connection = ServerConnectionObject::getServerConnection();
    connect(server_connection, &TcpClient::setConnectionStatusSignal, this, &MainWindow::showConnectionStatus);

}

MainWindow::~MainWindow() {
    delete ui;
    delete server_connection;
}

void MainWindow::connectAndJoinRoom() {
    ConnectionDialog dialog(this);
    dialog.exec();
    if (this->server_connection->getConnectionStatus() != TcpClient::NOT_CONNECTED && this->server_connection->getConnectionStatus() != TcpClient::CONNECTION_ERROR) {
        ui->notInGameWarning->hide();
    }
}

void MainWindow::showConnectionStatus(TcpClient::CONNECTION_STATUS connection_status) {
    switch(connection_status) {
    case TcpClient::NOT_CONNECTED:
        ui->connectionStatus->setText("Not connected to server");
        ui->notInGameWarning->show();
        break;
    case TcpClient::CONNECTED:
        ui->connectionStatus->setText("You aren't in a game room");
        break;
    case TcpClient::IN_ROOM:
        ui->connectionStatus->setText("Waiting for oponent");
        ui->notInGameWarning->hide();
        break;
    case TcpClient::IN_GAME_LIGHT:
        ui->connectionStatus->setText("In game. Playing light");
        ui->notInGameWarning->hide();
        break;
    default:
        break;
    }
}
