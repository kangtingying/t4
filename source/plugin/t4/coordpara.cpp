#include "coordpara.h"
#include "ui_coordpara.h"
#include "../../include/mydef.h"
CoordPara::CoordPara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordPara)
{
    ui->setupUi(this);

    ui->spinRa->setSuffix( char_deg );
    ui->spinRb->setSuffix( char_deg );
    ui->spinRc->setSuffix( char_deg );
}

CoordPara::~CoordPara()
{
    delete ui;
}

void CoordPara::setPara( double paras[6] )
{
    ui->spinPx->setValue( paras[0] );
    ui->spinPy->setValue( paras[1] );
    ui->spinPz->setValue( paras[2] );

    ui->spinRa->setValue( paras[3] );
    ui->spinRb->setValue( paras[4] );
    ui->spinRc->setValue( paras[5] );
}
void CoordPara::getPara( double paras[6] )
{
    paras[0] = ui->spinPx->value();
    paras[1] = ui->spinPx->value();
    paras[2] = ui->spinPx->value();

    paras[3] = ui->spinRa->value();
    paras[4] = ui->spinRb->value();
    paras[5] = ui->spinRc->value();
}
