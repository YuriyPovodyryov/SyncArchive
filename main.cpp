#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(syncarchive);
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
