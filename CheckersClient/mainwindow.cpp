#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->actionGameRoom, &QAction::triggered, this, &MainWindow::connectAndJoinRoom);
    server_connection = new MockServer();
}

MainWindow::~MainWindow() {
    delete ui;
    delete server_connection;
}

void MainWindow::connectAndJoinRoom() {
    ConnectionDialog dialog(this->server_connection, this);
    dialog.exec();
    if (server_connection->IN_ROOM) {
        ui->notInGameWarning->hide();
    }
}

void MainWindow::showConnectionStatus(ServerConnection::CONNECTION_STATUS connection_status) {
    switch(connection_status) {
    case ServerConnection::NOT_CONNECTED:
        ui->connectionStatus->setText("Not connected to server");
        ui->notInGameWarning->show();
        break;
    case ServerConnection::CONNECTED:
        ui->connectionStatus->setText("You aren't in a game room");
        break;
    case ServerConnection::IN_ROOM:
        ui->connectionStatus->setText("Waiting for oponent");
        ui->notInGameWarning->hide();
        break;
    default:
        break;
    }
}
