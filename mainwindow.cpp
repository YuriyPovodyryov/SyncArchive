#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ARROW_RIGHT QChar(8594)


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/img/sync.ico"));
    QString Caption = QString::fromLocal8Bit("????????????? ??????? / ??????? SHA256");
    setWindowTitle(Caption);
    TaskIni.insert(tr("C:/PCOSFirmware/"), tr("H:/PCOSFirmware/"));
    readSettings();
    Count = 0;
    ui->textEdit_Log->setAcceptRichText(true);
}



void MainWindow::hashingFiles(const QString &Dir, const QString &FileNameHash)
{
    QSettings *FileHash = new QSettings(FileNameHash, QSettings::IniFormat);
    FileHash->setIniCodec("CP1251");
    FileHash->remove("");
    findFiles(Dir, Dir, FileHash);
    FileHash->sync();
}



void MainWindow::findFiles(const QDir &Dir, const QDir &FirstDir, QSettings *FileHash)
{
    if (!(Count % 10)) qApp->processEvents();
    Count++;

    QStringList ListFiles = Dir.entryList(QDir::Files);
    foreach (QString Files, ListFiles) {
        QString ArrowRight = tr(" %1 ").arg(ARROW_RIGHT);
        QString Hash(fileCheckSum(Dir.absoluteFilePath(Files), QCryptographicHash::Sha256));
        QString SubPath = Dir.absoluteFilePath(Files).mid(FirstDir.absolutePath().length(), Dir.absoluteFilePath(Files).length());
        ui->textEdit_Log->append(SubPath + ArrowRight + Hash);
        FileHash->setValue(tr("SHA256/%1").arg(SubPath), Hash);
    }

    QStringList ListDir = Dir.entryList(QDir::Dirs);
    foreach (QString SubDir, ListDir) {
        if (SubDir == "." || SubDir == "..") {
            continue;
        }
        findFiles(QDir(Dir.absoluteFilePath(SubDir)), FirstDir, FileHash);
    }
}



QByteArray MainWindow::fileCheckSum(const QString &FileName, QCryptographicHash::Algorithm HashAlgorithm)
{
    QFile F(FileName);
    if (F.open(QFile::ReadOnly)) {
        QCryptographicHash Hash(HashAlgorithm);
        if (Hash.addData(&F)) {
            return(Hash.result().toHex());
        }
    }
    return QByteArray();
}



void MainWindow::addSlash(QString *Text)
{
    QString tmp = *Text;
    if (tmp[tmp.length() - 1] != '/') {
        // ???? ????????? ?????? ?? ?????? ????, ?? ???????? ???
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


    QString Str = QString::fromLocal8Bit("?????? ?????????????:\n------------------------------");
    ui->textEdit_Log->append(Str);


    for (int i = 0; i < Size; i++) {
         From = sett->value(tr("TASK%1/FROM").arg(i)).toString();
         To   = sett->value(tr("TASK%1/TO").arg(i)).toString();
         Task.insert(From, To);

         QString ArrowRight = tr(" %1 ").arg(ARROW_RIGHT);
         QString FileNameRar = QString::fromLocal8Bit("<strong>%1_(????_?????).rar</strong>").arg(getArchiveName(From));
         Str = QString::fromLocal8Bit("?????? %1: ???????????? %2 ").arg(i + 1).arg(From);
         ui->textEdit_Log->append(Str + ArrowRight + To + FileNameRar);

         if (getLastSync(From, To) == "") {
             Str = QString::fromLocal8Bit("?????? ?????????????!");
             ui->textEdit_Log->append(Str);
         } else {
             Str = QString::fromLocal8Bit("????????? ????????????? ???? ");
             ui->textEdit_Log->append(Str + getLastSync(From, To));
         }
    }

    if (!Task.size()) {
        Task = TaskIni;
        writeSettings();
    }
}



//  ???????? ?????? ? ????????? ?????????????
QString MainWindow::getLastSync(const QString &From, const QString &To)
{
    QString LastSync("");
    QDir Dir = To;
    QStringList ListFiles = Dir.entryList(QStringList() << "*.txt", QDir::Files);

    QString MaxDateTime("");
    foreach (QString Files, ListFiles) {
             QStringList Name = Files.split('.');
             QStringList DateTime;
             DateTime = Name[0].split("_");

             if (DateTime[0] == getArchiveName(From)) {
                 QString Id = DateTime[1] + DateTime[2];
                 if (MaxDateTime < Id) MaxDateTime = Id;
             }
    }

    LastSync = MaxDateTime.mid(0, 4) + "-" + MaxDateTime.mid(4, 2) + "-" + MaxDateTime.mid(6, 2) + " " +
               MaxDateTime.mid(8, 2) + ":" + MaxDateTime.mid(10, 2) + ":" + MaxDateTime.mid(12, 2);

    return(LastSync);
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



void MainWindow::on_pushButton_Sync_clicked()
{
    QProcess proc;
    QFile file;
    QString FromRar, ToRar, ToHash;
    QString FileName, FileNameRar, FileNameHash;

    QMapIterator<QString, QString> iter(Task);

    while (iter.hasNext()) {
           iter.next();

           FromRar = iter.key();
           addSlash(&FromRar);

           ToRar   = iter.value();
           addSlash(&ToRar);
           ToHash =  ToRar;

           QString DateTime(getDateTime());

           FileName    = tr("%1_%2").arg(getArchiveName(FromRar)).arg(DateTime);
           FileNameRar = tr("%1.rar").arg(FileName);
           FileNameHash = tr("%1.txt").arg(FileName);
           hashingFiles(FromRar, FileNameHash);

           proc.start(tr("WinRAR.exe a -m5 -r %1 %2").arg(FileName).arg(FromRar));
           proc.waitForFinished(-1);

           FromRar += tr("%1").arg(FileNameRar);
           ToRar   += tr("%1").arg(FileNameRar);
           ToHash  += tr("%1").arg(FileNameHash);

           //   ??????????? ????? ??????
           QString ArrowRight = tr(" %1 ").arg(ARROW_RIGHT);
           if (!file.copy(FileNameRar, ToRar)) {
               QString FailedCopy = QString::fromLocal8Bit("????????? ??????????? ??????!");
               ui->textEdit_Log->append(FailedCopy);
           } else {
               ui->textEdit_Log->append("------------------------------");
               QString Move = QString::fromLocal8Bit("??????????? ");
               ui->textEdit_Log->append(Move + FileNameRar + ArrowRight + ToRar);
               //  ??????? ???????? ?????
               file.remove(FileNameRar);
           }

           //   ??????????? ????? ?????
           if (!file.copy(FileNameHash, ToHash)) {
               qDebug() << FileNameHash << ToHash;
               QString FailedCopy = QString::fromLocal8Bit("????????? ??????????? ?????? ?????!");
               ui->textEdit_Log->append(FailedCopy);
           } else {
               ui->textEdit_Log->append("------------------------------");
               QString Move = QString::fromLocal8Bit("??????????? ");
               ui->textEdit_Log->append(Move + FileNameHash + ArrowRight + ToHash);
               //  ??????? ???????? ?????
               file.remove(FileNameHash);
           }
    }
}



void MainWindow::on_pushButton_Exit_clicked()
{
    qApp->exit();
}
