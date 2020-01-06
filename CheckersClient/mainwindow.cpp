 #include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"
#include "serverconnectionobject.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::connectToServerDialog);
    server_connection = ServerConnectionObject::getServerConnection();
    connect(server_connection, &TcpClient::setConnectionStatusSignal, this, &MainWindow::showConnectionStatus);
    connect(server_connection, &TcpClient::gameStatusSignal, this, &MainWindow::showGameStatus);
    connect(server_connection, &TcpClient::gameErrorSignal, this, &MainWindow::showGameError);
    connect(server_connection, &TcpClient::debugSignal, this, &MainWindow::debugSlot);
    if (QCoreApplication::arguments().contains("debug")) ui->debugBox->show();
    else ui->debugBox->hide();
    ui->errorMessage->hide();

}

MainWindow::~MainWindow() {
    delete ui;
    delete server_connection;
}

void MainWindow::connectToServerDialog() {
    ConnectionDialog dialog(this);
    dialog.exec();
    if (this->server_connection->getConnectionStatus() != TcpClient::NOT_CONNECTED && this->server_connection->getConnectionStatus() != TcpClient::CONNECTION_ERROR) {
        ui->notConnectedWarning->hide();
    }
}

void MainWindow::showConnectionStatus(TcpClient::CONNECTION_STATUS connection_status) {
    switch (connection_status) {
    case TcpClient::NOT_CONNECTED:
        ui->connectionStatus->setText("Not connected to server");
        ui->notConnectedWarning->show();
        break;
    case TcpClient::IN_ROOM:
        ui->connectionStatus->setText("Waiting for an opponent");
        ui->notConnectedWarning->hide();
        break;
    case TcpClient::IN_GAME_LIGHT:
        ui->connectionStatus->setText("In game. Playing light");
        ui->notConnectedWarning->hide();
        break;
    case TcpClient::IN_GAME_DARK:
        ui->connectionStatus->setText("In game. Playing dark");
        ui->notConnectedWarning->hide();
        break;
    default:
        break;
    }
}

void MainWindow::showGameStatus(GLOBAL::GAME_STATUS status) {
    QMessageBox msg_box;
    switch (status) {
    case GLOBAL::GAME_STATUS::TURN_DARK:
        ui->gameStatus->setText("Dark player turn");
        break;
    case GLOBAL::GAME_STATUS::TURN_LIGHT:
        ui->gameStatus->setText("Light player turn");
        break;
    case GLOBAL::GAME_STATUS::LIGHT_WON:
        ui->gameStatus->setText("Light player won");
        msg_box.setText("Light player has won the game.");
        msg_box.exec();
        break;
    case GLOBAL::GAME_STATUS::DARK_WON:
        ui->gameStatus->setText("Dark player won");
        msg_box.setText("Dark player has won the game.");
        msg_box.exec();
        break;
    case GLOBAL::GAME_STATUS::TIE:
        ui->gameStatus->setText("A tie");
        msg_box.setText("No more possible moves. The game has ended with a tie.");
        msg_box.exec();
        break;
    case GLOBAL::GAME_STATUS::OPPONENT_LEFT:
        ui->gameStatus->setText("Opponent left");
        msg_box.setText("Your opponent has left the game. You will be now disconnected from the server.");
        msg_box.exec();
        break;
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

void MainWindow::debugSlot(QString message) {
    ui->debugBox->append(message + '\n');
}
