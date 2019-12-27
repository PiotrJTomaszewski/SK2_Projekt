#include <QApplication>
#include "mainwindow.h"
#include "serverconnectionobject.h"

int main(int argc, char *argv[]) {
    ServerConnectionObject object;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
