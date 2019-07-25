#include "iostate.h"
#include "ui_iostate.h"

IoState::IoState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IoState)
{
    ui->setupUi(this);
}

IoState::~IoState()
{
    delete ui;
}

void IoState::setName( QString name )
{
    ui->label->setText( name );
    _name = name;
}

void IoState::setChecked( bool b )
{
    ui->radioButton->setChecked( b );
}

void IoState::setCheckable( bool b )
{
    ui->radioButton->setCheckable( b );
}
