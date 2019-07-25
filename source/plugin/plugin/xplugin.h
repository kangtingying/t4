#ifndef XPLUGIN_H
#define XPLUGIN_H

#include <QObject>
#include <QStackedWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QThread>
#include <QMutex>

#include "../../../source/wnd/syspara.h"
#include "xpage.h"
#include "xpluginintf.h"
#include "xpluginworkingthread.h"

#include "mlog.h"

//! macros
#define new_widget_base( type, var, idstr, txt, icon ) \
do{ \
    var = new type;\
    Q_ASSERT( NULL != var ); \
    var->attachPref( m_pPref );\
    var->attachPlugin( this );\
    var->attachWorkings(); \
    var->setObjectName( idstr );\
    \
    QTreeWidgetItem *plwItem = new QTreeWidgetItem();   \
    Q_ASSERT( NULL != plwItem ); \
    plwItem->setIcon( 0, QIcon( icon) ); \
    plwItem->setText( 0, (txt) ); \
    plwItem->setData( 0, Qt::UserRole, QVariant( QVariant::fromValue(var) ) ); \
    plwItem->setData( 0, Qt::UserRole + 1, QVariant( QVariant::fromValue(this) ) ); \
    mTreeItems.append( plwItem );\
    pRoot->addChild(plwItem); \
    stack->addWidget( var );     \
    mPluginWidgets.append( var ); \
}while(0)

#define new_widget( type, var, txt, icon ) new_widget_base( type, var, txt, txt, icon )

#define new_root_widget( type, var, txt, icon, root ) \
do{ \
    var = new type;\
    Q_ASSERT( NULL != var ); \
    var->attachPlugin( this );\
    var->attachWorkings();\
    var->setObjectName( txt );\
    \
    \
    root->setIcon( 0, QIcon( icon) ); \
    root->setText( 0, (txt) ); \
    root->setData( 0, Qt::UserRole, QVariant( QVariant::fromValue(var) ) ); \
    stack->addWidget( var ); \
    mPluginWidgets.append( var ); \
}while(0)

#define _begin_log()    begin_log(); m_pLog<<__FUNCTION__<<__LINE__
#define _end_log()      end_log()

//! message box
#include <qmessagebox.h>
#define msgBox_Warning_ok( title, content )     (QMessageBox::warning(this, title, content, QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok ? 1:0)
#define msgBox_Information_ok( title, content ) (QMessageBox::information(this, title, content, QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok ? 1:0)


class XPluginWorkingThread;
class XPluginBgThread;

class XEvent : public QEvent
{
public:
    enum XEventType
    {
        e_xevent_base = QEvent::User + 1,
        e_xevent_start = QEvent::User + 1,
        e_xevent_stop,
        e_xevent_pause,
        e_xevent_estop,
        e_xevent_startup,
                            //! v1: exception id
        e_xevent_device_exception,

        e_xevent_prompt,    //! v1: 0,1,2; v2: info

        e_xevent_plugins = QEvent::User + 128,



        e_xevent_max = QEvent::User + 512,
    };

public:
    XEvent( int tpe) : QEvent( (QEvent::Type)tpe )
    {}

    XEvent( int tpe, QVariant v1 ) : QEvent( (QEvent::Type)tpe )
    { mVar1 = v1; }

    XEvent( int tpe, QVariant v1, QVariant v2 ) : QEvent( (QEvent::Type)tpe )
    {
        mVar1 = v1;
        mVar2 = v2;
    }

public:
    QVariant mVar1;
    QVariant mVar2;

public:
    void setPara( QVariant v1, QVariant v2 )
    {
        mVar1 = v1;
        mVar2 = v2;
    }
};

class XPlugin : public XPluginIntf
{
    Q_OBJECT
public:
    typedef int (XPlugin::*bgProc)();

public:
    XPlugin( QObject *parent = nullptr );
    virtual ~XPlugin();

signals:
    void signal_focus_changed( const QString &model,
                               const QString &contextHelp );
    void signal_setting_changed( XSetting setting );
    void signal_request_save();
    void signal_request_load();

    void signal_api_operate( QObject *pApi, bool );
    void signal_timer_op( QTimer *pTimer, int tmo, bool );

public:
    virtual QTreeWidgetItem* createPrefPages( QStackedWidget *stack );
    virtual QWidget *createOpsPanel( QAbstractListModel *pModel,
                                     QWidget *parent=nullptr );

    virtual void setActive( );
    virtual void retranslateUi();

    virtual void rstErrorMgrTable();
    virtual void rstRecordTable();

    virtual void onOperateAble( bool b );
    virtual void onOnLine( bool b );

public:
    virtual int save( const QString &filename );
    virtual int load( const QString &filename );

    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

    virtual bool isEnabled( void *context );

    virtual void onSetting( XSetting setting );

public:

    virtual void updateUi();

    virtual void startup();
    virtual void rst();
    virtual void home();
    virtual void fold();

    virtual void reboot();
    virtual void powerOff();

    virtual int upload();
    virtual int download();
    virtual int diff();

    virtual int jointDir( int jid );

    virtual int startDemo( int id );
    virtual int stopDemo( int id );

public:
    virtual void emit_user_role_change();

    void emit_timer_op( QTimer *pTimer, int tmo, bool b );

    void emit_setting_changed( XPage::eXSetting setting, bool b );
    void emit_setting_changed( XPage::eXSetting setting, QVariant var );

    void emit_save();
    void emit_load();

    void setViewObj( void *pObj );
    void *viewObj();

    void attachDock( QDockWidget *pDock );
    void attachPanel( QWidget *pPanel );

    void attachPref( SysPara *pPref );
    SysPara * pref();

    QWidget *panel();

    QString homePath();
    QString selfPath();
    QString demoPath();
    QString modelPath();

public:
    void lockWorking();
    void unlockWorking();

    void awakeUpdate();

    void begin_log();
    void end_log();
    void clean_log();
    void flush_log();

    void attachUpdateWorking( XPage *pObj,
                        WorkingApi::eWorkingType eType,
                        XPage::procDo proc,
                        const QString &desc="",
                        void *pContext = NULL,
                        int tmoms = 100
                        );
    void attachUpdateWorking( XPage *pObj,
                        WorkingApi::eWorkingType eType,

                        XPage::procDo proc,
                        XPage::preDo pre,
                        XPage::postDo post,
                        const QString &desc="",
                        void *pContext = NULL,
                        int tmoms = 100
                        );

    void __attachUpdateWorking( XPage *pObj,
                        WorkingApi::eWorkingType eType,

                        XPage::procDo proc,
                        XPage::preDo pre,
                        XPage::postDo post,
                        const QString &desc="",
                        void *pContext = NULL,
                        int tmoms = 100
                        );

    void attachMissionWorking( XPage *pObj,
                        XPage::onMsg onmsg,
                        QVariant var,
                        const QString &desc="",
                        bool bMission = true
                        );

    void attachEmergencyWorking( XPage *pObj,
                        XPage::onMsg onmsg,
                        QVariant var,
                        const QString &desc=""
                        );

    //! plugin
    void attachBgWorking(
                        XPlugin::bgProc proc,
                        const QString &desc=""
                        );
    void cancelBgWorking();

protected:
    bool splitPathName( const QString &fullPath, QString &path, QString &name, const QString &sep="/" );
    virtual bool event(QEvent *event);
    virtual int onXEvent( XEvent *pEvent );

     virtual void onDeviceException( QVariant &var );
protected:
    QTreeWidgetItem *m_pRootWidgetItem;
    QWidget *m_pPanelWidget;
    QDockWidget *m_pDock;

    QList<QWidget*> mPluginWidgets;
    QList<QTreeWidgetItem*> mTreeItems;

    void *m_pViewObj;

    SysPara *m_pPref;

public:
    MLog *m_pLog;
    QMutex mLogMutex;
    QByteArray mLogStream;
public:
    XPluginWorkingThread *m_pEmergencyWorking;
    XPluginWorkingThread *m_pMissionWorking;
    XPluginWorkingThread *m_pUpdateWorking;

    XPluginBgThread *m_pBgWorking;

    QMutex mEmergMutex, mMissionMutex, mUpdateMutex;

    QMutex mShareMutex;

protected Q_SLOTS:
    virtual void slot_save_setting();
    virtual void slot_load_setting();

    void slot_plugin_setting_changed( XSetting setting );
};

class XPluginBgThread : public QThread
{
    Q_OBJECT
public:
    XPluginBgThread( XPlugin *plugin, QObject *parent = NULL );

    void attachShareMutex( QMutex *pMutex );
protected:
    virtual void run();

public:
    void attachBgProc( XPlugin::bgProc );

protected:
    XPlugin *m_pPlugin;
    QQueue< XPlugin::bgProc > mApis;
    QMutex mMutex;

    QMutex *m_pShareMutex;
};




#endif

