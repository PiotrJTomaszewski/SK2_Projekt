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
    connect(server_connection, &TcpClient::gameStatusSignal, this, &MainWindow::showGameStatus);
    connect(server_connection, &TcpClient::gameErrorSignal, this, &MainWindow::showGameError);

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
    switch (connection_status) {
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

void MainWindow::showGameStatus(GLOBAL::GAME_STATUS status) {
    switch (status) {
    case GLOBAL::GAME_STATUS::TURN_DARK:
        ui->gameStatus->setText("Dark player turn");
        break;
    case GLOBAL::GAME_STATUS::TURN_LIGHT:
        ui->gameStatus->setText("Light player turn");
    }
}

void MainWindow::showGameError(GLOBAL::GAME_ERROR error) {
    switch (error) {
    case GLOBAL::GAME_ERROR::NO_ERROR:
        ui->errorMessage->hide();
        break;
    case GLOBAL::GAME_ERROR::ERROR_NOT_KING:
        ui->errorMessage->setText("Error!\nOnly king can go backwards!");
        ui->errorMessage->show();
        break;
    case GLOBAL::GAME_ERROR::ERROR_FIELD_TAKEN:
        ui->errorMessage->setText("Error!\nThis field is taken!");
        ui->errorMessage->show();
        break;
    case GLOBAL::GAME_ERROR::ERROR_ILLEGAL_MOVE:
        ui->errorMessage->setText("Error!\nThis move is illegal!");
        ui->errorMessage->show();
        break;
    case GLOBAL::GAME_ERROR::ERROR_NOT_YOUR_TURN:
        ui->errorMessage->setText("Error!\nIt's not your turn!");
        ui->errorMessage->show();
        break;
    case GLOBAL::GAME_ERROR::ERROR_CANT_MOVE_PIECE:
        ui->errorMessage->setText("Error!\nYou can't move this piece!");
        ui->errorMessage->show();
        break;
    }
}
