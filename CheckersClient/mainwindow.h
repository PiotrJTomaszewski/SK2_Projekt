#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serverconnection.h"
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
};

#endif // MAINWINDOW_H
