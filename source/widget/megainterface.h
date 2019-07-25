#ifndef MEGAINTERFACE_H
#define MEGAINTERFACE_H

#include <QDialog>
#include <QMenu>
#include <QVariant>
#include <QStandardItemModel>
#include <QWidget>
#include <QMetaType>
#include <QAbstractItemView>
#include <QTableWidget>
#include <QAbstractButton>

#include "syspara.h"

namespace Ui {
class MegaInterface;
}

enum _DEV_TYPES
{
    TYPE_LAN = 2,
    TYPE_USB = 3
};

class DevInfo
{
public:
    int mId;
    QString mType;
    QString mSN;
    QString mSoftVer;
    QString mFirmWareHard;
    QString mFirmWareBoot;
    QString mFirmWareFpga;
};

class RoboInfo
{
public:
    QString mRawInfo;

    QString mAddr;

    QString mFMModel, mFMSN, mFMVer;

    QString mRoboSN;
    QString mRoboModel;

    int mId;

    DevInfo mDevInfo;
};

class MegaInterface : public QDialog
{
    Q_OBJECT

public:
    static QString megaMrxTypeToString( int type );

public:
    explicit MegaInterface(SysPara *pPara, QWidget *parent = 0);
    ~MegaInterface();

protected:
    void setupMenu();
    void buildConnection();
    void retranslateUi();

    void clearListView();
    int deviceOpen();

signals:
    //! stringlist
    //! devinfo,model
    void signalSelectedInfo(const QStringList &strDevInfo );
    void signal_setting_changed();

private slots:
    void slotDeviceScanEnd();
//    void slotShowSearchResult(QString strDevices);

    void slotShowSearchResult( QVariant var );

    void slotShowContextmenu(const QPoint &pos);
    void soltActionOpen();
    void soltActionClose();

    void slotSelectDevices();
    void on_buttonBox_clicked(QAbstractButton *button);

public slots:
    void on_pushButton_Scan_clicked();

    void on_comboBox_DevType_currentIndexChanged(int index);


private:
    Ui::MegaInterface *ui;
    QStandardItemModel* m_model;
    QList<QStandardItem *> m_itemList;

    QMenu *m_menu;
    QAction *m_pActionOpen, *m_pActionClose;

    int m_devType;

    QList<RoboInfo > mSearchRobos;


    SysPara *m_pPara;
};


#endif // MEGAINTERFACE_H
