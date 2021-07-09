#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");

    QApplication a(argc, argv);
    //QTextCodec *codec = QTextCodec::codecForName("CP1251");
    //QTextCodec::setCodecForLocale(codec);

    MainWindow w;
    w.show();

    return a.exec();
}
