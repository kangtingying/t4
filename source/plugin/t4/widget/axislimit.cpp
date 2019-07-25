#include "axislimit.h"
#include "ui_axislimit.h"

AxisLimit::AxisLimit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxisLimit)
{
    ui->setupUi(this);

    mLSpins<<ui->spinL0<<ui->spinL1<<ui->spinL2<<ui->spinL3;
    mHSpins<<ui->spinU0<<ui->spinU1<<ui->spinU2<<ui->spinU3;
}

AxisLimit::~AxisLimit()
{
    delete ui;
}

void AxisLimit::setLimitOn( bool b )
{
    ui->gpLimit->setChecked( b );
}
bool AxisLimit::limitOn()
{
    return ui->gpLimit->isChecked();
}

int AxisLimit::setRange( int id, double l, double h )
{
    if ( id < 0 || id >= mLSpins.size() )
    { return -1; }

    mLSpins.at( id )->setValue( l );
    mHSpins.at( id )->setValue( h );

    return 0;
}
int AxisLimit::range( int id, double &l, double &h )
{
    if ( id < 0 || id >= mLSpins.size() )
    { return -1; }

    l = mLSpins.at( id )->value();
    h = mHSpins.at( id )->value();

    return 0;
}
void AxisLimit::retranslateUi()
{
    ui->retranslateUi(this);
}

