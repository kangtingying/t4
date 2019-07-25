#include "h2status.h"
#include "ui_h2status.h"
#include <QTimer>

namespace mrx_h2 {

H2Status::H2Status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::H2Status)
{
    ui->setupUi(this);

    connect(ui->chkMct,SIGNAL(toggled(bool)),
            this,SIGNAL(signal_mct_checked(bool)));

    connect(ui->chkPwr,SIGNAL(toggled(bool)),
            this,SIGNAL(signal_power_checked(bool)));

    connect(ui->btnAckError, SIGNAL(clicked(bool)),this, SIGNAL(signal_btnAckError_clicked()));
}

H2Status::~H2Status()
{
    delete ui;
}

void H2Status::changeLanguage(QString qmFile)
{
    //  翻译文件
    qApp->removeTranslator(&m_translator);
    m_translator.load(qmFile);
    qApp->installTranslator(&m_translator);
    ui->retranslateUi(this);
}

void H2Status::on_chkMct_toggled(bool checked)
{
    ui->chkMct->setChecked(checked);

    if(!checked){
        ui->chkPwr->setChecked(false);
        ui->chkPwr->setEnabled(false);
    }
    else{
        ui->chkPwr->setEnabled(true);
        ui->chkPwr->setChecked(false);
    }
}

void H2Status::set_chkMct_enabled(bool isEnabled)
{
    ui->chkMct->setEnabled(isEnabled);
}

}

