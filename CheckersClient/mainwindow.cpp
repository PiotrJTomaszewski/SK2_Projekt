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
}
