#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Sub2Text");
    QApplication::setApplicationVersion("0.1");
    MainWindow w;
    w.show();
    
    return a.exec();
}
