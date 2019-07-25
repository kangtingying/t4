#include "xpluginintf.h"
#include "MegaGateway.h"
#include "../../../include/mydebug.h"

#define def_user_pw     "megar2019"
#define def_admin_pw    "mega2019_advance"

XPluginIntf::XPluginIntf( QObject *parent ) : QObject( parent )
{
    mVi = -1;

    mAttr = plugin_attr_none;

    mbOperateAble = true;
    mbOnLine = true;
    mbWorking = false;

    mWorkingRole = working_unk;

    rstAuthority();
}

void XPluginIntf::rstAuthority()
{
    //! default
    mbAutoLogin = true;
    mUserPw = def_user_pw;
    mAdminPw = def_admin_pw;
    mUserRole = user_user;
}

//! addr
//! model
//! sn
bool XPluginIntf::isEqual( XPluginIntf *p )
{
    return isEqual( p->addr(), p->model(), p->SN() );
}

bool XPluginIntf::isEqual( const QString &_addr,
              const QString &_model,
              const QString &_sn )
{
//    if ( QString::compare( addr(), _addr, Qt::CaseInsensitive) != 0 )
//    { return false; }

    if ( QString::compare( model(), _model, Qt::CaseInsensitive) != 0 )
    { return false; }

    if ( QString::compare( SN(), _sn, Qt::CaseInsensitive) != 0 )
    { return false; }

    return true;
}


//! \see SysPara plugin
QString XPluginIntf::description()
{
    QStringList strList;

    strList<<addr()<<model()<<SN()<<firmwareVer()<<mechanicalVer()
           <<Type_MRQ()
           <<SN_MRQ()
           <<SoftVer_MRQ()
           <<FirmWareHard_MRQ()
           <<FirmWareBoot_MRQ()
           <<FirmWareFpga_MRQ()
           <<QString::number( DevId() );
    return strList.join(',');
}

void XPluginIntf::setOperateAble( bool b )
{
    mbOperateAble = b;

    onOperateAble( b );
}
bool XPluginIntf::operateAble()
{ return mbOperateAble; }

void XPluginIntf::onOperateAble( bool b )
{}

void XPluginIntf::setOnLine( bool b )
{
    mbOnLine = b;

    onOnLine( b );
}
bool XPluginIntf::onLine()
{ return mbOnLine; }

void XPluginIntf::onOnLine( bool b )
{ }

void XPluginIntf::setWorking( bool b )
{ mbWorking = b; }
bool XPluginIntf::isWorking()
{ return mbWorking; }

void XPluginIntf::setModel( const QString &model )
{ mModel = model; }
QString XPluginIntf::model()
{ return mModel; }

void XPluginIntf::setAddr( const QString &addr )
{ mAddr = addr; }
QString XPluginIntf::addr()
{ return mAddr; }

void XPluginIntf::setViewAddr( const QString &vAddr )
{ mViewAddr = vAddr; }
QString XPluginIntf::viewAddr()
{ return mViewAddr; }

void XPluginIntf::setSN( const QString &sn )
{ mSN = sn; }
QString XPluginIntf::SN()
{ return mSN; }

void XPluginIntf::setFirmwareVer( const QString &ver )
{ mFirmwareVer = ver; }
QString XPluginIntf::firmwareVer()
{ return mFirmwareVer; }

void XPluginIntf::setMechanicalVer( const QString &ver )
{ mMechanicalVer = ver; }
QString XPluginIntf::mechanicalVer()
{ return mMechanicalVer; }

void XPluginIntf::setMRHType( const QString &type )
{ mMRHType = type; }
QString XPluginIntf::mrhType()
{ return mMRHType; }

void XPluginIntf::setMRHSN( const QString &sn )
{ mMRHSN = sn; }
QString XPluginIntf::mrhSn()
{ return mMRHSN; }

void XPluginIntf::setMRHVer( const QString &ver )
{ mMRHVer = ver; }
QString XPluginIntf::mrhVer()
{ return mMRHVer; }

void XPluginIntf::setMRHBBSw( const QString &sw )
{ mMRHBBSw = sw; }
QString XPluginIntf::mrhBBSw()
{ return mMRHBBSw; }

void XPluginIntf::setMRHBBHw( const QString &hw )
{ mMRHBBHw = hw; }
QString XPluginIntf::mrhBBHw()
{ return mMRHBBHw; }

void XPluginIntf::setPw( XPluginIntf::eUserRole role, const QString &pw )
{
    do
    {
        if ( role == XPluginIntf::user_user )
        { mUserPw = pw; }
        else if ( role == XPluginIntf::user_admin )
        { mAdminPw = pw; }
        else
        { break; }

        emit signal_request_pw_save();
    }while( 0 );
}
QString XPluginIntf::getPw( XPluginIntf::eUserRole role, bool &bOk )
{
    bOk = true;
    if ( role == XPluginIntf::user_user )
    { return mUserPw; }
    else if ( role == XPluginIntf::user_admin )
    { return mAdminPw; }
    else
    {
        bOk = false;
        return QString();
    }
}

void XPluginIntf::rstPw( eUserRole role )
{
    do
    {
        if ( role == XPluginIntf::user_user )
        { mUserPw = def_user_pw; }
        else if ( role == XPluginIntf::user_admin )
        { mAdminPw = def_admin_pw; }
        else
        { break; }

        emit signal_request_pw_save();
    }while( 0 );
}

void XPluginIntf::setAutoLogin( bool b )
{
    mbAutoLogin = b;

    emit signal_request_pw_save();
}
bool XPluginIntf::isAutoLogin()
{
    return mbAutoLogin;
}

int XPluginIntf::save( QXmlStreamWriter &writer )
{
    writer.writeTextElement( "model", mModel );
    writer.writeTextElement( "addr", mAddr );

    return 0;
}
int XPluginIntf::load( QXmlStreamReader &reader )
{
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "model" )
        {
            if ( mModel.length() > 0 )
            { reader.skipCurrentElement(); }
            else
            { mModel = reader.readElementText(); }
        }
        else if ( reader.name() == "addr" )
        {
            if ( mAddr.length() > 0 )
            { reader.skipCurrentElement(); }
            else
            { mAddr = reader.readElementText(); }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return 0;
}

int XPluginIntf::savePw( const QString &path, const QString &name )
{
    QByteArray theAry;

    QXmlStreamWriter writer( &theAry );

    writer.writeStartDocument();

        writer.writeStartElement("pw");
            writer.writeTextElement( "login", QString::number(mbAutoLogin) );
            writer.writeTextElement( "user", mUserPw );
            writer.writeTextElement( "admin", mAdminPw );
        writer.writeEndElement();

    writer.writeEndDocument();

    //! compress
    QByteArray encData;
    encData = qCompress( theAry );

    //! write data
    int ret;
    ret = mrgStorageWriteFile( deviceVi(),
                               0,
                               path.toLatin1().data(),
                               name.toLatin1().data(),
                               (quint8*)encData.data(),
                               encData.length() );

    return ret;
}

int XPluginIntf::loadPw( const QString &path, const QString &name )
{
    int ret;

    //! \todo need query the file size
    QByteArray theData;
    theData.reserve( 1024 * 1024 );

    ret = mrgStorageReadFile( deviceVi(),
                              0,
                              path.toLatin1().data(),
                              name.toLatin1().data(),
                              (quint8*)theData.data() );
    if ( ret <= 0 )
    { return -1; }

    theData.resize( ret );

    //! decompress
    QByteArray rawData;
    rawData = qUncompress( theData );
//    rawData = theData;

    //! deparse the data
    ret = -2;
    QXmlStreamReader reader( rawData );
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "pw" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "login" )
                {
                    mbAutoLogin = reader.readElementText().toInt() > 0;
                    logDbg()<<mbAutoLogin;
                }
                else if ( reader.name() == "user" )
                {
                    mUserPw = reader.readElementText();
                    logDbg()<<mUserPw;
                }
                else if ( reader.name() == "admin" )
                {
                    mAdminPw = reader.readElementText();
                    ret = 0;
                    logDbg()<<mAdminPw;
                }
                else
                {}
            }
        }
        else
        { reader.skipCurrentElement(); }
    }

    return ret;
}

void XPluginIntf::emit_user_role_change()
{
}

int XPluginIntf::open()
{ return -1; }
void XPluginIntf::close()
{}

int XPluginIntf::stop()
{ return 0; }

int XPluginIntf::fStop()
{ return 0; }

bool XPluginIntf::isOpened()
{ return ( mVi > 0 ); }

#define has_attr( attr )    ( (mAttr & attr ) == attr )
bool XPluginIntf::isFoldable()
{
//    return (mAttr & plugin_attr_foldable) == plugin_attr_foldable;
    return has_attr( plugin_attr_foldable );
}

bool XPluginIntf::isRebootable()
{ return has_attr( plugin_attr_reboot_able ); }
bool XPluginIntf::isPowerOffable()
{ return has_attr( plugin_attr_poweroff_able ); }

bool XPluginIntf::isOnLine()
{ return true; }

void XPluginIntf::attachVi( int vi )
{ mVi = vi; }
int XPluginIntf::deviceVi()
{ return mVi; }

void XPluginIntf::lockVi()
{ mRWMutex.lock(); }
void XPluginIntf::unlockVi()
{ mRWMutex.unlock(); }

void XPluginIntf::setUserRole( eUserRole role )
{
    mUserRole = role;

    //! \note changed
    emit_user_role_change();
}
XPluginIntf::eUserRole XPluginIntf::userRole()
{ return mUserRole; }

bool XPluginIntf::isAdmin()
{ return mUserRole == user_admin; }

void XPluginIntf::setWorkingRole( XPluginIntf::eWorkingRole role )
{ mWorkingRole = role; }
XPluginIntf::eWorkingRole XPluginIntf::workingRole()
{ return mWorkingRole; }


