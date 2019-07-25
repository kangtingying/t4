#include <QDir>
#include <QTcpSocket>
#include "t4.h"

#include "t4oppanel.h"

int MRX_T4::serialOut(QXmlStreamWriter &writer)
{
    return T4Para::serialOut( writer );
}
int MRX_T4::serialIn(QXmlStreamReader &reader)
{
    return T4Para::serialIn( reader );
}

void MRX_T4::slot_save_pw()
{
    int ret;

    QString path = selfPath();

    //! save file
    ret = savePw( path, QString(password_file_name) );
    if ( ret != 0 )
    {
        sysError( tr("Password save fail") );
    }
}

void MRX_T4::slot_save_setting()
{
    int ret;

    QString path = selfPath();

    //! save file
    QString setupfileName = path + "/" + config_file_name;
    ret = save( setupfileName );
    if ( ret != 0 )
    {
        sysError( tr("Config save fail") );
    }

    logDbg()<<setupfileName;
}

void MRX_T4::slot_load_setting()
{
    int ret;

    QString path = selfPath();

    //! save file
    QString setupfileName = path + "/" + config_file_name;
    ret = load( setupfileName );
    if ( ret != 0 )
    {
        sysError( tr("Config load fail") );
    }

    XPlugin::slot_load_setting();

    //! load pw
    ret = loadPw( path, QString(password_file_name) );
    if ( ret != 0 )
    {
        sysError( tr("Password load fail") );

        //! recover
        rstPw( user_user );
        rstPw( user_admin );
    }
    else
    {}
}

void MRX_T4::slot_exception_arrived()
{
    lockWorking();
        //! proc the event
        QByteArray theData;
        if ( m_pExceptionSocket->canReadLine() )
        {
            //"0xFF31,WARNING,2019/07/22_11:16:50,MRQ,1,ENCODER3
            theData = m_pExceptionSocket->readAll();
            logDbg()<<theData;
        }

    unlockWorking();

    //! now try parse
    QStringList exceptionList = QString( theData).split( '\n', QString::SkipEmptyParts );
    QString item;
    bool bOk;

    for ( int i = 0; i < exceptionList.size(); i++ )
    {
        item = exceptionList.at(i);

        XEvent *pEvent = new XEvent( XEvent::e_xevent_device_exception, item );
        if ( NULL != pEvent )
        {
            qApp->postEvent( this, pEvent );
        }
        else
        {
            sysError( tr("New event fail") );
        }
    }

}

void MRX_T4::slotSocketError(QAbstractSocket::SocketError e)
{
    logDbg() <<"Socket error: "<< e
    << m_pExceptionSocket->errorString();
}

void MRX_T4::slot_dia_refresh_timeout()
{logDbg();
    if ( NULL != m_pOpPanel )
    { m_pOpPanel->postRefreshDiagnosisInfo(); }
}

