#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpClient.h"

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
    TcpClient *server_connection;

private slots:
    void showConnectionStatus(TcpClient::CONNECTION_STATUS connection_status);
    void showGameStatus(GLOBAL::GAME_STATUS status);
    void showGameError(GLOBAL::GAME_ERROR error);
};

#endif // MAINWINDOW_H
