#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    //paths.append("sqldrivers");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
