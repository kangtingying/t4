#include "advpara.h"
#include "ui_advpara.h"
#include <QtWidgets>

#include "t4.h"
#include "../../../include/mydef.h"

namespace mrx_t4 {


AdvPara::AdvPara(QWidget *parent) :
    XPage(parent),
    ui(new Ui::AdvPara)
{
    ui->setupUi(this);

    setContextHelp("advpara");

    spyEdited();

    set_page_rstable();

    ui->spinHomeSpeed->setSuffix( QString("%1/s").arg( char_deg )  );
    ui->spinAcc->setSuffix( QString("mm/s%1").arg( char_square ) );
    ui->spinJerk->setSuffix( QString("mm/s%1").arg( char_trible ) );

    ui->spinJoint0Speed->setSuffix( QString("%1/s").arg( char_deg ) );
    ui->spinJoint1Speed->setSuffix( QString("%1/s").arg( char_deg ) );
    ui->spinJoint2Speed->setSuffix( QString("%1/s").arg( char_deg ) );
    ui->spinJoint3Speed->setSuffix( QString("%1/s").arg( char_deg ) );
    ui->spinJoint4Speed->setSuffix( QString("%1/s").arg( char_deg ) );
}

AdvPara::~AdvPara()
{
    delete ui;
}

void AdvPara::updateUi()
{
    MRX_T4 *pRobo = (MRX_T4*)m_pPlugin;
    Q_ASSERT( NULL != pRobo );

//    ui->spinHomeSpeed->setValue( pRobo->mHomeSpeed );
//    ui->spinHomeTimeout->setValue( pRobo->mHomeTimeout );

//    ui->spinVel->setValue( pRobo->mDefSpeed );
//    ui->spinAcc->setValue( pRobo->mDefAcc );

//    ui->spinJointSpeed->setValue( pRobo->mMaxJointSpeed );
//    ui->spinTerminalSpeed->setValue( pRobo->mMaxTerminalSpeed );

    bool bDir = true;

    exchange_spin( spinHomeSpeed, pRobo->mHomeSpeed, bDir );
    exchange_spin( spinHomeTimeout, pRobo->mHomeTimeout, bDir );

    exchange_spin( spinAcc, pRobo->mMaxAcc, bDir );
    exchange_spin( spinJerk, pRobo->mMaxJerk, bDir );
    exchange_spin( spinAutoAcc, pRobo->mAutoAcc, bDir );

    exchange_spin( spinJoint0Speed, pRobo->mMaxJointSpeeds[0], bDir );
    exchange_spin( spinJoint1Speed, pRobo->mMaxJointSpeeds[1], bDir );
    exchange_spin( spinJoint2Speed, pRobo->mMaxJointSpeeds[2], bDir );
    exchange_spin( spinJoint3Speed, pRobo->mMaxJointSpeeds[3], bDir );
    exchange_spin( spinJoint4Speed, pRobo->mMaxJointSpeeds[4], bDir );

    exchange_spin( spinTerminalSpeed, pRobo->mMaxTerminalSpeed, bDir );
}

void AdvPara::updateData()
{
    MRX_T4 *pRobo = (MRX_T4*)m_pPlugin;
    Q_ASSERT( NULL != pRobo );

    bool bDir = false;

    exchange_spin( spinHomeSpeed, pRobo->mHomeSpeed, bDir );
    exchange_spin( spinHomeTimeout, pRobo->mHomeTimeout, bDir );

    exchange_spin( spinAcc, pRobo->mMaxAcc, bDir );
    exchange_spin( spinJerk, pRobo->mMaxJerk, bDir );
    exchange_spin( spinAutoAcc, pRobo->mAutoAcc, bDir );

    exchange_spin( spinJoint0Speed, pRobo->mMaxJointSpeeds[0], bDir );
    exchange_spin( spinJoint1Speed, pRobo->mMaxJointSpeeds[1], bDir );
    exchange_spin( spinJoint2Speed, pRobo->mMaxJointSpeeds[2], bDir );
    exchange_spin( spinJoint3Speed, pRobo->mMaxJointSpeeds[3], bDir );
    exchange_spin( spinJoint4Speed, pRobo->mMaxJointSpeeds[4], bDir );

    exchange_spin( spinTerminalSpeed, pRobo->mMaxTerminalSpeed, bDir );
}

void AdvPara::spyEdited()
{
    QGroupBox *gpBox[]=
    {
    };
    QCheckBox *checkBoxes[]=
    {
//        ui->chkEn
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinAutoAcc
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinHomeSpeed,
        ui->spinHomeTimeout,

        ui->spinAcc,
        ui->spinJerk,

        ui->spinJoint0Speed,
        ui->spinJoint1Speed,
        ui->spinJoint2Speed,
        ui->spinJoint3Speed,
        ui->spinJoint4Speed,

        ui->spinTerminalSpeed,
    };

    QComboBox *comboxes[]={
//        ui->cmbInterpMode,
    };

    QSlider *sliders[]
    {
    };

    install_spy();

//    foreach( QLineEdit *pEdit, mAxesEdits )
//    {
//        spy_control_edit( pEdit );
//    }
}

void AdvPara::retranslateUi()
{ ui->retranslateUi( this ); }

void AdvPara::updateRole()
{
    setEnabled( m_pPlugin->isAdmin() );
}

}
