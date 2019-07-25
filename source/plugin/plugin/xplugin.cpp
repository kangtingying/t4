#include "xplugin.h"
#include "../../../include/mystd.h"
#include "xpluginworkingthread.h"

#include "MegaGateway.h"

#define log_file_path   homePath() + "/log"
#define log_file_name   log_file_path + "/" + QDateTime::currentDateTime().toString("yyyy_M_d")+".dat"

XPlugin::XPlugin( QObject *parent ) : XPluginIntf( parent )
{
    m_pRootWidgetItem = NULL;
    m_pDock = NULL;
    m_pPref = NULL;
    m_pPanelWidget = NULL;
    m_pViewObj = NULL;
    m_pLog = new MLog( &mLogStream, QIODevice::WriteOnly );
    Q_ASSERT( NULL != m_pLog );

    //! updateing
    m_pUpdateWorking = new XPluginWorkingThread( this );
    m_pUpdateWorking->attachMutex( &mUpdateMutex );
    m_pUpdateWorking->start();
    m_pUpdateWorking->setTick( 1000 );

    m_pMissionWorking = new XPluginWorkingThread( this );
    m_pMissionWorking->attachMutex( &mMissionMutex );
    m_pMissionWorking->start();

    m_pEmergencyWorking = new XPluginWorkingThread( this );
    m_pEmergencyWorking->attachMutex( &mEmergMutex );
    m_pEmergencyWorking->start();

    m_pBgWorking = new XPluginBgThread( this, this );
    m_pBgWorking->start();

    //! attach share mutex
    m_pUpdateWorking->attachShareMutex( &mShareMutex );
    m_pBgWorking->attachShareMutex( &mShareMutex );

//    m_pMissionWorking->attachShareMutex( &mShareMutex );

    connect( this, SIGNAL(signal_request_load()),
             this, SLOT(slot_load_setting()) );

    connect( this, SIGNAL(signal_setting_changed(XSetting)),
             this, SLOT(slot_plugin_setting_changed(XSetting)));

    //! signal
    connect( this, SIGNAL(signal_api_operate(QObject*,bool)),
             m_pUpdateWorking, SLOT(slot_api_operate(QObject*,bool)));

}

XPlugin::~XPlugin()
{
    m_pUpdateWorking->requestInterruption();
    m_pUpdateWorking->wait();

    m_pMissionWorking->requestInterruption();
    m_pMissionWorking->wait();

    m_pEmergencyWorking->requestInterruption();
    m_pEmergencyWorking->wait();

    m_pBgWorking->requestInterruption();
    m_pBgWorking->wait();

    //! flush file
    flush_log();
    delete m_pLog;

    //! \note only close, delete by the parent
    foreach( QWidget *pWidget, mPluginWidgets )
    {
        pWidget->close();
//        delete pWidget;
    }

    close();
}

QTreeWidgetItem* XPlugin::createPrefPages( QStackedWidget *stack )
{ return NULL; }
QWidget *XPlugin::createOpsPanel( QAbstractListModel *pModel,
                                  QWidget *parent )
{ return NULL; }

void XPlugin::setActive( )
{
    Q_ASSERT( NULL != m_pDock && NULL != m_pPanelWidget );

    m_pDock->setWidget( m_pPanelWidget );
}
void XPlugin::retranslateUi()
{

}
void XPlugin::rstErrorMgrTable()
{}
void XPlugin::rstRecordTable()
{}

void XPlugin::onOperateAble( bool b )
{
    emit_setting_changed( XPage::e_setting_op_able, b );
}

void XPlugin::onOnLine( bool b )
{ emit_setting_changed( XPage::e_setting_online, b ); }

int XPlugin::save( const QString &fileName )
{
    QByteArray theAry;

    QXmlStreamWriter writer( &theAry );

    writer.writeStartDocument();

    writer.writeStartElement("config");

        writer.writeStartElement("intf");
            XPluginIntf::save( writer );
        writer.writeEndElement();

        writer.writeStartElement("setup");
            serialOut( writer );
        writer.writeEndElement();

    writer.writeEndElement();

    writer.writeEndDocument();

    bool bOk;
    QString path, name;
    bOk = splitPathName( fileName, path, name );
    if ( !bOk )
    { return -1; }
logDbg()<<path<<name<<theAry.length();
    int ret;
    ret = mrgStorageWriteFile( mVi,
                                   0,
                                   path.toLatin1().data(),
                                   name.toLatin1().data(),
                                   (quint8*)theAry.data(),
                                   theAry.length()
                                   );
    logDbg()<<ret;
    return ret;
}
int XPlugin::load( const QString &fileName )
{
    bool bOk;
    QString path, name;
    bOk = splitPathName( fileName, path, name );
    if ( !bOk )
    { return -1; }

    //! \todo need the file size
    QByteArray theAry;
    theAry.reserve( 1024 * 1024 );

    int ret = mrgStorageReadFile( mVi, 0,
                                  path.toLatin1().data(),
                                  name.toLatin1().data(),
                                  (quint8*)theAry.data() );
    if ( ret <= 0 )
    { return -1; }

    theAry.resize( ret );
    logDbg()<<ret;

    QXmlStreamReader reader( theAry );

    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "config" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "intf" )
                {
                    XPluginIntf::load( reader );
                }
                else if ( reader.name() == "setup" )
                {
                    serialIn( reader );
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int XPlugin::serialOut( QXmlStreamWriter &writer )
{ return 0; }
int XPlugin::serialIn( QXmlStreamReader &reader )
{ return 0; }

bool XPlugin::isEnabled( void *context )
{ return isOpened(); }

void XPlugin::onSetting( XSetting setting )
{
    if ( setting.mSetting == XPage::e_setting_mission_working )
    { setWorking( setting.mPara1.toBool() ); }
    else
    {
        //! do nothing
    }
}

void XPlugin::updateUi()
{
    XPage *pPage;
    foreach( QWidget *pWidget, mPluginWidgets )
    {
        Q_ASSERT( NULL != pWidget );
        pPage = dynamic_cast<XPage*>( pWidget );
        if ( NULL != pPage )
        { pPage->updateUi(); }
    }
}

void XPlugin::startup()
{}

void XPlugin::rst()
{}

void XPlugin::home()
{}
void XPlugin::fold()
{}

void XPlugin::reboot()
{}
void XPlugin::powerOff()
{}

int XPlugin::upload()
{ return 0; }
int XPlugin::download()
{ return 0; }
int XPlugin::diff()
{ return 0; }

int XPlugin::jointDir( int jid )
{ return 1; }

int XPlugin::startDemo( int id )
{ return 0; }
int XPlugin::stopDemo( int id )
{ return 0; }

void XPlugin::emit_user_role_change()
{
    emit_setting_changed( XPage::e_setting_user_role, QVariant() );
}

void XPlugin::emit_timer_op( QTimer *pTimer, int tmo, bool b )
{
    emit signal_timer_op( pTimer, tmo, b );
}

void XPlugin::emit_setting_changed( XPage::eXSetting setting, bool b )
{
    XSetting lSetting;

    lSetting.mSetting = setting;
    lSetting.mPara1.setValue( b );
    emit signal_setting_changed( lSetting );

    //! on setting
}

void XPlugin::emit_setting_changed( XPage::eXSetting setting, QVariant var )
{
    XSetting lSetting;

    lSetting.mSetting = setting;
    lSetting.mPara1 = var;
    emit signal_setting_changed( lSetting );
}

void XPlugin::emit_save()
{ emit signal_request_save(); }
void XPlugin::emit_load()
{ emit signal_request_load(); }

void XPlugin::setViewObj( void *pObj )
{
    Q_ASSERT( NULL != pObj );
    m_pViewObj = pObj;
}
void *XPlugin::viewObj()
{ return m_pViewObj; }

void XPlugin::attachDock( QDockWidget *pDock )
{
    Q_ASSERT( NULL != pDock );

    m_pDock = pDock;
}
void XPlugin::attachPanel( QWidget *pPanel )
{
    Q_ASSERT( NULL != pPanel );
    m_pPanelWidget = pPanel;
}
QWidget *XPlugin::panel()
{
    return m_pPanelWidget;
}

void XPlugin::attachPref( SysPara *pPref )
{
    Q_ASSERT( NULL != pPref );
    m_pPref = pPref;
}
SysPara * XPlugin::pref()
{ return m_pPref; }

QString XPlugin::homePath()
{
    return QDir::homePath() + "/AppData/Roaming/mct/" + model() + "/" + SN();
}

QString XPlugin::selfPath()
{
    return QString(mct_path) + "/" + model() + "/" + SN();
}

QString XPlugin::demoPath()
{
    return QString(mct_path) + "/" + model() + "/demo";
}

QString XPlugin::modelPath()
{
    return "/home/megarobo/MCT/" + model();
}
void XPlugin::lockWorking()
{
    mEmergMutex.lock();
    mMissionMutex.lock();
    mUpdateMutex.lock();
}
void XPlugin::unlockWorking()
{
    mEmergMutex.unlock();
    mMissionMutex.unlock();
    mUpdateMutex.unlock();
}

void XPlugin::awakeUpdate()
{
    if ( NULL != m_pUpdateWorking )
    { m_pUpdateWorking->awake(); }
}

void XPlugin::begin_log()
{
    Q_ASSERT( NULL != m_pLog );
    mLogMutex.lock();
}
void XPlugin::end_log()
{
    m_pLog->flush();

    //! write file

    //! sure the path
    if ( assurePath( log_file_path ) != 0 )
    { return; }

    //! over flush
    if ( mLogStream.size() > 5120 )
    {
        flush_log();
    }
    else
    {}

    mLogMutex.unlock();
}

//! delete the oldest one
void XPlugin::clean_log()
{
    QDir dir( log_file_path );

    QStringList logList;
    logList = dir.entryList( QDir::Files, QDir::Time | QDir::Reversed );

    logDbg()<<logList;

    for( int i = 0; i < logList.size() - 7; i++ )
    {
        dir.remove( logList.at( i ) );
    }
}

void XPlugin::flush_log()
{
    //! clean log
    clean_log();

    QFile file( log_file_name );
    if ( file.open( QIODevice::Append ) )
    {
        do
        {
            QTextCodec *pCoder = QTextCodec::codecForName("UTF-8");
            if ( NULL == pCoder )
            { break; }

            QTextEncoder *pEnc = pCoder->makeEncoder();
            if ( NULL == pEnc )
            { break; }

            file.write( pEnc->fromUnicode( mLogStream ) );
            delete pEnc;
        }while( 0 );

        file.close();

        mLogStream.clear();
        //! \note seek 0
        m_pLog->seek( 0 );
    }
}

void XPlugin::attachUpdateWorking( XPage *pObj,
                             WorkingApi::eWorkingType eType,
                             XPage::procDo proc,
                             const QString &desc,
                             void *pContext,
                             int tmoms
                    )
{
    Q_ASSERT( NULL != pObj );

    attachUpdateWorking( pObj, eType, proc, NULL, NULL, desc, pContext, tmoms );
}

void XPlugin::attachUpdateWorking(
                    XPage *pObj,
                    WorkingApi::eWorkingType eType,
                    XPage::procDo proc,
                    XPage::preDo pre,
                    XPage::postDo post,
                    const QString &desc,
                    void *pContext,
                    int tmoms
                    )
{
    Q_ASSERT( NULL != pObj );

    WorkingApi *pApi = new WorkingApi();
    if ( NULL == pApi )
    { return; }

    pApi->mWorkingType = eType;

    pApi->m_pIsEnabled = isEnabled;

    pApi->m_pProcDo = proc;
    pApi->m_pContext = pContext;
    pApi->m_pObj = pObj;

    pApi->m_pPreDo = pre;
    pApi->m_pPostDo = post;

    pApi->mDescription = desc;

    m_pUpdateWorking->attach( pApi );
    if ( m_pUpdateWorking->isRunning() )
    {}
    else
    { m_pUpdateWorking->start(); }
}

void XPlugin::__attachUpdateWorking(
                    XPage *pObj,
                    WorkingApi::eWorkingType eType,
                    XPage::procDo proc,
                    XPage::preDo pre,
                    XPage::postDo post,
                    const QString &desc,
                    void *pContext,
                    int tmoms
                    )
{
    Q_ASSERT( NULL != pObj );

    WorkingApi *pApi = new WorkingApi();
    if ( NULL == pApi )
    { return; }

    pApi->mWorkingType = eType;

    pApi->m_pIsEnabled = isEnabled;

    pApi->m_pProcDo = proc;
    pApi->m_pContext = pContext;
    pApi->m_pObj = pObj;

    pApi->m_pPreDo = pre;
    pApi->m_pPostDo = post;

    pApi->mDescription = desc;

    do
    {
        //! tmo
        Q_ASSERT( m_pUpdateWorking );
        if ( tmoms > 0 )
        {
            //! in the update working thread
//            QTimer *pTimer = new QTimer( );
//            if ( NULL == pTimer )
//            { break; }
//            logDbg()<<QThread::currentThreadId();
//            logDbg()<<pTimer->thread()<<thread()<<m_pUpdateWorking;
//            pTimer->moveToThread( m_pUpdateWorking );
//            logDbg()<<pTimer->thread();

//            pApi->m_pTimer = pTimer;
//            pTimer->setInterval( tmoms );
//            emit signal_timer_op( pTimer, tmoms, true );
//            emit_timer_op( pTimer, tmoms, true );
//            pTimer->start();

            //! connect
//            connect( pTimer, SIGNAL(timeout()),
//                     m_pUpdateWorking, SLOT())
//            connect( pTimer, SIGNAL(timeout()),
//                     m_pUpdateWorking,  [=]{ slot_api_proc( pApi); } );

//            m_pUpdateWorking->po
            QEvent *pEvent = new QEvent( QEvent::User );
            qApp->postEvent( m_pUpdateWorking, pEvent );

            //! mapped
//            connect( pTimer, SIGNAL(timeout()),
//                     m_pMapper, SLOT(map()));
//            m_pMapper->setMapping( pTimer, pApi );

            //! enable
            emit signal_api_operate( pApi, true );
        }

//        m_pUpdateWorking->attach( pApi );

        return;
    }while( 0 );

    delete pApi;
}

void XPlugin::attachMissionWorking( XPage *pObj,
                    XPage::onMsg onmsg,
                    QVariant var,
                    const QString &desc,
                    bool bMission
                    )
{
    Q_ASSERT( NULL != pObj );

    WorkingApi *pApi = new WorkingApi();
    if ( NULL == pApi )
    { return; }

    pApi->m_pObj = pObj;

    pApi->m_pIsEnabled = isEnabled;

    pApi->m_pOnMsg = onmsg;
    pApi->mVar = var;

    pApi->setType( WorkingApi::e_work_single );
    pApi->setDescription( desc );
    pApi->setMission( bMission );

    Q_ASSERT( m_pMissionWorking!=NULL );
    m_pMissionWorking->attach( pApi );
    if ( m_pMissionWorking->isRunning() )
    {}
    else
    { m_pMissionWorking->start(); }
}

void XPlugin::attachEmergencyWorking( XPage *pObj,
                    XPage::onMsg onmsg,
                    QVariant var,
                    const QString &desc
                    )
{
    Q_ASSERT( NULL != pObj );

    WorkingApi *pApi = new WorkingApi();
    if ( NULL == pApi )
    { return; }

    pApi->m_pObj = pObj;

    pApi->m_pIsEnabled = isEnabled;

    pApi->m_pOnMsg = onmsg;
    pApi->mVar = var;

    pApi->setType( WorkingApi::e_work_single );
    pApi->setDescription( desc );

    Q_ASSERT( m_pEmergencyWorking );
    m_pEmergencyWorking->attach( pApi );
    if ( m_pEmergencyWorking->isRunning() )
    {}
    else
    { m_pEmergencyWorking->start(); }
}

void XPlugin::attachBgWorking(
                    XPlugin::bgProc proc,
                    const QString &desc
                    )
{
    Q_ASSERT( NULL != m_pBgWorking );
    m_pBgWorking->attachBgProc( proc );
}

void XPlugin::cancelBgWorking()
{
    Q_ASSERT( NULL != m_pBgWorking );
    m_pBgWorking->requestInterruption();
    m_pBgWorking->wait();
}

bool XPlugin::splitPathName( const QString &fullPath,
                             QString &path, QString &name,
                             const QString &sep )
{
    int id;
    id = fullPath.lastIndexOf( sep );

    if ( id <= 0 )
    { return false; }

    name = fullPath.mid( id + 1, fullPath.length() - id + 1 );
    path = fullPath.mid( 0, id );
    return true;
}

bool XPlugin::event(QEvent *pEvent)
{
    Q_ASSERT( NULL != pEvent );
    if ( pEvent->type() >= XEvent::e_xevent_base
         && pEvent->type() < XEvent::e_xevent_max )
    {
        int ret;

        ret = onXEvent( (XEvent*)pEvent );

        pEvent->accept();
        return true;
    }
    else
    { return XPluginIntf::event( pEvent); }
}
int XPlugin::onXEvent( XEvent *pEvent )
{
    //! \todo on the event msg

    //! system
    if ( pEvent->type() == XEvent::e_xevent_prompt )
    {
        //! str, level
        sysPrompt( pEvent->mVar2.toString(), pEvent->mVar1.toInt() );
        return 0;
    }
    else if ( pEvent->type() == XEvent::e_xevent_startup )
    { startup(); }
    else if ( pEvent->type() == XEvent::e_xevent_device_exception )
    { onDeviceException( pEvent->mVar1 );}
    else
    {}

    return 0;
}

void XPlugin::onDeviceException( QVariant &var )
{

}

void XPlugin::slot_save_setting()
{
}

void XPlugin::slot_load_setting()
{ updateUi(); }

void XPlugin::slot_plugin_setting_changed( XSetting setting )
{
    onSetting( setting );
}

XPluginBgThread::XPluginBgThread( XPlugin *plugin, QObject *parent ) :QThread(parent),m_pPlugin(plugin)
{
    m_pShareMutex = NULL;
}

void XPluginBgThread::attachShareMutex( QMutex *pMutex )
{
    Q_ASSERT( NULL != pMutex );

    m_pShareMutex = pMutex;
}

void XPluginBgThread::run()
{
    XPlugin::bgProc proc;
    int ret;

    while( !mApis.isEmpty() )
    {
        if ( isInterruptionRequested() )
        { break; }

        proc = mApis.dequeue();

        if ( NULL != m_pShareMutex )
        { m_pShareMutex->lock(); }

            ret = (m_pPlugin->*proc)();

        if ( NULL != m_pShareMutex )
        { m_pShareMutex->unlock(); }
    }
}

void XPluginBgThread::attachBgProc( XPlugin::bgProc proc )
{
    mMutex.lock();
        mApis.enqueue( proc );
    mMutex.unlock();

    if ( isRunning() )
    {}
    else
    { start(); }
}
