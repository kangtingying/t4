#include "info.h"
#include "ui_info.h"
#include "../plugin/xplugin.h"
#include "../../plugin/t4/t4.h"

namespace mrx_t4 {

Info::Info(QWidget *parent) :
    XPage(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

    setContextHelp( "info" );

    //! cell items
    //! MRX-T4
    for ( int i = 0; i < ui->tableWidget->rowCount(); i++ )
    {
        for ( int j = 0; j < ui->tableWidget->columnCount(); j++ )
        {
            mTableItems.append( new QTableWidgetItem() );
            ui->tableWidget->setItem( i, j, mTableItems.last() );
        }
    }

    //! MRH-T
    for ( int i = 0; i < ui->tableWidgetT->rowCount(); i++ )
    {
        for ( int j = 0; j < ui->tableWidgetT->columnCount(); j++ )
        {
            mMRH_T.append( new QTableWidgetItem() );
            ui->tableWidgetT->setItem( i, j, mMRH_T.last() );
        }
    }

    //! translate
//    retranslateUi();

    //! MRQ
    for( int i = 0; i < ui->tableWidget2->rowCount(); i++)
    {
        for(int j = 0; j < ui->tableWidget2->columnCount(); j++)
        {
            m_T4Items.append( new QTableWidgetItem );
            ui->tableWidget2->setItem( i, j, m_T4Items.last() );
        }
    }
}

Info::~Info()
{
    delete ui;
}

void Info::updateUi()
{
    //! title config
    retranslateUi();

    Q_ASSERT( NULL != m_pPlugin );
    mTableItems[1]->setData( Qt::DisplayRole, m_pPlugin->model() );
    mTableItems[3]->setData( Qt::DisplayRole, m_pPlugin->SN() );
    mTableItems[5]->setData( Qt::DisplayRole, m_pPlugin->viewAddr() );
    mTableItems[7]->setData( Qt::DisplayRole, m_pPlugin->firmwareVer() );
    mTableItems[9]->setData( Qt::DisplayRole, m_pPlugin->mechanicalVer() );

    mMRH_T[1]->setData( Qt::DisplayRole, m_pPlugin->mrhType() );
    mMRH_T[3]->setData( Qt::DisplayRole, m_pPlugin->mrhSn() );
    mMRH_T[5]->setData( Qt::DisplayRole, m_pPlugin->mrhVer() );
    mMRH_T[7]->setData( Qt::DisplayRole, m_pPlugin->mrhBBSw() );
    mMRH_T[9]->setData( Qt::DisplayRole, m_pPlugin->mrhBBHw() );

    m_T4Items[1]->setData( Qt::DisplayRole, m_pPlugin->Type_MRQ() );
    m_T4Items[3]->setData( Qt::DisplayRole, m_pPlugin->SN_MRQ() );
    m_T4Items[5]->setData( Qt::DisplayRole, m_pPlugin->SoftVer_MRQ() );
    m_T4Items[7]->setData( Qt::DisplayRole, m_pPlugin->FirmWareHard_MRQ() );
    m_T4Items[9]->setData( Qt::DisplayRole, m_pPlugin->FirmWareBoot_MRQ() );
    m_T4Items[11]->setData( Qt::DisplayRole,m_pPlugin->FirmWareFpga_MRQ() );
}

void Info::updateRole()
{
    ui->tableWidgetT->setVisible( m_pPlugin->isAdmin() );
    ui->tableWidget2->setVisible( m_pPlugin->isAdmin() );
}

void Info::retranslateUi()
{
    ui->retranslateUi( this );

    mTableItems[0]->setData( Qt::DisplayRole, tr("Model") );
    mTableItems[2]->setData( Qt::DisplayRole, tr("SN") );
    mTableItems[4]->setData( Qt::DisplayRole, tr("Addr") );
    mTableItems[6]->setData( Qt::DisplayRole, tr("Firmware") );
    mTableItems[8]->setData( Qt::DisplayRole, tr("Mechanical") );

    mMRH_T[0]->setData( Qt::DisplayRole, tr("Model") );
    mMRH_T[2]->setData( Qt::DisplayRole, tr("SN") );
    mMRH_T[4]->setData( Qt::DisplayRole, tr("Version") );
    mMRH_T[6]->setData( Qt::DisplayRole, tr("Back board SW") );
    mMRH_T[8]->setData( Qt::DisplayRole, tr("Back board HW") );

    m_T4Items[0]->setData( Qt::DisplayRole, tr("Type") );
    m_T4Items[2]->setData( Qt::DisplayRole, tr("SN") );
    m_T4Items[4]->setData( Qt::DisplayRole, tr("Firmware") );
    m_T4Items[6]->setData( Qt::DisplayRole, tr("Hardware") );
    m_T4Items[8]->setData( Qt::DisplayRole, tr("Boot") );
    m_T4Items[10]->setData( Qt::DisplayRole, tr("FPGA") );
}

}

