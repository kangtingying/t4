#include <float.h>
#include "t4.h"
#include <QTcpSocket>
#include <QHostAddress>
#include "MegaGateway.h"
#include <QAbstractSocket>
#include "../../../include/mystd.h"

#include "../../plugin/xpluginworkingthread.h"

#include "actiontable.h"
#include "t4oppanel.h"

bool MRX_T4::isOnLine()
{
    return mbAxisPwr && mbMctEn;
}

int MRX_T4::open()
{
    int ret;

    int localVi = 0;
    ret = _open( localVi );
    if ( ret == 0 )
    {
        mVi = localVi;

        //! changed
        emit_setting_changed( XPage::e_setting_opened, true );

        emit_setting_changed( XPage::e_setting_user_role, QVariant() );

        //! note work role
        setWorkingRole( working_normal );
        emit_setting_changed( XPage::e_setting_work_role, QVariant((int)working_normal) );

        //! change setting
        //! update the angle
        float zeroAngles[4];
        ret = mrgGetRobotHomeAngle( self_robot_var(), zeroAngles );
        if ( ret < 0 )
        { sysError( tr("Zero Angle update fail") ); }
        else
        {
            for ( int i = 0; i < ret && i < T4Para::_axis_cnt; i++ )
            { mAxisZero[ i ] = zeroAngles[ i ]; }
        }

        //! update the current
        for ( int i = 0; i < T4Para::_axis_cnt; i++ )
        {
            ret = mrgMRQMotorCurrent_Query( self_device_var(), i, mAxisCurrents + i );
            if ( ret != 0 )
            {
                sysError( tr("Current read fail") );
                break;
            }
        }

        //! \note
        //! \todo
        //! config the data
        int dataOnOff;
        if ( sysHasArgv("-noupdate") )
        { dataOnOff = 0; }
        else
        { dataOnOff = 1; }

        //! \note no sg/se
        {
            for ( int i = 0; i < T4Para::_axis_cnt; i++ )
            {
               ret = mrgMRQReportState( self_device_var(), i, 0, 0 );
               if ( ret != 0 )
               {
                   sysError( tr("Data report state fail") );
                   break;
               }
            }
        }

        emit_load();

        //! update ui
        updateUis();

        //! startup
        _startupProc();
    }
    else
    {
        emit_setting_changed( XPage::e_setting_opened, false );

        //! note work role
        setWorkingRole( working_disconnected );
        emit_setting_changed( XPage::e_setting_work_role, QVariant((int)working_disconnected) );
    }

    return ret;
}

#define local_robot_var()     (ViSession)vi, robotHandle()
#define local_device_var()    (ViSession)vi,deviceHandle()
int MRX_T4::_open( int &vi )
{
    //! init setting
    vi = 0;

    //! local vi
    vi = mrgOpenGateWay(1, mAddr.toLatin1().data(), 2000 );
    if ( vi > 0 )
    {
        //! \note delay setting changed
//        mVi = vi;
    }
    else
    {
        return -1;
    }

    int ret;
    do
    {
        //! get robot handle
        int names[16];

        //! \todo names overflow
        ret = mrgGetRobotName( vi, names );
        if ( ret > 16 || ret < 1 )
        { ret = -1; break; }

        mRobotHandle = names[0];
        sysInfo( tr("Robot"), mRobotHandle );

        //! device handle
        int deviceHandles[16];
        ret = mrgGetRobotDevice( local_robot_var(), deviceHandles );
        if ( ret > 16 || ret < 1 )
        { ret = -1; break; }
        mDeviceHandle = deviceHandles[0];
        sysInfo( tr("Device"), mDeviceHandle );

        //! \todo get mrx-t4 ver

        //! get mrh info
        {
            char vers[128];

            //! MegaRobo Technologies,MRH-T-29-N,MRHT10000519700001,00.00.02.07
            ret = mrgGateWayIDNQuery( vi, vers );
            if ( ret != 0 )
            { ret = -1; break; }

            QStringList infoList = QString(vers).split(',', QString::SkipEmptyParts );
            if ( infoList.size() < 4 )
            { ret = -1; break; }

            setMRHType( infoList.at(1) );

            setMRHSN( infoList.at(2) );

            ret = mrgSysGetProjectSoftVersion( vi, vers );
            if ( ret != 0 )
            { ret = -1; break; }
            setMRHVer( vers );

            ret = mrgSysGetBackBoardSoftVersion( vi, vers );
            if ( ret != 0 )
            { ret = -1; break; }
            setMRHBBSw( vers );

            ret = mrgSysGetBackBoardHardVersion( vi, vers );
            if ( ret != 0 )
            { ret = -1; break; }
            setMRHBBHw( vers );
        }

        //! get mrq info
        {
            char buf[128];
            ret = mrgGetDeviceInfo( vi, mDeviceHandle, buf);
            if(ret != 0)
            { ret = -1; break; }

            QString t( buf );
            QStringList tList = t.split(":");
            if ( tList.size() < 5 )
            { ret = -2; break; }

            setSN_MRQ( tList.at( 0 ) );
            setFirmWareHard_MRQ( tList.at(1) );
            setSoftVer_MRQ( tList.at(2) );
            setFirmWareBoot_MRQ( tList.at( 3 ) );
            setFirmWareFpga_MRQ( tList.at( 4) );

            ret = mrgGetDeviceType( vi, mDeviceHandle, buf);
            if(ret!=0)
            { ret = -1; break; }
            setType_MRQ( buf );

            mrgGetDeviceSoftVersion( vi, mDeviceHandle, buf );
            if ( ret != 0 )
            { ret = -1; break; }
            setMrqVer( buf );

        }

        //! connect socket
        QStringList secList = mAddr.split("::");
        if ( secList.size() < 3 )
        { ret = -1; break; }

        m_pExceptionSocket = new QTcpSocket();
        if ( NULL == m_pExceptionSocket )
        { ret = -1; break; }

        connect( m_pExceptionSocket, SIGNAL(readyRead()), this, SLOT(slot_exception_arrived()));
        connect( m_pExceptionSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                 this, SLOT(slotSocketError(QAbstractSocket::SocketError)) );

        //! bind success
        QString pureIp = secList.at(1);
        m_pExceptionSocket->connectToHost( pureIp, MEGAROBO_TCP_EXCEPTION_PORT );
        bool b = m_pExceptionSocket->waitForConnected( 200 );
        if ( b )
        {
            //connect( m_pExceptionSocket, SIGNAL(readyRead()), this, SLOT(slot_exception_arrived()));
        }
        //! \note connect fail, still use the device
        else
        {
            sysPrompt( tr("Connect exception port fail") );
            sysError( tr("Connect exception port fail") );
        }

        ret = 0;
    }while( false );

    if ( ret != 0 )
    {
        mrgCloseGateWay( vi);
    }

    return ret;
}
void MRX_T4::close()
{
    lockWorking();
        if ( mVi > 0 )
        {
            mrgCloseGateWay( mVi);
            mVi = -1;
        }
        else
        {   }

        //! valid exception
        if ( m_pExceptionSocket != NULL )
        {
            if ( m_pExceptionSocket->isOpen() )
            { m_pExceptionSocket->close(); }
            delete m_pExceptionSocket;
            m_pExceptionSocket = NULL;
        }

    unlockWorking();

    emit_setting_changed( XPage::e_setting_opened, false );

    //! note work role
    setWorkingRole( working_disconnected );
    emit_setting_changed( XPage::e_setting_work_role, QVariant((int)(working_disconnected)) );
}

void MRX_T4::startup()
{
    startupProc();
}

int MRX_T4::stop()
{
    if ( isOpened() )
    {}
    else
    { return 0; }

    //! stop working
    m_pMissionWorking->requestInterruption();
    m_pMissionWorking->wait();

    int ret = mrgRobotStop( mVi, mRobotHandle, wave_table );
    if ( ret != 0 )
    { sysError( tr("Stop fail") );}

    return 0;
}

int MRX_T4::fStop()
{
    if (isOpened())
    {}
    else
    { return 0; }

    //! stop working
    m_pMissionWorking->requestInterruption();
    m_pMissionWorking->wait();

    int ret = mrgSysSetEmergencyStopState( mVi, 1 );
    if ( ret != 0 )
    { sysError( tr("Stop fail") );}

    //! stop off
    mrgSysSetEmergencyStopState( mVi, 0 );

    return 0;
}

void MRX_T4::rst()
{
    rstRecordTable();

    rstErrorMgrTable();

    T4Para::rst();

    emit_setting_changed( XPage::e_setting_update_ui, true );

    //! update ui
    slot_save_setting();
}

void MRX_T4::home()
{
    Q_ASSERT( NULL != m_pOpPanel );
    m_pOpPanel->home();
}
void MRX_T4::fold()
{
    Q_ASSERT( NULL != m_pOpPanel );
    m_pOpPanel->fold();
}

void MRX_T4::reboot()
{
    //! \todo the the bg thread
    //!
    int ret = mrgSystemRunCmd(mVi,(char*)"reboot", 0);
    if(ret != 0){
        sysError( tr("Reboot fail") );
    }

    close();
}
void MRX_T4::powerOff()
{
    //! \todo the the bg thread
    int ret = mrgSystemRunCmd(mVi,(char*)"poweroff",0);
    if(ret != 0){
        sysError( tr("Poweroff fail") );
    }

    close();
}

int MRX_T4::upload()
{
    attachBgWorking( (XPlugin::bgProc)(&MRX_T4::uploadProc) );
    return 0;
}
int MRX_T4::download()
{
    attachBgWorking( (XPlugin::bgProc)(&MRX_T4::downloadProc) );
    return 0;
}
int MRX_T4::diff()
{
    attachBgWorking( (XPlugin::bgProc)(&MRX_T4::diffProc) );
    return 0;
}

int MRX_T4::jointDir( int jid )
{
    //! by the terminal type
    if ( jid != 4 )
    { return 1; }

    if ( mTerminalType != e_terminal_a5 )
    { return 1; }

    return -1;
}

int MRX_T4::startDemo( int id )
{
    int ret;

    QString strPath = demoPath() + "/" + QString::number( id );

    //! load the action table
    ret = m_pRecordView->requestLoad( strPath, record_file_name );
    if ( ret != 0 )
    { return ret; }

    //! load the debug.xml
    ret = m_pOpPanel->requestLoad_debug( strPath, debug_file_name );
    if ( ret != 0 )
    {
        return ret;
    }

    //! start
    mrx_t4::OpEvent *demoEvent = new mrx_t4::OpEvent( mrx_t4::OpEvent::demo_start );
    if ( NULL != demoEvent )
    { qApp->postEvent( m_pOpPanel, demoEvent ); }

    return ret;
}
int MRX_T4::stopDemo( int id )
{


    return 0;
}

int MRX_T4::onXEvent( XEvent *pEvent )
{
    //! \todo stop/estop/
    if ( pEvent->type() == e_x_update_ui )
    {
        xevent_updateui( pEvent );
        return 0;
    }

    return XPlugin::onXEvent( pEvent );
}

//! 0xFF31,WARNING,2019/07/22_11:16:50,MRQ,1,ENCODER3
void MRX_T4::onDeviceException( QVariant &var )
{
    //! prompt
    if ( var.isValid() )
    {
        //! try decode the info
        QStringList itemList = var.toString().split(",", QString::SkipEmptyParts );

        QString promptString;
        int errLev = 2;
        bool bOk;

        do
        {
            //! default
            promptString = var.toString();

            if ( itemList.size() > 0 )
            {
                //! try get the index
                int errCode;
                errCode = itemList.at( 0 ).toInt( &bOk, 16 );
                if ( bOk )
                {
                    QVariant var;
                    var = mErrorConfigTable.errorBrief( errCode, sysLangIndex() == e_lang_cn ? 0 : 1 );
                    if ( var.isValid() )
                    {  promptString = var.toString(); }
                    else
                    {
                        //! invlid var
                    }
                }
                else
                {   //! invalid err code
                }
            }

            //! level
            if ( itemList.size() > 1 )
            {
                if ( str_is( itemList.at(1), "WARNING") )
                { errLev = 1; }
                else if ( str_is( itemList.at(1), "ERROR") )
                { errLev = 2; }
                else
                { errLev = 0; }
            }

        }while( 0 );

        sysPrompt( promptString, errLev );
    }
    else
    {
        sysWarning( tr("Invalid exception info") );
    }

    //! \note post refresh for combine the logs
    mRefTimer.start( 5000 );
}

void MRX_T4::updateUis()
{
    XPage* pPage;
    foreach( QWidget *pWig, mPluginWidgets )
    {
        Q_ASSERT( NULL != pWig );
        pPage = dynamic_cast<XPage*>( pWig );
        if ( NULL != pPage )
        {
            do
            {
                XEvent *pEvent = new XEvent( MRX_T4::e_x_update_ui, QVariant::fromValue( (QObject*)pWig) );
                if ( NULL == pEvent )
                { logDbg(); break; }

                qApp->postEvent( this, pEvent );

            }while( 0 );
        }
    }
}

void MRX_T4::xevent_updateui( XEvent *pEvent )
{logDbg();
    //! cast to page
    QObject *pObj;
    pObj = pEvent->mVar1.value<QObject*>();
    if ( NULL == pObj )
    { logDbg(); return; }

    XPage *pPage = dynamic_cast<XPage*>(pObj);
    if ( NULL == pPage )
    { logDbg(); return; }
logDbg()<<pObj->objectName();
    pPage->updateUi();logDbg()<<pObj->objectName();
}

int MRX_T4::_uploadProc()
{
    //! foeach pages
    int ret;
    XPage *pPage;
    int i = 0;
    foreach( QWidget *pWig, mPluginWidgets )
    {
        sysProgress( "Uploading...", true, i++ );

        Q_ASSERT( NULL != pWig );
        pPage = dynamic_cast<XPage*>( pWig );
        if ( NULL != pPage )
        {logDbg()<<pPage->objectName();
            //! upload
            ret = pPage->upload();
            if ( ret != 0 )
            {
                logDbg()<<pPage->objectName()<<ret;
                sysError( tr("Upload fail") );
                return 0;
            }

            //! update data
            if ( ret == 0 )
            {
                do
                {
                    XEvent *pEvent = new XEvent( MRX_T4::e_x_update_ui, QVariant::fromValue( (QObject*)pWig) );
                    if ( NULL == pEvent )
                    { logDbg(); break; }

                    qApp->postEvent( this, pEvent );

                }while( 0 );
            }
        }
    }

    return 0;
}

int MRX_T4::uploadProc()
{
    int ret;

    sysProgress( "Uploading...", true, 0 );

    ret = _uploadProc();

    sysProgress( "Uploading...", true, 50 );

    //! save
    if ( ret == 0 )
    {
        slot_save_setting();

        //! start up
        XEvent *pEvent = new XEvent( XEvent::e_xevent_startup );
        if ( NULL != pEvent )
        {
            qApp->postEvent( this, pEvent );
        }

        sysPrompt( tr("Upload success"), 0 );
    }
    //! upload fail
    else
    {
        sysPrompt( tr("Upload fail") );
    }

    sysProgress( "Uploading...", false );

    return 0;
}

int MRX_T4::_downloadProc()
{
    int ret;
    XPage *pPage;
    int i = 0;
    foreach( QWidget *pWig, mPluginWidgets )
    {
        sysProgress( "Downloading...", true, i++ );

        Q_ASSERT( NULL != pWig );
        pPage = dynamic_cast<XPage*>( pWig );
        if ( NULL != pPage )
        {
            ret = pPage->download();
            if ( ret != 0 )
            {
                logDbg()<<pPage->objectName();
                sysError( tr("Download fail") );
                return ret;
            }
        }
    }
    return 0;
}
int MRX_T4::downloadProc()
{
    int ret;

    sysProgress( "Downloading...", true, 0 );

    ret = _downloadProc();

    sysProgress( "Downloading...", false );

    if ( ret != 0 )
    {
        sysPrompt( tr("Download fail") );
    }
    else
    {
        sysPrompt( tr("Download success"), 0 );
    }

    return 0;
}

int MRX_T4::_diffProc()
{ return 0; }
int MRX_T4::diffProc()
{
    //! \todo
    return 0;
}

int MRX_T4::onStop( QVariant var )
{
    return 0;
}

int MRX_T4::_startupProc()
{
    //! foeach pages
    XPage *pPage;
    int i = 0;
    foreach( QWidget *pWig, mPluginWidgets )
    {
        Q_ASSERT( NULL != pWig );
        pPage = dynamic_cast<XPage*>( pWig );
        if ( NULL != pPage )
        {
            //! startup
            pPage->startup();
        }
    }

    return 0;
}

int MRX_T4::startupProc()
{
    sysProgress( "Startup...", true, 0 );

    _startupProc();

    sysProgress( "Startup...", false );

    return 0;
}

//int MRX_T4::onHoming( QVariant var )
//{
//    _check_connect_ret( this, -1 );

//    int ret;

//    ret = mrgRobotGoHome( robot_var(),
//                          pRobo->mHomeTimeout*1000 );

//    return ret;
//}

//int MRX_T4::onFolding( QVariant var )
//{
//    _check_connect_ret( this, -1 );

//    int ret;

//    ret = mrgGetRobotFold( robot_var(),
//                           wave_table,
//                           pRobo->mPackagesAxes[0],
//                           pRobo->mPackagesAxes[1],
//                           pRobo->mPackagesAxes[2],
//                           pRobo->mPackagesAxes[3]
//                           );
//    return ret;
//}

int MRX_T4::robotHandle()
{ return mRobotHandle; }

int MRX_T4::deviceHandle()
{ return mDeviceHandle; }

//!
int MRX_T4::currentRecordIndex()
{
    Q_ASSERT( NULL != m_pRecordView );

    return m_pRecordView->currentIndex();
}

double MRX_T4::eulaDistance( double x, double y, double z,
                            double x1, double y1, double z1 )
{
    //! calc the distance
    double dist = sqrt( pow(  x - x1, 2) +
                       pow(  y - y1, 2) +
                       pow(  z - z1, 2)
                       );
    return dist;
}

int MRX_T4::relMove( QString para,
             double x, double y, double z,
             double pw, double h,
             double v, bool bLine )
{logDbg();
    self_check_connect_ret( -1 );

    int ret;

    //! calc the distance
    double dist = eulaDistance( 0,0,0,
                                x, y, z
                                );
    if ( dist < distance_error )
    { return 0; }

    Q_ASSERT( v > 0 );
    float t = qAbs( dist ) / v;

    logDbg()<<x<<y<<z<<t<<guess_dist_time_ms( t, dist );
    if ( bLine )
    {
        ret = mrgRobotRelMoveL( self_robot_var(),
                          wave_table,
                          x,
                          y,
                          z,
                          t,
                          guess_dist_time_ms( t, dist )
                          );
    }
    else
    {
        ret = mrgRobotRelMove( self_robot_var(),
                               wave_table,
                               x,
                               y,
                               z,
                               t,
                               guess_dist_time_ms( t, dist )
                               );
    }

    return ret;
}

int MRX_T4::absMove( QString para,
             double x, double y, double z,
             double pw, double h,
             double v, bool bLine )
{
    self_check_connect_ret( -1 );

    //! get remote pos
    int ret;
    float xn,yn,zn;
    ret = mrgGetRobotCurrentPosition( self_robot_var(),
                                      &xn, &yn, &zn );
    if ( ret != 0 )
    {
        sysError( tr("Position read fail") );
        return -1;
    }

    //! calc the distance
    double dist = eulaDistance( xn,yn,zn,
                                x, y, z
                                );

    //! \note distance error
    if ( dist < distance_error )
    { return 0; }

    Q_ASSERT( v > 0 );

    //! guess t and a
    float t = qAbs( dist ) / v;
    float a = 16*qAbs( dist )/(3*t*t);

    //! tune the t for a
    float amax = 16*mMaxTerminalSpeed / 3 * mAutoAcc / 100;

    if( a > amax ){
        t = sqrt( 16*qAbs( dist )/(3*amax) );
        sysInfo( tr("Slow the velocity") );
    }else{
        //! no change
    }

    logDbg()<<x<<y<<z<<t<<guess_dist_time_ms( t, dist ) <<dist<<qAbs( dist ) / v<<v << a;

    if ( bLine )
    {
        ret = mrgRobotMoveL( self_robot_var(),
                             wave_table,
                             x,
                             y,
                             z,
                             t,
                             guess_dist_time_ms( t, dist )
                             );
    }
    else
    {
        ret = mrgRobotMove( self_robot_var(),
                               wave_table,
                               x,
                               y,
                               z,
                               t,
                               guess_dist_time_ms( t, dist )
                               );
    }

    return ret;
}

void MRX_T4::setAbsMarker( double x, double y, double z,
                   double pw, double h,
                   double v, bool bLine )
{
    m_pMarkerItem->bValid = true;

    m_pMarkerItem->x = x;
    m_pMarkerItem->y = y;
    m_pMarkerItem->z = z;

    m_pMarkerItem->pw = pw;
    m_pMarkerItem->h = h;

    m_pMarkerItem->v = v;
    m_pMarkerItem->bLine = bLine;

}
void MRX_T4::setAbsMarker( SequenceItem &item )
{ *m_pMarkerItem = item; }
SequenceItem *MRX_T4::absMarker()
{ return m_pMarkerItem; }
