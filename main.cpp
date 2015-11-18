#include "mainwindow.h"
#include <QApplication>
#include <QString>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setTex(QString::fromLocal8Bit(argv[1]));
    w.show();
    return a.exec();
}
