#include "controllerstatus.h"
#include "ui_controllerstatus.h"


ControllerStatus::ControllerStatus(QWidget *parent) :
    XPage(parent),
    ui(new Ui::ControllerStatus)
{
    ui->setupUi(this);

    mRole = 0;

    //! connect ui
    connect( ui->chkMct,SIGNAL(toggled(bool)),
             this, SIGNAL(signal_mct_checked(bool)) );
    connect( ui->chkPwr,SIGNAL(toggled(bool)),
             this, SIGNAL(signal_device_power(bool)));
    connect( ui->btnAckError, SIGNAL(clicked(bool)),
             this, SIGNAL(signal_ack_error()) );

    spyEdited();

    IoIndicatorList << ui->radRdyEn << ui->radStart << ui->radEnable << ui->radEnabled << ui->radFault << ui->radAck << ui->radMc;
    for ( int i = 0; i < IoIndicatorList.size(); i++ )
    {
        IoIndicatorList.at( i )->setChecked( true );
    }
}

ControllerStatus::~ControllerStatus()
{
    delete ui;
}

void ControllerStatus::spyEdited()
{
    QGroupBox *gpBox[]=
    {
    };
    QCheckBox *checkBoxes[]=
    {
        ui->chkMct,
        ui->chkPwr
    };
    QRadioButton *radBoxes[] = {
    };
    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };

    QDoubleSpinBox *doubleSpinBoxes[]={


    };

    QComboBox *comboxes[]={

    };

    QSlider *sliders[]
    {
    };

    install_spy();
}

void ControllerStatus::retranslateUi()
{
    //! base
    ui->retranslateUi( this );

    ui->radRdyEn->setText( tr("RDYEN") );
    ui->radStart->setText( tr("START") );
    ui->radEnable->setText( tr("ENABLE") );
    ui->radEnabled->setText( tr("ENABLED") );

    ui->radFault->setText( tr("FAULT") );
    ui->radAck->setText( tr("ACK") );

    ui->radMc->setText( tr("MC") );

    updateRole( mRole );
}

void ControllerStatus::updateRole( int role )
{
    if ( role == 0 )
    {
        ui->labRole->setText( tr("Operator") );
    }
    else
    {
        ui->labRole->setText( tr("Administrator") );
    }
}

void ControllerStatus::setOut( SysOut e, bool b )
{}
void ControllerStatus::setWorkingMode( const QString &mode )
{ ui->edtOpMode->setText( mode ); }
//void ControllerStatus::setWarning( const QString &warning )
//{ ui->edtWarning->setText( warning ); }
//void ControllerStatus::setError( const QString &error )
//{ ui->edtError->setText( error ); }

void ControllerStatus::setRole( int role )
{
    mRole = role;
    updateRole( mRole );
}
void ControllerStatus::setWorkingStatus( const QString &status )
{ ui->labWorkingStatus->setText( status );}
void ControllerStatus::setRecordName( const QString &name )
{ ui->labReccordName->setText( name ); }

void ControllerStatus::setMctChecked( bool b )
{ ui->chkMct->setChecked( b ); }
bool ControllerStatus::isMctChecked()
{ return ui->chkMct->isChecked(); }

void ControllerStatus::setDevicePower( bool b )
{ ui->chkPwr->setChecked( b ); }
bool ControllerStatus::getDevicePower()
{ return ui->chkPwr->isChecked(); }

void ControllerStatus::setDevicePowerEnable( bool b )
{ ui->chkPwr->setEnabled( b ); }
bool ControllerStatus::isDevicePowerEnable()
{ return ui->chkPwr->isEnabled(); }

void ControllerStatus::setDeviceStatCheck(int index, bool b)
{
    IoIndicatorList.at(index)->setChecked(b);
}
