#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Sub2Text");
    QApplication::setApplicationVersion("0.1");

    /// system locale for messageboxes ///
       QTranslator qtTranslator;
       qtTranslator.load("qt_" + QLocale::system().name(),
                             QLibraryInfo::location(QLibraryInfo::TranslationsPath));
       a.installTranslator(&qtTranslator);

       /// Programm language ///
       QTranslator prTranslator;
       prTranslator.load(":/lng/" + QLocale::system().name());
       a.installTranslator(&prTranslator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
