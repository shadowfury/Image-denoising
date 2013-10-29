#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    a.addLibraryPath("plugins");
    if (a.arguments().size()==1) w.show();
    else{
        w.setArgs(a.arguments());
        QTimer::singleShot(0, &w, SLOT(onEventLoopStarted()));
    }
    //qDebug() << a.libraryPaths();
    return a.exec();
}
