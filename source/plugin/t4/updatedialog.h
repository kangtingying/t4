#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractButton>
#include <QDialog>
#include <QProcess>
#include "MegaGateway.h"
#include "../plugin/xplugin.h"
#include <QSemaphore>


#define MRHT_SYSTEM_ERROR       -1      /* backbord update error */
#define NET_ERROR               -2
#define MRHT_FILE_INVALID       -3
#define CONFIG_ERROR            -4
#define CONNECT_FAIL            -5      /* "Error:Cannot establish communication! Please check the connection !" */
#define BOOT_OLD                -6      /* "Error:The BOOT version is very old,Cannot update to this version!" */
#define MRQ_File_INVALID        -7      /* "Error:The update file is invalid !" */
#define GET_BOOT_VERS_FAIL      -8      /* "Error:Cannot get the BOOT version!" */
#define RELOAD_FAIL             -9      /* "Error:Reload failed !" */
#define GET_DEV_SOFTVER_FAIL    -10     /* "Error:Cannot get the software version of the device !" */
#define ARM_FILE_INVALID        -11     /* "Error:ARM file has validation error, file may be corrupted,Stop updating!" */
#define BOOT_DOWN_FAIL          -12     /* "Error:BOOT data download failed !" */
#define NOT_RESPOND             -13     /* "Error:Does not respond" */
#define FPGA1_FILE_INVALID      -14     /* "Error:FPGA1 file has validation error, file may be corrupted,Stop updating!" */
#define ERASE_FLASH_FAIL        -15     /* "Error:Erase the FLASH failed!" */
#define FPGA2_FILE_INVALID      -16     /* "Error:FPGA2 file has validation error, file may be corrupted,Stop updating!" */
#define OPEN_MRH_FAIL           -18     /* "Notify:Open MRH-T Failed" */
#define DEVICE_UPDATE_SUCCESS   9       /* "Notify:Update Complete!" */
#define MRHT_UPDATE_SUCCESS     0
#define MRQ_BEGIN_UPDATE        1
#define MRHT_BEGIN_UPDATE       2


namespace Ui {
class Widget;
}
class XPlugin;
class Entity;
class MThead;
class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget *parent = 0);
    ~UpdateDialog();

    void attatchPlugin(XPlugin *xp);

protected:
    int openDevice();

    int loadRemoteInfo( int vi );

    int parseUpdateFile(QByteArray &in );

signals:

    //! control thread
    //! 0: standby  1:begin
    void changeTheadWorkMode( int mode );

public slots:
    void updateUi( int i );
    void updateProgress(QString );

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    int versionComparison(const QString &fileVer);

    void on_toolButton_clicked();

    void on_lineEdit_textChanged(const QString &s);

private:
    Ui::Widget *ui;

    QStatusBar *pStatusBar;

    Entity *m_mrqEntity, *m_mrhEntity;

    QByteArray mRemoteVerionStream;

    QString m_addr,recvID;
    XPlugin *m_pPlugin;
    QString m_desc;
    QString sPath;
    int m_robotID;
    bool isAdmin;

    MThead *pWorkThead;

};

#endif // WIDGET_H

class MThead: public QThread
{
    Q_OBJECT

public:
    MThead(QObject *parent = 0);

    void setAddr( QString str ){ m_addr = str; }

    void attachEntity_MRQ( Entity *e ){ pEntity_MRQ = e; }
    void attachEntity_MRH( Entity *e ){ PEntity_MRH = e; }

    virtual int generateDevUpdateFile();

    virtual int updateDevice();
    virtual int updateController();

    void attatchPlugin(XPlugin *xp);

protected:
    virtual void run();

signals:
    void resultReady( QString );
    void resultReady( int );

private:
    QProcess *pProc;

    QString m_addr;
    int vi;

    int iFlag;

    Entity *pEntity_MRQ;
    Entity *PEntity_MRH;

    int iProgress;

    //! flat mrq update complete
    int iEndFlag;

    QSemaphore *pSemaphore;

    XPlugin *pXPlugin;

signals:
    void sigStandOutput( QByteArray &ba );

private slots:
    void slotReadyRead();

public slots:
    void switchWorkMode( int mode );

    void parseStandOutput();
};

//! entity
class Entity : public QObject
{
    Q_OBJECT
public:
    enum ID{
        MRH_ENTITY,
        MRQ_ENTITY,
        MCT_ENTITY,
        PACKAGE
    };

public:
    int mDescLen;
    QString mDescription;
    quint32 mId;
    quint32 mSize;
    quint32 mCheck;
    quint32 mFormat;
    quint32 mSections;
    QByteArray mPayload;

public:
    Entity(QObject *parent = nullptr);
};
