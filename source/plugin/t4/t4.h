#ifndef MRX_T4_H
#define MRX_T4_H


#include <QObject>
#include <QTcpSocket>
#include <float.h>
#include "../plugin/xplugin.h"
#include "../model/errmgrmodel.h"

#include "./model/treemodel.h"

#include "t4para.h"

#define selfPara  ((MRX_T4*)m_pPlugin)

#define _check_connect( p ) MRX_T4 *pRobo = (MRX_T4*)p;\
                        Q_ASSERT( NULL != pRobo );\
                        \
                        if ( pRobo->isOpened() )\
                        {}\
                        else\
                        { return; }

#define _check_connect_ret( p, ret ) MRX_T4 *pRobo = (MRX_T4*)p;\
                        Q_ASSERT( NULL != pRobo );\
                        \
                        if ( pRobo->isOpened() )\
                        {}\
                        else\
                        { return ret; }

#define check_connect() _check_connect( m_pPlugin )

#define check_connect_ret( ret ) _check_connect_ret( m_pPlugin, ret )



#define robot_var() (ViSession)pRobo->deviceVi(), pRobo->robotHandle()
#define device_var() (ViSession)pRobo->deviceVi(), pRobo->deviceHandle()
#define robot_var_handle() pRobo->robotHandle()
#define device_var_handle() pRobo->deviceHandle()
#define device_var_vi() (ViSession)pRobo->deviceVi()

#define self_check_connect() \
                        if ( isOpened() )\
                        {}\
                        else\
                        { return; }

#define self_check_connect_ret( ret ) \
                        if ( isOpened() )\
                        {}\
                        else\
                        { return ret; }

#define self_robot_var()  (ViSession)deviceVi(),robotHandle()
#define self_device_var()    (ViSession)deviceVi(),deviceHandle()

#define _on_post_setting( cls, onSetting, desc, bMission )    m_pPlugin->attachMissionWorking( this, (XPage::onMsg)(&cls::onSetting), var, desc, bMission );

#define on_post_setting( cls, onSetting, desc )    _on_post_setting( cls, onSetting, desc, true )
#define on_post_setting_n_mission( cls, onSetting, desc )    _on_post_setting( cls, onSetting, desc, false )

#define on_post_setting_emerge( cls, onSetting, desc ) m_pPlugin->attachEmergencyWorking( this, (XPage::onMsg)(&cls::onSetting), var, desc );

#define wave_table  0
#define distance_error      (0.1)

#define guess_dist_calc_time_ms( dist )    (dist * 1000)
//! 运行时间+计算时间+交互时间   交互时间>800 ms 下发指令到runing状态大概900ms左右
#define guess_dist_time_ms( ts, dist )     ( (ts)*1000 + guess_dist_calc_time_ms(dist) + 2000)

#define rel_to_abs_speed( rel ) ( rel/100.0*pRobo->mMaxTerminalSpeed )


//! file names
#define record_file_name  "MCT_motion.mrp"
//#define error_mgr_file_name "errmgr.xml"
#define config_file_name  "config.xml"
#define debug_file_name "debug.xml"

#define update_file_name    "update.txt"

//! \todo need the file size query
#define max_file_size       (1024*1024)

#define plugin_root_path()  m_pPlugin->selfPath().toLatin1().data()
#define plugin_root_dir()   m_pPlugin->deviceVi(),\
                            0,\
                            plugin_root_path()

namespace mrx_t4 {
class ActionTable;
class T4OpPanel;
}
class SequenceItem;

class MRX_T4 : public XPlugin, public T4Para
{
    Q_OBJECT

public:
    enum eT4Settings
    {
        e_setting_terminal = XPage::e_setting_user,
        e_setting_current,
        e_add_record,
        e_edit_record,
        e_setting_record,
    };

    enum eT4XEvent
    {
        e_x_update_ui = XEvent::e_xevent_plugins,
    };

public:
    MRX_T4( QObject *parent = nullptr );
    ~MRX_T4();

public:
    virtual QTreeWidgetItem* createPrefPages( QStackedWidget *stack );
    virtual QWidget *createOpsPanel( QAbstractListModel *pModel,
                                     QWidget *parent=nullptr );
    virtual void retranslateUi();

public:
    virtual void rstErrorMgrTable();
    virtual void rstRecordTable();

protected:
    void ErrorMgrTable( QByteArray &ary );
    void RecordData( QByteArray &ary );
public:
    virtual void onSetting(XSetting setting);
public:
    virtual int serialOut(QXmlStreamWriter &writer);
    virtual int serialIn(QXmlStreamReader &reader);

protected Q_SLOTS:
    void slot_save_pw();
    void slot_save_setting();
    void slot_load_setting();
    void slot_exception_arrived();
    void slotSocketError(QAbstractSocket::SocketError e);

    void slot_dia_refresh_timeout();

public:
    virtual bool isOnLine();

    virtual int open();
private:
    int _open( int &vi );

public:
    virtual void close();

    virtual void startup();

    virtual int stop();
    virtual int fStop();

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

protected:
    virtual int onXEvent( XEvent *pEvent );
    virtual void onDeviceException( QVariant &var );

protected:
    void updateUis();
    void xevent_updateui( XEvent *pEvent );

protected:
    int _uploadProc();
    int uploadProc();

    int _downloadProc();
    int downloadProc();

    int _diffProc();
    int diffProc();

    int onStop( QVariant var );

    int _startupProc();
    int startupProc();

public:
    int robotHandle();
    int deviceHandle();

public:
    int currentRecordIndex();

    //! api
public:
    static double eulaDistance( double x, double y, double z,
                                double x1, double y1, double z1 );
public:
    int relMove( QString para,
                 double x, double y, double z,
                 double pw, double h,
                 double v, bool bLine );
    int absMove( QString para,
                 double x, double y, double z,
                 double pw, double h,
                 double v, bool bLine );

    void setAbsMarker( double x, double y, double z,
                       double pw, double h,
                       double v, bool bLine );
    void setAbsMarker( SequenceItem &item );
    SequenceItem *absMarker();

public:
    TreeModel* m_pRecordModel;
protected:
    ErrorMgrModel mErrorConfigTable;

    int mRobotHandle;
    int mDeviceHandle;

private:
    mrx_t4::ActionTable *m_pRecordView;
    mrx_t4::T4OpPanel *m_pOpPanel;

    QTcpSocket *m_pExceptionSocket;

private:
    SequenceItem *m_pMarkerItem;
    QTimer mRefTimer;

};

#endif // T4_H
