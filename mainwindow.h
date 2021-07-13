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

private slots:
    void on_pushButton_Sync_clicked();

    void on_pushButton_Exit_clicked();

private:
    Ui::MainWindow *ui;
    void readSettings();
    void writeSettings();
    QString getArchiveName(QString ArchiveName);
    QString getDateTime();
    void addSlash(QString *Text);
    void findFiles(const QDir &Dir, const QDir &FirstDir, QSettings *FileHash);
    void hashingFiles(const QString &Dir, const QString &FileNameHash);
    QString getLastSync(const QString &From, const QString &To);
    QByteArray fileCheckSum(const QString &FileName, QCryptographicHash::Algorithm HashAlgorithm);

    QMap<QString, QString> TaskIni, Task;
    int Count;
};

#endif // MAINWINDOW_H
