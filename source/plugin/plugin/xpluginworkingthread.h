#ifndef XPLUGINWORKINGTHREAD_H
#define XPLUGINWORKINGTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "xpage.h"

class WorkingApi : public QObject
{
    Q_OBJECT

public:
    enum eWorkingType
    {
        e_work_loop,
        e_work_single
    };

    enum eWorkingClass
    {
        e_work_emergency,
        e_work_mission,
        e_work_update,
    };
public:
    WorkingApi( QObject *parent = Q_NULLPTR );
    ~WorkingApi();

    void setType( WorkingApi::eWorkingType type );
    WorkingApi::eWorkingType getType();

    void setClass( WorkingApi::eWorkingClass cls );
    WorkingApi::eWorkingClass getClass();

    void setDescription( const QString &desc );

    void setMission( bool b );

public:
    eWorkingType mWorkingType;
    eWorkingClass mWorkingClass;

    XPage *m_pObj;
    void *m_pContext;
    XPage::preDo m_pPreDo;
    XPage::procDo m_pProcDo;
    XPage::postDo m_pPostDo;

    QVariant mVar;
    XPage::onMsg m_pOnMsg;
    XPage::isEnable m_pIsEnabled;

    QTimer *m_pTimer;
    QString mDescription;
    bool mbMission;
};

//Q_DECLARE_METATYPE( WorkingApi * )
//Q_DECLARE_METATYPE( WorkingApi )

class XPluginWorkingThread : public QThread
{
    Q_OBJECT
public:
    XPluginWorkingThread( QObject *parent =  Q_NULLPTR );
    virtual ~XPluginWorkingThread();

    void attachMutex( QMutex *pMutex );
    void attachShareMutex( QMutex *pMutex );
Q_SIGNALS:
    void signal_enter_working( WorkingApi *api);
    void signal_exit_working( WorkingApi *api, int );

    void signal_updating();

protected Q_SLOTS:
    void slot_api_proc( QObject *pApi );
    void slot_api_operate( QObject *pApi, bool b );

public:
    void attach( WorkingApi * pApi );
    void detach();

    void setTick( int ms );
    int tick();

    void awake();

protected:
    virtual void run();

protected:
    void procApis();
    void procApi( WorkingApi *pApi );

    void selfSleep( int ms );

protected:
    const int _tickms = 100;
    const int _ticktick = 100;

    QList< WorkingApi *> mApis;

    int mTickms;

    QMutex mMutex;

    QMutex *m_pWorkMutex;
    QMutex *m_pShareMutex;

    QSemaphore mWakeupSema;
};

class XPluginUpdateingThread : public XPluginWorkingThread
{
    Q_OBJECT

public:
    XPluginUpdateingThread( QObject *parent =  Q_NULLPTR );
    virtual ~XPluginUpdateingThread();

public:
    virtual bool event( QEvent *event );

protected Q_SLOTS:
    void slot_timer_op( QTimer *pTimer, int tmo, bool b );
    void slot_timeout();

protected:
    virtual void run();

protected:
    QTimer *m_pTimer;
};

#endif // XPLUGINWORKINGTHREAD_H
