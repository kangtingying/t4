#include <QtWidgets>
#include "config.h"
#include "ui_config.h"

#include "t4.h"
#include "MegaGateway.h"


#define ABS_ANGLE_TO_DEG( angle )   (360.0f*(angle))/((1<<18)-1)
#define INC_ANGLE_TO_DEG( angle )   (360.0f*(angle))/(1<<18)

#define VALUE_TO_ABS_ANGLE( val )   (quint32)( (val) * ((1<<18)-1) / 360.0f )
#define VALUE_TO_INC_ANGLE( val )   (quint32)( (val) * ((1<<18)) / 360.0f )

#define pnVALUE_TO_ABS_ANGLE( val )   (qint32)( (val) * ((1<<18)-1) / 360.0f )

//! dirs
static int _axis_enc_dirs[]={ -1, -1, 1, -1 };

namespace mrx_t4 {

Config::Config(QWidget *parent) :
    XPage(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);

    setContextHelp("config");

    //! clear all
    ui->cmbTypeTerminal->clear();
    ui->cmbTypeTerminal->addItem( QIcon(":/res/image/t4/f2.png"), tr("MRX-F2"), QVariant( (int)T4Para::e_terminal_f2 ) );
    ui->cmbTypeTerminal->addItem( QIcon(":/res/image/t4/f3.png"), tr("MRX-F3"), QVariant( (int)T4Para::e_terminal_f3  ) );
    ui->cmbTypeTerminal->addItem( QIcon(":/res/image/t4/tip.png"), tr("MRX-TIP"), QVariant( (int)T4Para::e_terminal_tip ) );
    ui->cmbTypeTerminal->addItem( QIcon(":/res/image/t4/a5.png"), tr("MRX-AXIS5"), QVariant( (int)T4Para::e_terminal_a5 ) );
    ui->cmbTypeTerminal->addItem( QIcon(":/res/image/icon/51.png"), tr("User"), QVariant( (int)T4Para::e_terminal_user ) );

    spyEdited();

    set_page_rstable();

    //! \note to the first page
    ui->tabWidget->setCurrentIndex( 0 );

    //! deg
    ui->spinZero0->setSuffix( char_deg );
    ui->spinZero1->setSuffix( char_deg );
    ui->spinZero2->setSuffix( char_deg );
    ui->spinZero3->setSuffix( char_deg );
}

Config::~Config()
{
    delete ui;
}

void Config::setOperAble( bool b )
{
    //! for each page operate able
    setOpened( b );

}
void Config::setOpened( bool b )
{
    //! for each page operate
    for ( int i = 0; i < ui->tabWidget->count(); i++ )
    {
        ui->tabWidget->widget(i)->setEnabled( b );
    }
}

#define set_zero( id )  ui->spinZero##id->setValue( selfPara->mAxisZero[id] );

#define get_zero( id )  selfPara->mAxisZero[id] = ui->spinZero##id->value();

void Config::updateUi()
{
    //! set type
    int id;
    ui->cmbTypeTerminal->setCurrentIndex( 0 );
    for ( int i = 0; i < ui->cmbTypeTerminal->count(); i++ )
    {
        id = ui->cmbTypeTerminal->itemData( i ).toInt();
        if ( id == (int)selfPara->mTerminalType )
        {
            ui->cmbTypeTerminal->setCurrentIndex( i );
            break;
        }
    }
    on_cmbTypeTerminal_currentIndexChanged( ui->cmbTypeTerminal->currentIndex() );

    if ( selfPara->mA5Range == T4Para::e_range_360 )
    { ui->rad360->setChecked( true ); }
    else
    { ui->rad270->setChecked( true ); }


    //! zero
    set_zero( 0 );
    set_zero( 1 );
    set_zero( 2 );
    set_zero( 3 )

    //! range
//    ui->lmtSafeArea->setLimitOn( selfPara->mbAxisSafeEnable );
    ui->lmtSoftLimit->setLimitOn( selfPara->mbAxisSoftEnable );
    for ( int i = 0; i < selfPara->_limit_axis_cnt; i++ )
    {
//        ui->lmtSafeArea->setRange( i, selfPara->mAxisSafeLower[i], selfPara->mAxisSafeUpper[i] );
        ui->lmtSoftLimit->setRange( i, selfPara->mAxisSoftLower[i], selfPara->mAxisSoftUpper[i] );
    }

    //! arm length
    ui->spinBase->setValue( selfPara->mArmLength[0] );
    ui->spinBA->setValue( selfPara->mArmLength[1] );
    ui->spinLA->setValue( selfPara->mArmLength[2] );

    //! slow
    ui->spinMult->setValue( selfPara->mSlowMult );
    ui->spinDiv->setValue( selfPara->mSlowDiv );
}

void Config::updateData()
{
    selfPara->mTerminalType = (T4Para::eTerminalType)ui->cmbTypeTerminal->currentData().toInt();
    selfPara->mA5Range = ui->rad360->isChecked() ? T4Para::e_range_360 : T4Para::e_range_270;

    get_zero( 0 );
    get_zero( 1 );
    get_zero( 2 );
    get_zero( 3 );

    //! range
    double h, l;
    int ret;
//    selfPara->mbAxisSafeEnable = ui->lmtSafeArea->limitOn();
    selfPara->mbAxisSoftEnable = ui->lmtSoftLimit->limitOn();
    for ( int i=0; i < selfPara->_limit_axis_cnt ; i++ )
    {
//        ret = ui->lmtSafeArea->range( i, l, h);
//        if ( ret != 0 )
//        { continue; }
        selfPara->mAxisSafeLower[ i ] = l;
        selfPara->mAxisSafeUpper[ i ] = h;

        ret = ui->lmtSoftLimit->range( i, l, h );
        if ( ret != 0 )
        { continue; }
        selfPara->mAxisSoftLower[ i ] = l;
        selfPara->mAxisSoftUpper[ i ] = h;
    }

    //! ratio
    selfPara->mSlowMult = ui->spinMult->value();
    selfPara->mSlowDiv = ui->spinDiv->value();
}

int Config::upload()
{
    check_connect_ret( -1 );

    int ret, val;
    double angle;

    //! get zero
    for ( int i = 0; i < 4; i++ )
    {
        ret = mrgMRQAbsEncoderZeroValue_Query( device_var(),
                                                i,
                                                &val );
        if ( ret != 0 )
        { return -1; }

        //! convert the value
        angle = ABS_ANGLE_TO_DEG( val );
        selfPara->mAxisZero[i] = angle;
    }

    //! limit
    double lmtL, lmtH;
    int lmtOnOff;
    bool lmtsOnOff = true;
    for ( int i = 0; i < 4; i++ )
    {
        //! down
        ret = mrgMRQAbsEncoderAlarmDownLimit_Query( device_var(),
                                        i,
                                        &val );
        if ( ret != 0 )
        { return -1; }

        //! convert the value
        lmtL = ABS_ANGLE_TO_DEG( val );
        logDbg() << i << val<<lmtL;
        //! up
        ret = mrgMRQAbsEncoderAlarmUpLimit_Query( device_var(),
                                        i,
                                        &val );
        if ( ret != 0 )
        { return -1; }

        //! convert the value
        lmtH = ABS_ANGLE_TO_DEG( val );
        logDbg() << i << val<<lmtH;
//        ui->lmtSoftLimit->setRange( i, lmtL, lmtH );

        if ( _axis_enc_dirs[i] < 0 )
        {
            selfPara->mAxisSoftLower[i] = -lmtL;
            selfPara->mAxisSoftUpper[i] = -lmtH;
        }
        else
        {
            selfPara->mAxisSoftLower[i] = lmtL;
            selfPara->mAxisSoftUpper[i] = lmtH;
        }

        ret = mrgMRQAbsEncoderAlarmState_Query( device_var(), i, &lmtOnOff );
        if ( ret != 0 )
        { return ret; }
        logDbg() << lmtsOnOff;
        lmtsOnOff = lmtsOnOff && (lmtOnOff > 0 );
    }
    selfPara->mbAxisSoftEnable = lmtsOnOff;

    //! \todo safe area

    //! slow ratio
    int a, b;
    char type[1]={0};

    int iTerminalType;
    ret = mrgRobotGetToolType( robot_var(), &iTerminalType );
    logDbg() << "terminaltype: " <<iTerminalType;
    selfPara->mTerminalType = T4Para::eTerminalType(iTerminalType);

    if( T4Para::eTerminalType(iTerminalType) == T4Para::e_terminal_user )
    {
        ret = mrgMRQMotorGearRatio_Query( device_var(),
                                    4,
                                    &a, &b );
        if ( ret == 0 )
        {
            selfPara->mSlowMult = a;
            selfPara->mSlowDiv = b;

        }else{
            return ret;
        }
    }
    else if ( iTerminalType == T4Para::e_terminal_a5 )
    {
        int mode;
        ret = mrgRobotToolExeMode_Query( robot_var(), &mode );
        if ( ret == 0 )
        {
            selfPara->mA5Range = (T4Para::eAxis5Range)mode;
        }
        else
        { sysError( "read terminal fail", e_out_log ); }
    }
    else {

    }

    //! arm length
    //! \todo
    float link[5];
    int linkCnt;
    ret = mrgGetRobotLinks( robot_var(), link, &linkCnt );
    if ( ret != 0 )
    { logDbg(); return -1; }

    for ( int al = 0; al < 3; al++ )
    {
        selfPara->mArmLength[al] = link[al];
    }

    return 0;
}
int Config::download()
{
    check_connect_ret( -1 );

    int ret, val;
    double angle;

    //! set terminal
    int type = ui->cmbTypeTerminal->currentData().toInt();

    char t[4] = {0};

    //! F2, F3, A5
    if ( type >= T4Para::e_terminal_f2 && type <= T4Para::e_terminal_a5 )
    {
        QString baStr = QString("4@%1").arg( device_var_handle() );
        ret = mrgRobotToolSet( robot_var(), type, baStr.toLatin1().data() );
        if(ret != 0){
            return -1;
        }

        //! a5
        if ( type == T4Para::e_terminal_a5 )
        {
            ret = mrgRobotToolExeMode( robot_var(), ui->rad270->isChecked() ? 0 : 1 );
        }
        else if ( type == T4Para::e_terminal_f2 )
        {
            ret = mrgRobotToolExeMode( robot_var(), 0 );
        }
        //! \todo f3
        else if ( type == T4Para::e_terminal_f3 )
        {}
        else
        {}
    }

    //! set zero
    if( ui->tabZero->isEnabled() )
    {
        QList<QDoubleSpinBox*> spins;
        spins<<ui->spinZero0<<ui->spinZero1<<ui->spinZero2<<ui->spinZero3;
        for ( int i = 0; i < 4; i++ )
        {
            angle = spins[i]->value();

            //! convert the value
            val = VALUE_TO_ABS_ANGLE( angle );

            ret = mrgMRQAbsEncoderZeroValue( device_var(),
                                                    i,
                                                    val );
            if ( ret != 0 )
            { return -1; }
        }
    }

    //! limit
    if ( ui->tabSoftLimit->isEnabled() )
    {
        double lmtL, lmtH;
        for ( int i = 0; i < 4; i++ )
        {
            ret = ui->lmtSoftLimit->range( i, lmtL, lmtH );
            if ( ret != 0 )
            { return -1; }


            if ( _axis_enc_dirs[i] > 0 )
            {
                //! down
                val = pnVALUE_TO_ABS_ANGLE( lmtL );
                ret = mrgMRQAbsEncoderAlarmDownLimit( device_var(),
                                                i,
                                                val );
                if ( ret != 0 )
                { return -1; }

                //! up
                val = pnVALUE_TO_ABS_ANGLE( lmtH );
                ret = mrgMRQAbsEncoderAlarmUpLimit( device_var(),
                                                i,
                                                val );
                if ( ret != 0 )
                { return -1; }
            }
            //! \note inverse the direction
            else
            {
                //! down
                val = pnVALUE_TO_ABS_ANGLE( lmtL );
                ret = mrgMRQAbsEncoderAlarmDownLimit( device_var(),
                                                i,
                                                -val );
                if ( ret != 0 )
                { return -1; }

                //! up
                val = pnVALUE_TO_ABS_ANGLE( lmtH );
                ret = mrgMRQAbsEncoderAlarmUpLimit( device_var(),
                                                i,
                                                -val );
                if ( ret != 0 )
                { return -1; }
            }

            //! config

            //! on off
            ret = mrgMRQAbsEncoderAlarmState( device_var(), i, ui->lmtSoftLimit->limitOn() );
        }
    }

    //! \todo soft area

    //! arm length
    if ( ui->tabArm->isEnabled() )
    {
        float links[]={
                    (float)ui->spinBase->value(),
                    (float)ui->spinBA->value(),
                    (float)ui->spinLA->value()
                };
        ret = mrgSetRobotLinks( robot_var(), links, 3 );
        if ( ret != 0 )
        { return -1; }
    }

    return 0;
}
int Config::diff()
{
    return 0;
}

void Config::spyEdited()
{
    QGroupBox *gpBox[]=
    {
    };
    QCheckBox *checkBoxes[]=
    {
//        ui->chkEn
    };
    QRadioButton *radBoxes[] = {
        ui->rad270,
        ui->rad360,

    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={
        ui->spinMult,
        ui->spinDiv
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->spinBase,
        ui->spinBA,
        ui->spinLA,

        ui->spinZero0,
        ui->spinZero1,
        ui->spinZero2,
        ui->spinZero3,
    };

    QComboBox *comboxes[]={
        ui->cmbTypeTerminal
    };

    QSlider *sliders[]
    {
    };

    install_spy();
}

void Config::retranslateUi()
{
    ui->retranslateUi( this );
    ui->lmtSoftLimit->retranslateUi();
}

void Config::updateRole()
{
    bool bEditable = m_pPlugin->isAdmin();

    ui->tabZero->setEnabled( bEditable );
    ui->tabSoftLimit->setEnabled( bEditable );
    ui->tabArm->setEnabled( bEditable );
}

void Config::on_cmbTypeTerminal_currentIndexChanged(int index)
{
    if ( NULL == m_pPlugin )
    { return; }

    int typeId = ui->cmbTypeTerminal->currentData().toInt();

    QIcon icon = ui->cmbTypeTerminal->itemIcon( index );
    if ( icon.isNull() )
    {
        ui->labelTerminalImg->setVisible( false );
        ui->gpSlow->setVisible( false );
    }
    else
    {
        ui->labelTerminalImg->setPixmap( icon.pixmap( 160,160 ) );
        ui->labelTerminalImg->setVisible( true );

        ui->gpSlow->setVisible( typeId == T4Para::e_terminal_user );
        ui->gpRange->setVisible( typeId == T4Para::e_terminal_a5 );
    }

    //! validate the terminal
    if ( typeId == T4Para::e_terminal_tip || typeId == T4Para::e_terminal_user )
    {
        Q_ASSERT( NULL != m_pPlugin );
        m_pPlugin->emit_setting_changed( (eXSetting)MRX_T4::e_setting_terminal, false );
    }
    else
    {
        Q_ASSERT( NULL != m_pPlugin );
        m_pPlugin->emit_setting_changed( (eXSetting)MRX_T4::e_setting_terminal, true );
    }
}

}
