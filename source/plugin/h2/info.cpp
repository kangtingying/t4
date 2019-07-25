#include "info.h"
#include "ui_h2info.h"

namespace mrx_h2 {

Info::Info(QString strDevInfo, QWidget *parent) :
    XPage(parent),
    ui(new Ui::H2Info)
{
    ui->setupUi(this);

////    setFocuHelpName( "Product" );

//    QStringList strListDev = strDevInfo.split(',', QString::SkipEmptyParts);

//    QString strIP;
//    QString strID = strListDev.at(0);
//    if(strID.left(3) == "USB")
//    {
//        QStringList lst = strID.split('_', QString::SkipEmptyParts);
//        strIP = lst.at(1) + "_" + lst.at(2);
//    }
//    else{
//        strIP = strID;
//    }

//    this->m_IP      = strIP;
//    this->m_Type    = strListDev.at(2);
//    this->m_SN      = strListDev.at(3);
//    this->m_Version = strListDev.at(4);

//    ui->label_ip->setText(this->m_IP);
//    ui->label_sn->setText(this->m_SN);
//    ui->label_type->setText(this->m_Type);
//    ui->label_version->setText(this->m_Version);

//    change_online_status(false);
}

Info::~Info()
{
    delete ui;
}

void Info::on_pushButton_status_clicked()
{
    emit signal_online_clicked(m_IP);
}

void Info::change_online_status(bool bl)
{
    if(bl){
        ui->pushButton_status->setIcon(QIcon(":/res/image/h2product/connect.png"));
    }else{
        ui->pushButton_status->setIcon(QIcon(":/res/image/h2product/disconnect.png"));
    }
}

int Info::readDeviceConfig()
{
    return 0;
}

int Info::writeDeviceConfig()
{

    return 0;
}

int Info::loadConfig()
{

    return 0;
}

int Info::saveConfig()
{

    return 0;
}

void Info::updateShow()
{

}

void Info::translateUI()
{
    ui->retranslateUi(this);
}

}
