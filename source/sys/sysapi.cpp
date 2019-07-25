#include "sysapi.h"
#include "mainwindow.h"

static sysPara _sysPara = { 0, 0 };

//void setSysMode( sysPara::eSysMode mode )
//{
//    _sysPara.mMode = mode;
//}
//sysPara::eSysMode sysMode()
//{ return _sysPara.mMode; }

int sysLangIndex()
{
    return MainWindow::langIndex();
}

void sysSetArg( int argc, char **argv )
{
    _sysPara.mArgc = argc;
    _sysPara.mArgv = argv;
}
int sysArgc()
{ return _sysPara.mArgc; }
char** sysArgv()
{ return _sysPara.mArgv; }

bool sysHasArgv( const QString &arg )
{
    for ( int i = 1; i < _sysPara.mArgc; i++ )
    {
        if ( str_is( arg, _sysPara.mArgv[i]) )
        { return true; }
    }

    return false;
}

void sysInfo( const QString &str, eOutHandle oHandle )
{ MainWindow::requestLogout( str, LogStr::log_info, (int)oHandle ); }

void sysInfo( const QString &info, const QString &str2, eOutHandle oHandle )
{
    QString str = QString("%1 %2").arg(info).arg(str2);
    MainWindow::requestLogout( str, LogStr::log_info, (int)oHandle );
}

void sysInfo( const QString &info, const int &val, eOutHandle oHandle )
{
    QString str = QString("%1 : %2").arg(info).arg(val);
    MainWindow::requestLogout( str, LogStr::log_info, (int)oHandle );
}

void sysWarning( const QString &str, eOutHandle oHandle )
{ MainWindow::requestLogout( str, LogStr::log_warning, (int)oHandle ); }

void sysError( const QString &str, eOutHandle oHandle )
{ MainWindow::requestLogout( str, LogStr::log_error, (int)oHandle ); }

void sysError( const QString &info , const int &val, eOutHandle oHandle )
{
    QString str = QString("%1 : %2").arg(info).arg(val);
    MainWindow::requestLogout( str, LogStr::log_error, (int)oHandle );
}

void sysProgress( const QString &info, bool b, int now, int mi, int ma )
{
    MainWindow::requestProgress( info, b, now, mi, ma );
}

void sysPrompt( const QString &info, int lev )
{ MainWindow::requestPrompt( info, lev ); }

void sysShowStatus(const QString &statusInfo)
{
    MainWindow::showStatus(statusInfo);
}

bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

QString readFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("Can't ReadOnly open the file: %1").arg(fileName);
        return "";
    }

    QByteArray array = file.readAll();
    file.close();
    return QString(array);
}

int writeFile(QString fileName, QString text)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << QString("Can't WriteOnly open the file: %1").arg(fileName);
        return -1;
    }

    file.write(text.toUtf8());
    file.close();
    return 0;
}

int assurePath( const QString &path )
{
    //! create path
    QDir dir( path );
    if ( dir.exists() )
    {}
    else
    {
        if ( dir.mkpath( path) )
        {}
        else
        {
            sysError( path + " " + QObject::tr("create fail") );
            return -1;
        }
    }

    return 0;
}
