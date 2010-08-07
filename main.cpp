#include <QtGui/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load(a.applicationDirPath() + "/mosaicit_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    MainWindow w(a.applicationDirPath());
    w.show();
    return a.exec();
}
