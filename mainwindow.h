#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void readSettings();
    void writeSettings();
    QString getArchiveName(QString ArchiveName);
    QString getDateTime();
    void addSlash(QString *Text);

    QMap<QString, QString> TaskIni, Task;
};

#endif // MAINWINDOW_H
