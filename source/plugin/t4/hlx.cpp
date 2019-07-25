#include "hlx.h"
#include "ui_hlx.h"

HLX::HLX(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HLX)
{
    ui->setupUi(this);
}

HLX::~HLX()
{
    delete ui;
}

void HLX::setName( const QString &name)
{ ui->label->setText( name ); }
QString HLX::name()
{ return ui->label->text(); }

void HLX::setValue( HLX::hlxValue val )
{ ui->comboBox->setCurrentIndex( (int)val );}
HLX::hlxValue HLX::value()
{ return(HLX::hlxValue)ui->comboBox->currentIndex(); }
