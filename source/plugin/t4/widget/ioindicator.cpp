#include "ioindicator.h"
#include "ui_ioindicator.h"

IoIndicator::IoIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IoIndicator)
{
    ui->setupUi(this);

    mIOIndiction<<"L"<<"H";

    connect( ui->radioButton, SIGNAL(clicked(bool)),
             this, SIGNAL(signal_clicked()));
    connect( ui->radioButton, SIGNAL(clicked(bool)),
             this, SLOT(slot_rad_clicked()) );
}

IoIndicator::~IoIndicator()
{
    delete ui;
}

void IoIndicator::setClickAble( bool b )
{ ui->radioButton->setEnabled( b );}
bool IoIndicator::isClickAble()
{ return ui->radioButton->isEnabled(); }

void IoIndicator::setIndication( const QStringList &indi )
{
    mIOIndiction = indi;
}

void IoIndicator::setChecked( bool b )
{
    ui->radioButton->setChecked(b);

    ui->radioButton->setText( mIOIndiction.at( (int)b ));
}
bool IoIndicator::isChecked( )
{ return ui->radioButton->isChecked(); }

void IoIndicator::setText( const QString &text )
{ ui->label->setText( text ); }
QString IoIndicator::text()
{ return ui->label->text(); }

void IoIndicator::slot_rad_clicked()
{
    ui->radioButton->setText( mIOIndiction.at( isChecked() ));
}
