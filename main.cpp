#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.showFullScreen();
    if (!w.Initialize()) {
        return 0;
    }

    w.Start();
    return a.exec();
}
