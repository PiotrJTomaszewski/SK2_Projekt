#include <QApplication>
#include "mainwindow.h"
#include "serverconnectionobject.h"

int main(int argc, char *argv[]) {
    // Create an instance of tcp client inside the ServerConnectionObject class
    ServerConnectionObject object;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
