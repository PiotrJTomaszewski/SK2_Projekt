#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mockserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectAndJoinRoom();

private:
    Ui::MainWindow *ui;
    ServerConnection *server_connection;

private slots:
    void showConnectionStatus(ServerConnection::CONNECTION_STATUS connection_status);
};

#endif // MAINWINDOW_H
