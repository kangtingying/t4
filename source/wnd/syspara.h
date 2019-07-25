#ifndef SYSPARA_H
#define SYSPARA_H

#include <QtCore>

enum SysUserMode
{
    sys_user_operator,
    sys_user_administrator,
};

class SysPara
{
public:
    SysPara();

protected:
    void init();
public:
    void reset();

public:
    int save( const QString &fileName );
    int load( const QString &fileName );

public:
    int refreshIntervalMs();

protected:
    int serialOut( QXmlStreamWriter &writer );
    int serialIn( QXmlStreamReader &reader );

public:
    //! \note change the constructor
    int mLangIndex;
    int mStyleIndex;

    bool mbAutoExpand, mbAutoLoad, mbAutoSearch;
    bool mbAutoMax;
    bool mbAutoRpp;
    int mIntfIndex;
    int mRefreshIndex;

//    int mSysMode;

    QByteArray mPw;

    //! addr,model,sn,fw,mw,mrqtype,mrq_sn,mrq_sw,mrq_hw,mrq_boot,mrq_fpga,mrq_id
    QStringList mPlugins;

};

#endif // SYSPARA_H
