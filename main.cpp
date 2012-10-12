#include <QtGui/QApplication>
#include "mainwindow.h"
//#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.addLibraryPath("plugins");
    w.show();
    //qDebug() << a.libraryPaths();





    return a.exec();
}
