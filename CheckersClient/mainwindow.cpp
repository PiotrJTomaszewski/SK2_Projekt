#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->actionGameRoom, &QAction::triggered, this, &MainWindow::joinRoom);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::joinRoom() {
    ConnectionDialog dialog(this);
    dialog.exec();
}
