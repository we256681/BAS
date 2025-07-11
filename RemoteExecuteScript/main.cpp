#include "remote.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QDir>
#include <QLockFile>
#include "addavexclusion.h"


void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    QString txt;
    QString strReplaced = str;
    strReplaced = strReplaced.replace("https://bablosoft.com/", "");
    strReplaced = strReplaced.replace("bablosoft.com", "");
    strReplaced = strReplaced.replace("bablosoft", "");
    QString datestring = QTime::currentTime().toString("hh:mm:ss");
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug %1: %2").arg(datestring).arg(strReplaced);
        break;
    case QtWarningMsg:
        txt = QString("Warning %1: %2").arg(datestring).arg(strReplaced);
    break;
    case QtCriticalMsg:
        txt = QString("Critical %1: %2").arg(datestring).arg(strReplaced);
    break;
    case QtFatalMsg:
        txt = QString("Fatal %1: %2").arg(datestring).arg(strReplaced);
        abort();
    }
    QFile outFile("data/debug.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    outFile.close();
    QTextStream out(stdout);
    out<<strReplaced<<endl;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageHandler);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QDir::setCurrent(a.applicationDirPath());

    QDir("data").mkpath(".");

    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));

    qApp->setStyle(QStyleFactory::create("fusion"));

    QFont font = qApp->font();
    font.setPixelSize(13);
    qApp->setFont(font);

    QPalette palette;
    qApp->setPalette(palette);


    /*{
        AddAVExclusion e;
        e.Run();
    }*/

    Remote u;
    if(u.Start(a.arguments()))
        return a.exec();
    else
        return 0;
}

