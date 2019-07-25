#include "xpluginworkingthread.h"

WorkingApi::WorkingApi( QObject *parent ) : QObject( parent )
{
    mWorkingType = e_work_loop;
    mWorkingClass = e_work_mission;

    m_pObj = NULL;
    m_pContext = NULL;

    m_pPreDo = NULL;
    m_pProcDo = NULL;
    m_pPostDo = NULL;

    m_pOnMsg = NULL;
    m_pIsEnabled = NULL;

    m_pTimer = NULL;

    mbMission = true;
}

WorkingApi::~WorkingApi()
{
//    if ( NULL != m_pTimer )
//    { delete m_pTimer; }
}

void WorkingApi::setType( WorkingApi::eWorkingType type )
{ mWorkingType = type; }
WorkingApi::eWorkingType WorkingApi::getType()
{ return mWorkingType; }

void WorkingApi::setClass( WorkingApi::eWorkingClass cls )
{ mWorkingClass = cls; }
WorkingApi::eWorkingClass WorkingApi::getClass()
{ return mWorkingClass; }

void WorkingApi::setDescription( const QString &desc )
{ mDescription = desc; }

void WorkingApi::setMission( bool b )
{ mbMission = b; }

XPluginWorkingThread::XPluginWorkingThread( QObject *parent ) : QThread( parent )
{
    m_pWorkMutex = NULL;
    m_pShareMutex = NULL;

    mTickms = 100;
}

XPluginWorkingThread::~XPluginWorkingThread()
{
    delete_all( mApis );
}

void XPluginWorkingThread::attachMutex( QMutex *pMutex )
{
    Q_ASSERT( NULL != pMutex );

    m_pWorkMutex = pMutex;
    logDbg()<<m_pWorkMutex;
}

void XPluginWorkingThread::attachShareMutex( QMutex *pMutex )
{
    Q_ASSERT( NULL != pMutex );

    m_pShareMutex = pMutex;
}

void XPluginWorkingThread::slot_api_proc( QObject *pApi )
{
    Q_ASSERT( NULL != pApi );

    procApi( (WorkingApi*)pApi );
}

void XPluginWorkingThread::slot_api_operate( QObject *pApi, bool b )
{
    Q_ASSERT( NULL != pApi );

    WorkingApi *pWorkingApi;

    pWorkingApi = (WorkingApi*)pApi;
    Q_ASSERT( NULL != pWorkingApi && NULL != pWorkingApi->m_pTimer );
    if ( b )
    {
        pWorkingApi->m_pTimer->start();
    }
    else
    { pWorkingApi->m_pTimer->stop(); }
}

void XPluginWorkingThread::attach( WorkingApi * pApi )
{
    Q_ASSERT( NULL != pApi );

    mMutex.lock();
    if ( mApis.contains( pApi ) )
    {}
    else
    { mApis.append( pApi ); }

    mMutex.unlock();
}
void XPluginWorkingThread::detach()
{
    mMutex.lock();

    //! \todo api

    mMutex.unlock();
}

void XPluginWorkingThread::setTick( int ms )
{ mTickms = ms; }
int XPluginWorkingThread::tick()
{ return mTickms; }

void XPluginWorkingThread::awake()
{
    mWakeupSema.release();
}

void XPluginWorkingThread::run()
{
    bool bLocked = false;
    try
    {
        forever
        {
            if ( isInterruptionRequested() )
            { break; }

            if ( mApis.size() < 1 )
            {
                QThread::msleep( XPluginWorkingThread::_tickms);
                continue;
            }

            if ( NULL != m_pShareMutex )
            { m_pShareMutex->lock(); bLocked = true; }

                //! proc
                procApis();

            if ( NULL != m_pShareMutex )
            { m_pShareMutex->unlock(); bLocked = false; }
        }
    }
    catch( QException &e )
    {
        if ( NULL != m_pShareMutex && bLocked )
        { m_pShareMutex->unlock(); bLocked = false; }
    }

    delete_all( mApis );
    logDbg();
}

void XPluginWorkingThread::procApis()
{
    foreach( WorkingApi *pApi, mApis )
    {
        Q_ASSERT( NULL != pApi );
        Q_ASSERT( NULL != pApi->m_pObj );

        if ( isInterruptionRequested() )
        { return; }

        procApi( pApi );

        //! remove
        if ( pApi->mWorkingType == WorkingApi::e_work_loop )
        {  }
        else
        {
            delete pApi;
            mMutex.lock();
            mApis.removeAll( pApi );
            mMutex.unlock();
        }
    }

    //! batch tick
    if ( mTickms > 0 )
//    { QThread::msleep( mTickms ); }
    { selfSleep( mTickms ); }
}

void XPluginWorkingThread::procApi( WorkingApi *pApi )
{
    Q_ASSERT( NULL != pApi );
    int ret;

    bool bMission = ( pApi->mbMission );

    if ( bMission )
    { emit signal_enter_working( pApi ); }

        do
        {
            //! enabled
            if ( pApi->m_pIsEnabled &&
                 !(pApi->m_pObj->*(pApi->m_pIsEnabled))( pApi->m_pContext ) )
            { break; }
            else
            {  }

            if ( NULL != m_pWorkMutex  )
            { m_pWorkMutex->lock(); }

            try
            {
                if ( pApi->m_pPreDo )
                {  (pApi->m_pObj->*(pApi->m_pPreDo))( pApi->m_pContext ); }

                //! call the api
                if ( pApi->m_pProcDo )
                {
                    ret = (pApi->m_pObj->*(pApi->m_pProcDo))( pApi->m_pContext );
                    if ( ret != 0 )
                    {
                        logDbg()<<ret;
                        if ( pApi->mDescription.length() > 0 )
                        { sysError( pApi->mDescription + " " + tr("execute fail") ); }
                    }
                }
                else
                { ret = 0; }

                //! msg api
                if ( pApi->m_pOnMsg )
                {
                    ret = (pApi->m_pObj->*(pApi->m_pOnMsg))( pApi->mVar );
                    logDbg()<<ret;
                    if ( ret != 0 && pApi->mDescription.length() > 0 )
                    { sysPrompt( pApi->mDescription + " " + tr("execute fail") ); logDbg(); }
                }

                if ( pApi->m_pPostDo )
                {  (pApi->m_pObj->*(pApi->m_pPostDo))( pApi->m_pContext, ret ); }
            }
            catch( QException &e )
            {}

            if ( NULL != m_pWorkMutex )
            { m_pWorkMutex->unlock(); }

        }while( 0 );

        //! \note delete in this thread
        //! alert to be used in slot
    if ( bMission )
    { emit signal_exit_working( pApi, ret ); }
}

//selfSleep
void XPluginWorkingThread::selfSleep( int ms )
{
    while( ms > 0 )
    {
        if ( mWakeupSema.tryAcquire( 1, XPluginWorkingThread::_ticktick ) )
        { break; }
        else
        { ms -= XPluginWorkingThread::_ticktick; }
    }
}

XPluginUpdateingThread::XPluginUpdateingThread( QObject *parent )
                        : XPluginWorkingThread(parent)
{}

XPluginUpdateingThread::~XPluginUpdateingThread()
{}

bool XPluginUpdateingThread::event( QEvent *event )
{
    if ( event->type() == QEvent::User )
    {
        logDbg()<<thread()<<QThread::currentThreadId();
        event->accept();
        return true;
    }
    else
    {
        return QThread::event( event );
    }
}

void XPluginUpdateingThread::slot_timer_op( QTimer *pTimer, int tmo, bool b )
{
    Q_ASSERT( NULL != pTimer );
logDbg()<<tmo<<b;
    logDbg()<<QThread::currentThread()<<pTimer->thread()<<QThread::currentThreadId();
//    if ( b )
//    { pTimer->start(tmo); }
//    else
//    { pTimer->stop();}
}

void XPluginUpdateingThread::slot_timeout()
{
    logDbg()<<QThread::currentThread()<<m_pTimer->thread()<<QThread::currentThreadId();
}

void XPluginUpdateingThread::run()
{logDbg()<<QThread::currentThreadId();

    m_pTimer = new QTimer();
    m_pTimer->moveToThread( this );
    m_pTimer->start( 3000 );

    connect( m_pTimer, SIGNAL(timeout()),
             this, SLOT(slot_timeout()) );

    try
    {
        QThread::run();
    }
    catch( QException &e )
    {}
    qDeleteAll( mApis );
    logDbg();
}
