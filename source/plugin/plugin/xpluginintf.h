#ifndef XPLUGININTF_H
#define XPLUGININTF_H

#include <QObject>
#include <QtCore>

#define password_file_name  "password.xml"
#define mct_path            "/home/megarobo/MCT"
class XPluginIntf : public QObject
{
public:
    enum eUserRole
    {
        user_user,
        user_admin,
    };

    enum eWorkingRole
    {
        working_unk,

        //! disconnected
        working_disconnected,

        //! connected
        working_normal,
        working_mrq_corrupted,
        working_device_not_ready,
    };

public:
    enum ePluginAttr{
        plugin_attr_none = 0,
        plugin_attr_foldable = 1,
        plugin_attr_reboot_able = 2,
        plugin_attr_poweroff_able = 4,
    };

    Q_OBJECT
public:
    XPluginIntf( QObject *parent = nullptr );

public:
    void rstAuthority();

public:
    bool isEqual( XPluginIntf *p );
    bool isEqual( const QString &addr,
                  const QString &model,
                  const QString &sn );

    QString description();

    void setOperateAble( bool b );
    bool operateAble();

    virtual void onOperateAble( bool b );

    void setOnLine( bool b );
    bool onLine();

    virtual void onOnLine( bool b );

    void setWorking( bool b );
    bool isWorking();

    void setModel( const QString &model );
    QString model();

    //! internal addr
    void setAddr( const QString &addr );
    QString addr();

    //! view addr
    void setViewAddr( const QString &vAddr );
    QString viewAddr();

    void setSN( const QString &sn );
    QString SN();

    void setFirmwareVer( const QString &ver );
    QString firmwareVer();

    void setMechanicalVer( const QString &ver );
    QString mechanicalVer();

    //! MRH-T Info
    void setMRHType( const QString &type );
    QString mrhType();

    void setMRHSN( const QString &sn );
    QString mrhSn();

    void setMRHVer( const QString &ver );
    QString mrhVer();

    void setMRHBBSw( const QString &sw );
    QString mrhBBSw();

    void setMRHBBHw( const QString &hw );
    QString mrhBBHw();

    //! mrq attr
    void setType_MRQ( const QString &s ){ mType = s; }
    QString Type_MRQ(){ return  mType; }

    void setSN_MRQ( const QString &s ){ mSN_MRQ = s; }
    QString SN_MRQ(){ return mSN_MRQ; }

    void setSoftVer_MRQ( const QString &s ){ mSoftVer_MRQ = s; }
    QString SoftVer_MRQ(){ return mSoftVer_MRQ; }

    void setMrqVer( const QString &ver )
    { mMrqVer = ver; }
    QString mrqVer()
    { return mMrqVer; }

    void setFirmWareHard_MRQ( const QString &s ){ mFirmWareHard_MRQ = s; }
    QString FirmWareHard_MRQ(){ return mFirmWareHard_MRQ; }

    void setFirmWareBoot_MRQ( const QString &s ){ mFirmWareBoot_MRQ = s; }
    QString FirmWareBoot_MRQ(){ return mFirmWareBoot_MRQ; }

    void setFirmWareFpga_MRQ( const QString &s ){ mFirmWareFpga_MRQ = s; }
    QString FirmWareFpga_MRQ(){ return mFirmWareFpga_MRQ; }

    void setDevId( const int id ){ mDevId = id; }
    int DevId(){ return mDevId; }

    //! authority
    void setPw( eUserRole role, const QString &pw );
    QString getPw( eUserRole role, bool &bOk );
    void rstPw( eUserRole role );

    void setAutoLogin( bool b );
    bool isAutoLogin();

public:
    int save( QXmlStreamWriter &writer );
    int load( QXmlStreamReader &reader );

    int savePw( const QString &path, const QString &name = password_file_name );
    int loadPw( const QString &path, const QString &name = password_file_name );

Q_SIGNALS:
    void signal_request_pw_save();
public:
    virtual void emit_user_role_change();

    virtual int open();
    virtual void close();

    virtual int stop();
    virtual int fStop();

    bool isOpened();
    bool isFoldable();
    bool isRebootable();
    bool isPowerOffable();

    virtual bool isOnLine();

    void attachVi( int vi );
    int deviceVi();

    void lockVi();
    void unlockVi();

    void setUserRole( eUserRole role );
    eUserRole userRole();
    bool isAdmin();

    void setWorkingRole( eWorkingRole role );
    eWorkingRole workingRole();

protected:
    bool mbOperateAble;
    bool mbOnLine;
    bool mbWorking;

    //! \note view addr for the user view
    QString mModel, mAddr, mViewAddr;
    int mVi;
    QMutex mRWMutex;

    QString mSN, mFirmwareVer, mMechanicalVer;

    //! MRH Info
    QString mMRHType, mMRHSN, mMRHVer, mMRHBBSw, mMRHBBHw;

    //! MRQ info
    QString mType, mSN_MRQ, mFirmWareHard_MRQ, mFirmWareBoot_MRQ, mFirmWareFpga_MRQ;
    QString mSoftVer_MRQ;   //! full ver
    QString mMrqVer;        //! brief ver

    ePluginAttr mAttr;
    int mDevId;

    //! pw
    //! encrypted
    bool mbAutoLogin;
    QString mUserPw, mAdminPw;
    eUserRole mUserRole;
    eWorkingRole mWorkingRole;

};

#endif // XPLUGININTF_H
