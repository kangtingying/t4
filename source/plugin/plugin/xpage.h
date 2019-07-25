#ifndef XPAGE_H
#define XPAGE_H

#include <QtWidgets>
#include "../../sys/sysapi.h"
#include "../../wnd/syspara.h"

#include "../../../include/mystd.h"

class XPlugin;
class XSetting;
class WorkingApi;

#define set_page_attr( attr )   mAttr = (attr);
#define set_page_rstable( )     set_page_attr( page_rst_able )

#define check_para1()       if ( setting.mPara1.isValid() )\
                            {}\
                            else\
                            { return; }

#define begin_page_log()    do{ \
                                if( NULL == m_pPlugin ){ break; }\
                                if ( NULL == m_pPlugin->m_pLog ){ break; }\
                                m_pPlugin->begin_log(); (*m_pPlugin->m_pLog)<<QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz ")\
                                                                            <<__FUNCTION__<<" "<<__LINE__<<" "
#define begin_page_log1( v1 ) begin_page_log()<<v1
#define begin_page_log2( v1,v2 ) begin_page_log1(v1)<<" "<<v2
#define begin_page_log3( v1,v2,v3 ) begin_page_log2(v1,v2)<<" "<<v3
#define begin_page_log4( v1,v2,v3,v4 ) begin_page_log3(v1,v2,v3)<<" "<<v4

#define end_page_log()      (*m_pPlugin->m_pLog)<<"\n"; m_pPlugin->end_log(); } while( 0 );

class XPage : public QWidget
{
    Q_OBJECT

public:
    enum PageAttr
    {
        page_attr_unk = 0,
        page_rst_able = 1
    };

    enum eXSetting
    {
        e_setting_unk = 0,
        e_setting_op_able,
        e_setting_online,
        e_setting_opened,
        e_setting_work_role,

        e_setting_update_ui,
        e_setting_mission_working,
        e_setting_user_role,

        e_setting_user = 1024,
    };

public:
    typedef bool (XPage::*isEnable)( void *context );
    typedef void (XPage::*preDo)( void *context );
    typedef int (XPage::*procDo)( void *context );
    typedef void (XPage::*postDo)( void *context, int ret );

    typedef int (XPage::*onMsg)( QVariant var );

public:
    explicit XPage(QWidget *parent = nullptr);

protected:
    virtual bool event(QEvent *event);

    virtual void changeEvent(QEvent *event);
    virtual void focusInEvent(QFocusEvent *event);

protected:
    virtual void retranslateUi();
    virtual void onPluginChanged();

public:
    void translateUi();

    void attachPlugin( XPlugin *pPlugin );
    XPlugin *pulgin();

    void attachPref( SysPara *pPref );
    SysPara * pref();

    bool isQualified();

    PageAttr pageAttr();
    virtual void rst();

    virtual void connectPlugin();
    virtual void attachWorkings();

    virtual void updateUi();        //! data -> ui
    virtual void updateData();
    virtual void startup();

    virtual void updateRole();
    virtual void updateWorkingRole( int wRole );

    virtual int upload();
    virtual int download();
    virtual int diff();

    virtual void onSetting( XSetting setting );
public:
    QString model();
    void showFocusHelp();

    void attachUpdateWorking( XPage::procDo proc,
                              int wType,
                              const QString &desc = "",
                              void *pContext = NULL,
                              int tmo = 100
                        );
    void attachUpdateWorking( XPage::procDo proc,
                              int wType,
                        XPage::preDo pre,
                        XPage::postDo post,
                        const QString &desc = "",
                        void *pContext = NULL,
                        int tmo = 100
                        );

    void attachMissionWorking( XPage *pObj,
                        XPage::onMsg onmsg,
                        QVariant var,
                        const QString &desc = ""
                        );

    void attachEmergencyWorking( XPage *pObj,
                        XPage::onMsg onmsg,
                        QVariant var,
                        const QString &desc = ""
                        );

protected:
    void setUri( const QString &uri );
    QString Uri( );

    void setContextHelp( const QString &str );
    QString contextHelp();

public:
    int save( const QString &fileName );
    int load( const QString &fileName );

protected:
    virtual int serialOut( QXmlStreamWriter &writer );
    virtual int serialIn( QXmlStreamReader &reader );

protected:
    virtual void spyEdited();
    void spySetting( int setting );
    bool filterSetting( XSetting setting );

    virtual void enterMission();
    virtual void exitMission();

    virtual void setOperAble( bool b );
    virtual void setOnLine( bool b );
    virtual void setOpened( bool b );

signals:
    void signal_request_save();
    void signal_focus_changed( const QString &model,
                               const QString &contextHelp );

protected slots:
    void slot_plugin_setting_changed( XSetting setting );
    void slot_modified();

    virtual void slot_enter_mission( WorkingApi *api );
    virtual void slot_exit_mission( WorkingApi *api, int ret );

    virtual void slot_request_save();
    virtual void slot_request_load();

protected:
    QList< int > mSpySetting;

    XPlugin *m_pPlugin;
    SysPara *m_pPref;

    PageAttr mAttr;

    QString mUri;

    QString mContextHelp;

    bool mbMissionWorking;

};

class XSetting
{
public:
    XPage::eXSetting mSetting;
    QVariant mPara1, mPara2, mPara3;
};
Q_DECLARE_METATYPE(XSetting)


#endif // XPAGE_H
