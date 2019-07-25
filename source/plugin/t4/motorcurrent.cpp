#include "motorcurrent.h"
#include "ui_motorcurrent.h"

MotorCurrent::MotorCurrent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorCurrent)
{
    ui->setupUi(this);
}

MotorCurrent::~MotorCurrent()
{
    delete ui;
}

void MotorCurrent::setData( double dC, double iC)
{
    ui->spinDriver->setValue( dC );
    ui->spinIdle->setValue( iC );
}
void MotorCurrent::getData( double &dC, double &iC)
{
    dC = ui->spinDriver->value();
    iC = ui->spinIdle->value();
}

QDoubleSpinBox *MotorCurrent::doubleSpinCurrent()
{ return ui->spinDriver; }
QDoubleSpinBox *MotorCurrent::doubleSpinIdleCurrent()
{ return ui->spinIdle; }
