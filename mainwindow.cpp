#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ARROW_RIGHT QChar(8594)


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString Caption = QString::fromLocal8Bit("������������� �������");
    setWindowTitle(Caption);
    TaskIni.insert(tr("C:/PCOSFirmware/"), tr("H:/PCOSFirmware/"));
    readSettings();

    QProcess proc;
    QFile file;
    QString From, To;
    QString FileName, FileNameRar;

    QMapIterator<QString, QString> iter(Task);

    while (iter.hasNext()) {
           iter.next();

           From = iter.key();
           addSlash(&From);

           To   = iter.value();
           addSlash(&To);

           FileName    = tr("%1_%2").arg(getArchiveName(From)).arg(getDateTime());
           FileNameRar = tr("%1.rar").arg(FileName);
           proc.start(tr("WinRAR.exe a -m5 -r %1 %2").arg(FileName).arg(From));
           proc.waitForFinished(-1);

           From += tr("%1").arg(FileNameRar);
           To   += tr("%1").arg(FileNameRar);

           if (!file.copy(FileNameRar, To)) {
               QString FailedCopy = QString::fromLocal8Bit("��������� ����������� ������!");
               ui->textEdit_Log->append(FailedCopy);
           } else {
               QString ArrowRight = tr(" %1 ").arg(ARROW_RIGHT);
               ui->textEdit_Log->append(FileNameRar + ArrowRight + To);
               //  ������� �������� �����
               file.remove(FileNameRar);
           }
    }

}


void MainWindow::addSlash(QString *Text)
{
    QString tmp = *Text;
    if (tmp[tmp.length() - 1] != '/') {
        // ���� ��������� ������ �� ������ ����, �� �������� ���
        *Text += '/';
    }
}



QString MainWindow::getArchiveName(QString ArchiveName)
{
    QStringList list = ArchiveName.split('/');
    return(list[list.length() - 2]);
}



QString MainWindow::getDateTime()
{
    QString DateTime("");
    QDate date;

    DateTime += tr("%1").arg(date.currentDate().year());

    if (date.currentDate().month() < 10)
       { DateTime += tr("0%1").arg(date.currentDate().month());
       } else
       { DateTime += tr("%1").arg(date.currentDate().month());
       }

    if (date.currentDate().day() < 10)
       { DateTime += tr("0%1_").arg(date.currentDate().day());
       } else
       { DateTime += tr("%1_").arg(date.currentDate().day());
       }

    QTime time;

    if (time.currentTime().hour() < 10) {
        DateTime += tr("0%1").arg(time.currentTime().hour());
    } else {
        DateTime += tr("%1").arg(time.currentTime().hour());
    }

    if (time.currentTime().minute() < 10)
       { DateTime += tr("0%1").arg(time.currentTime().minute());
       } else
       { DateTime += tr("%1").arg(time.currentTime().minute());
       }

    if (time.currentTime().second() < 10)
       { DateTime += tr("0%1").arg(time.currentTime().second());
       } else
       { DateTime += tr("%1").arg(time.currentTime().second());
       }
    return(DateTime);
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::readSettings()
{
    QSettings *sett = new QSettings(tr("sync_archive.ini"), QSettings::IniFormat);
    sett->setIniCodec("CP1251");

    int Size = sett->allKeys().size() / 2;

    QString From, To;
    Task.clear();

    for (int i = 0; i < Size; i++) {
     From = sett->value(tr("TASK%1/FROM").arg(i)).toString();
     To = sett->value(tr("TASK%1/TO").arg(i)).toString();
     Task.insert(From, To);
    }

    if (!Task.size()) {
     Task = TaskIni;
     writeSettings();
    }
}



void MainWindow::writeSettings()
{
    QSettings *sett = new QSettings(tr("sync_archive.ini"), QSettings::IniFormat);
    sett->setIniCodec("CP1251");

    QMapIterator<QString, QString> iter(Task);
    int Counter = 0;
    while (iter.hasNext())
          { iter.next();
            sett->setValue(tr("TASK%1/FROM").arg(Counter), iter.key());
            sett->setValue(tr("TASK%1/TO").arg(Counter), iter.value());
            Counter++;
          }
    sett->sync();
}