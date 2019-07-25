#include "megainterface.h"
#include "ui_megainterface.h"

#include <QByteArray>
#include <QDebug>
#include <QMessageBox>

#include "MegaGateway.h"
#include "sysapi.h"
#include "xthread.h"
#include "../include/mystd.h"
#include "../plugin/plugin/xplugin.h"

#define ICON_WIDTH      64
#define ICON_HEIGHT     64

QString MegaInterface::megaMrxTypeToString( int type )
{
    if ( type == MRX_TYPE_T4 )
    { return "MRX-T4"; }
    else if ( type == MRX_TYPE_H2 )
    { return "MRX-H2"; }
    else
    { return ""; }
}

MegaInterface::MegaInterface(SysPara *pPara, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MegaInterface), m_pPara( pPara )
{
    ui->setupUi(this);

    m_devType = TYPE_LAN;
    m_menu = NULL;

    setupMenu();

    buildConnection();

    retranslateUi();

    m_model = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setDefaultAlignment( Qt::AlignLeft );
    ui->tableView->horizontalHeader()->setDefaultSectionSize( 150 );

    //! context menu
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(slotShowContextmenu(const QPoint&)));

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(slotSelectDevices()));

//    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
//            this, SLOT(close()));
    connect( ui->buttonBox, SIGNAL(rejected()),
             this, SLOT(close()) );

    //! interface type
    ui->comboBox_DevType->setCurrentIndex( m_pPara->mIntfIndex );
    on_comboBox_DevType_currentIndexChanged( ui->comboBox_DevType->currentIndex() );

    //! \note disable usb
    ui->comboBox_DevType->setEnabled( false );
}

MegaInterface::~MegaInterface()
{
    delete ui;
}

void MegaInterface::setupMenu()
{
    m_menu = new QMenu( this );
    Q_ASSERT( NULL != m_menu );

    m_pActionOpen = m_menu->addAction( tr("Identify ON") );
    m_pActionClose = m_menu->addAction( tr("Identify OFF") );

    Q_ASSERT( NULL != m_pActionOpen );
    Q_ASSERT( NULL != m_pActionClose );
}
void MegaInterface::buildConnection()
{
    connect( m_pActionOpen, SIGNAL(triggered(bool)),
             this, SLOT(soltActionOpen()));
    connect( m_pActionClose, SIGNAL(triggered(bool)),
             this, SLOT(soltActionClose()));
}
void MegaInterface::retranslateUi()
{
    m_pActionOpen->setText( tr("Identify ON") );
    m_pActionClose->setText( tr("Identify OFF") );
}

void MegaInterface::clearListView()
{
    for(int i=0; i<m_itemList.count(); i++)
    {
        delete m_itemList.at(i);
    }
    m_itemList.clear();
    m_model->clear();
}

int MegaInterface::deviceOpen()
{
    //! current
    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().at(0);
    QString strAddr = m_model->data(index,Qt::DisplayRole).toString();
    QString fullAddr = QString("TCPIP0::%1::inst0::INSTR").arg(strAddr);

    //! open
    int visa =  mrgOpenGateWay( 1, fullAddr.toLocal8Bit().data(), 3000 );
    if(visa <= 0){
        QMessageBox::critical(this,tr("error"),tr("open device error"));
    }
    return visa;
}

void MegaInterface::slotDeviceScanEnd()
{
    ui->pushButton_Scan->setEnabled(true);
    ui->progressBar->setMaximum(100);
    ui->progressBar->hide();
}

void MegaInterface::slotShowSearchResult( QVariant var )
{
    if ( var.isValid() )
    {}
    else
    { return; }

    QStringList strListHeader;
    if(m_devType == TYPE_LAN)
    {
        strListHeader<< tr("IP")
                     << tr("Type")
                     << tr("SN")
                     << tr("Version")
                     << tr("Model");
    }
    else if(m_devType == TYPE_USB)
    {
        strListHeader<<tr("USBID")
                     << tr("Type")
                     << tr("SN")
                     << tr("Version")
                     << tr("Model");
    }
    else
    { Q_ASSERT(false); }

    m_model->setHorizontalHeaderLabels(strListHeader);

    QStandardItem *t_item;
    int rowCnt = 0;
    foreach( const RoboInfo &info, mSearchRobos )
    {
        QString strIP = info.mAddr.split("::").at(1);
        t_item = new QStandardItem( strIP );
        t_item->setData( strIP, Qt::ToolTipRole );
        m_model->setItem( rowCnt, 0, t_item );

        t_item = new QStandardItem( info.mFMModel );
        t_item->setData( info.mFMModel, Qt::ToolTipRole );
        m_model->setItem( rowCnt, 1, t_item );

        t_item = new QStandardItem( info.mFMSN );
        t_item->setData( info.mFMSN, Qt::ToolTipRole );
        m_model->setItem( rowCnt, 2, t_item );

        t_item = new QStandardItem( info.mFMVer );
        t_item->setData( info.mFMVer, Qt::ToolTipRole );
        m_model->setItem( rowCnt, 3, t_item );

        t_item = new QStandardItem( info.mRoboModel );
        t_item->setData( info.mRoboModel, Qt::ToolTipRole );
        m_model->setItem( rowCnt, 4, t_item );

//        t_item = new QStandardItem( info.mId );
//        m_model->setItem( rowCnt, 5, t_item );

        rowCnt++;
    }

    ui->tableView->setColumnWidth( 0, 200 );
}

void MegaInterface::slotShowContextmenu(const QPoint& pos)
{
    //! selected
    if(!((ui->tableView->selectionModel()->selectedIndexes()).empty()))
    {
        Q_ASSERT( NULL != m_menu );

        m_menu->exec(QCursor::pos());

        ui->tableView->selectionModel()->clear();
    }
}

void MegaInterface::soltActionOpen()
{
    int visa = deviceOpen();
    if(visa <= 0)
        return;

    mrgIdentify(visa, 1);

    mrgCloseGateWay(visa);
}

void MegaInterface::soltActionClose()
{
    int visa = deviceOpen();
    if(visa <= 0)
        return;

    mrgIdentify(visa, 0);
    mrgCloseGateWay(visa);
}
void MegaInterface::slotSelectDevices()
{
    //! only one item
    QString strAddr = m_model->data( m_model->index(  ui->tableView->currentIndex().row(), 0 ), Qt::DisplayRole ).toString();
    QString strFullAddr = QString("TCPIP0::%1::inst0::INSTR").arg(strAddr);

    //QString strModel = m_model->data( m_model->index(  ui->tableView->currentIndex().row(), 4 ), Qt::DisplayRole ).toString();
    QString strModel = m_model->data( m_model->index(  ui->tableView->currentIndex().row(), 4 ), Qt::DisplayRole ).toString();

    QString firmwareVer = m_model->data( m_model->index(  ui->tableView->currentIndex().row(), 3 ), Qt::DisplayRole ).toString();
    QString sn = m_model->data( m_model->index(  ui->tableView->currentIndex().row(), 2 ), Qt::DisplayRole ).toString();

    //! \todo get the mechanical ver from the protocol
    int iRow = ui->tableView->currentIndex().row();
    QString mechanicalVer = mSearchRobos.at(iRow).mDevInfo.mFirmWareHard;
    //！\todo devtype
    QString typeMRQ = mSearchRobos.at(iRow).mDevInfo.mType;
    QString snMRQ = mSearchRobos.at(iRow).mDevInfo.mSN;
    QString softVer = mSearchRobos.at(iRow).mDevInfo.mSoftVer;
    QString firmWareHard = mSearchRobos.at(iRow).mDevInfo.mFirmWareHard;
    QString FirmWareBoot = mSearchRobos.at(iRow).mDevInfo.mFirmWareBoot;
    QString FirmWareFpga = mSearchRobos.at(iRow).mDevInfo.mFirmWareFpga;
    int devId = mSearchRobos.at(iRow).mDevInfo.mId;

    //! append the item
    //! "addr,model,sn,firmwareVer,mechanicalVer"
    //!
    QStringList strList;
    strList<<QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg( strFullAddr).arg( strModel ).arg(sn).arg(firmwareVer).arg(mechanicalVer).arg(typeMRQ).arg(snMRQ).arg(softVer).arg(firmWareHard).arg(FirmWareBoot).arg(FirmWareFpga).arg(devId);
    logDbg()<<strList;

    emit signalSelectedInfo(strList);
}

void MegaInterface::on_buttonBox_clicked(QAbstractButton *button)
{
    QDialogButtonBox::ButtonRole role = ui->buttonBox->buttonRole( button );
    if ( QDialogButtonBox::AcceptRole == role )
    {
        slotSelectDevices();
    }
//    close();
}

void MegaInterface::on_pushButton_Scan_clicked()
{
//    auto func = [this]( QString &strRet )
//    auto func = [this]( QList<RoboInfo> &robos )
    auto func = [this]( void *ptr )
    {
        Q_ASSERT( NULL != ptr );
        QList<RoboInfo> *pRoboList = (QList<RoboInfo> *)ptr;

        QString strFindDevices = "";

        char buff[4096] = "";

        int ret = 0;
        if(m_devType == TYPE_LAN)
        { ret += mrgFindGateWay(1, buff, sizeof(buff) ); }
        else if(m_devType == TYPE_USB)
        { ret += mrgFindGateWay(3, buff, sizeof(buff) ); }
        else
        { return; }

        if( ret <= 0 ){
                    sysError("Find GateWay fail");
                    return;
                }

        //! split
        strFindDevices = QString("%1").arg(buff);
        //! removeDuplicates
        QStringList l = strFindDevices.split(',', QString::SkipEmptyParts);
        l.removeDuplicates();

        int idCnt;
        RoboInfo tInfo;
        foreach ( QString strDevice, l )
        {
            sysInfo( strDevice );

            int visa =  mrgOpenGateWay(1, strDevice.toLocal8Bit().data(), 2000);
            if(visa <= 0)
            {
                sysError( strDevice + " " + tr("Open fail!")  );
                continue;
            }

            do
            {
                //! idn query
                char IDN[128] = "";
                int ret = mrgGateWayIDNQuery(visa,IDN);
                if(ret != 0)
                {
                    break;
                }

                tInfo.mAddr = strDevice;
                tInfo.mRawInfo = QString("%1").arg( IDN );

                //! split the idn
                QStringList infoList;
                infoList = tInfo.mRawInfo.split(',', QString::SkipEmptyParts );
                if ( infoList.size() == 4 )
                {}
                else
                { break; }
                tInfo.mFMModel = infoList.at( 1 );
                tInfo.mFMSN = infoList.at(2);
                tInfo.mFMVer = infoList.at(3);

                //! device name
                int roboIds[1025];
                idCnt = mrgGetRobotName( visa, roboIds );
                if ( idCnt < 1 )
                { break; }

                QString strType;
                for ( int i = 0; i < idCnt; i++ )
                {
                    logDbg()<<roboIds[i];
                    ret = mrgGetRobotType( visa, roboIds[i] );
                    if ( ret < 0 || ret >= MRX_TYPE_UNKOWN )
                    { continue; }
                    else
                    {  }

                    strType = megaMrxTypeToString( ret );
                    if ( strType.length() < 1 )
                    { continue; }

                    tInfo.mId = roboIds[i];
                    tInfo.mRoboModel = QString("%1").arg( strType );
                    tInfo.mRoboSN = "";

//                    sysInfo( IDN );
                    sysInfo( QString("%1 %2 %3").arg(IDN).arg( tInfo.mRoboModel).arg( tInfo.mId ) );

                    int devName[128];
                    ret = mrgGetRobotDevice(visa, roboIds[i], devName);
                    if(ret <= 0){
                        continue;
                    }else{}

                    //! \note invalid sn
                    char buf[128];
                    ret = mrgGetDeviceInfo(visa, devName[0], buf);
                    if(ret != 0){
                        continue;
                    }else{}

                    QString t( buf );
                    QStringList tList = t.split(":");

                    //! \note the sn
                    tInfo.mFMSN = tList.at(0);

                    //! the mrq sn
                    tInfo.mDevInfo.mSN = tList.at(0);
                    tInfo.mDevInfo.mFirmWareHard = tList.at(1);
                    tInfo.mDevInfo.mSoftVer = tList.at(2);
                    tInfo.mDevInfo.mFirmWareBoot = tList.at(3);
                    tInfo.mDevInfo.mFirmWareFpga = tList.at(4);
                    tInfo.mDevInfo.mId = devName[0];

                    char bufDevType[128];
                    ret = mrgGetDeviceType(visa,devName[0], bufDevType);
                    if(ret!=0)
                        continue;
                    tInfo.mDevInfo.mType = QString::fromLocal8Bit(bufDevType);

                    //! append
                    pRoboList->append( tInfo );
                }

            }while( 0 );

            mrgCloseGateWay(visa);
        }

        return;
    };

    //! init
    clearListView();

    mSearchRobos.clear();

    //! attach
    XThread *thread = new XThread( func, &mSearchRobos );

    connect(thread, SIGNAL(finished()), this, SLOT(slotDeviceScanEnd()));
    connect(thread, SIGNAL(signalFinishResult( QVariant ) ),
            this, SLOT(slotShowSearchResult( QVariant )));

    ui->pushButton_Scan->setEnabled(false);
    ui->progressBar->setMaximum(0);
    ui->progressBar->show();

    thread->start();
}

void MegaInterface::on_comboBox_DevType_currentIndexChanged(int index)
{
    QIcon icon = ui->comboBox_DevType->itemIcon( index );
    if( icon.isNull() )
    { ui->label_icon->setVisible(false);}
    else
    {
        ui->label_icon->setVisible( true );
        ui->label_icon->setPixmap( icon.pixmap( ICON_WIDTH, ICON_HEIGHT ) );

        //! save the setting
        m_pPara->mIntfIndex = index;
        emit signal_setting_changed();
    }
}
