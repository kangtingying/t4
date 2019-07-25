#include "motorconfig.h"
#include "ui_motorconfig.h"
#include "t4.h"

#include "MegaGateway.h"

namespace mrx_t4 {

MotorConfig::MotorConfig(QWidget *parent) :
    XPage(parent),
    ui(new Ui::MotorConfig)
{
    ui->setupUi(this);

    setContextHelp( "motor" );

    spyEdited();

    set_page_rstable();

    spySetting( MRX_T4::e_setting_terminal );
}

MotorConfig::~MotorConfig()
{
    delete ui;
}

//#define set_current( id )   ui->doubleSpinBox##id->setValue( selfPara->mAxisCurrents[id] );
//#define get_current( id )   selfPara->mAxisCurrents[id] = ui->doubleSpinBox##id->value();

#define set_current( id )   ui->current##id->setData( selfPara->mAxisCurrents[id],\
                                                      selfPara->mAxisIdleCurrents[id]\
                                                        );

#define get_current( id )   ui->current##id->getData( dc, ic);\
                            selfPara->mAxisCurrents[id] = dc;\
                            selfPara->mAxisIdleCurrents[id] = ic;

void MotorConfig::updateUi()
{
    set_current( 0 );
    set_current( 1 );
    set_current( 2 );
    set_current( 3 );
    set_current( 4 );

    //! tip
    ui->current4->setVisible( selfPara->mTerminalType != T4Para::e_terminal_tip );
    ui->label->setVisible( selfPara->mTerminalType != T4Para::e_terminal_tip );
}

void MotorConfig::updateData()
{
    double dc, ic, st;

    get_current( 0 );
    get_current( 1 );
    get_current( 2 );
    get_current( 3 );

    get_current( 4 );
}

int MotorConfig::upload()
{
    check_connect_ret( -1 );

    int ret;

    QDoubleSpinBox *spins[]={
//        ui->doubleSpinBox0,
//        ui->doubleSpinBox1,
//        ui->doubleSpinBox2,
//        ui->doubleSpinBox3,
//        ui->doubleSpinBox4,
    };

    MotorCurrent *currentWidgets[]=
    {
        ui->current0,
        ui->current1,
        ui->current2,
        ui->current3,

        ui->current4,
    };

    float currents[ pRobo->_axis_cnt ], iCurrents[pRobo->_axis_cnt], sTimes[pRobo->_axis_cnt];
    for ( int i = 0; i < pRobo->_axis_cnt; i++ )
    {
        ret = mrgMRQDriverCurrent_Query( device_var(),
                                         i,
                                         currents + i );
        if ( ret != 0 )
        { return ret; }

        ret = mrgMRQDriverIdleCurrent_Query( device_var(),
                                             i,
                                             iCurrents + i );
        if ( ret != 0 )
        { return ret; }

        //! \range
        pRobo->mAxisCurrents[ i ]  = currents[i];
        pRobo->mAxisIdleCurrents[ i ] = iCurrents[i];
    }

    return 0;
}
int MotorConfig::download()
{
    check_connect_ret( -1 );

    int ret;

    MotorCurrent *spins[]={
        ui->current0,
        ui->current1,
        ui->current2,
        ui->current3,

        ui->current4,
    };

    double dc,ic;
    for ( int i = 0; i < pRobo->_axis_cnt; i++ )
    {
        //! terminaltype tip
        if( i == 4 && selfPara->mTerminalType == T4Para::e_terminal_tip )
            continue;

        spins[i]->getData( dc, ic);

        ret = mrgMRQDriverCurrent( device_var(),
                                   i,
                                   dc );
        if ( ret != 0 )
        { return ret; }

        ret = mrgMRQDriverIdleCurrent( device_var(),
                                   i,
                                   ic );
        if ( ret != 0 )
        { return ret; }
    }

    return 0;
}

void MotorConfig::onSetting(XSetting setting)
{
    XPage::onSetting(setting);

    if ( (int)setting.mSetting == (int)MRX_T4::e_setting_terminal )
    {
        if ( setting.mPara1.isValid() )
        {}
        else
        { return; }

        ui->current4->setVisible( setting.mPara1.toBool() );
        ui->label->setVisible( setting.mPara1.toBool() );
    }else{}
}

#define current_spins( id ) ui->current##id->doubleSpinCurrent(),\
                            ui->current##id->doubleSpinIdleCurrent()
void MotorConfig::spyEdited()
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
    };

    QDoubleSpinBox *doubleSpinBoxes[]={
            current_spins(0),
            current_spins(1),
            current_spins(2),
            current_spins(3),
            current_spins(4)
    };

    QComboBox *comboxes[]={

    };

    QSlider *sliders[]
    {
    };

    install_spy();
}

void MotorConfig::retranslateUi()
{ ui->retranslateUi( this ); }

void MotorConfig::updateRole()
{
    bool bEditable = m_pPlugin->isAdmin();

    setEnabled( bEditable );
}

}
