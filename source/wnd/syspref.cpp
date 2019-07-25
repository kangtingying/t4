#include "syspref.h"
#include "ui_syspref.h"

SysPref::SysPref(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysPref)
{
    ui->setupUi(this);
}

SysPref::SysPref( const SysPara &para, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::SysPref)
{
    ui->setupUi(this);

    //! copy
    mbAutoExpand = para.mbAutoExpand;
    mbAutoLoad = para.mbAutoLoad;
    mbAutoMax = para.mbAutoMax;
    mbAutoSearch = para.mbAutoSearch;

    mbAutoRpp = para.mbAutoRpp;

    mIntfIndex = para.mIntfIndex;
    mRefreshIndex = para.mRefreshIndex;
    mStyleIndex = para.mStyleIndex;

    mPlugins = para.mPlugins;
}

SysPref::~SysPref()
{
    delete ui;
}

int SysPref::exec()
{
    updateUi();

    int ret = QDialog::exec();

    updateData();

    return ret;
}

void SysPref::updateUi()
{
    ui->chkAutoExpand->setChecked( mbAutoExpand );
    ui->chkAutoLoad->setChecked( mbAutoLoad );
    ui->chkAutoSearch->setChecked( mbAutoSearch );
    ui->chkMax->setChecked( mbAutoMax );

    ui->chkAutoRpp->setChecked( mbAutoRpp );
}
void SysPref::updateData()
{
    mbAutoExpand = ui->chkAutoExpand->isChecked();
    mbAutoLoad = ui->chkAutoLoad->isChecked();
    mbAutoSearch = ui->chkAutoSearch->isChecked();
    mbAutoMax = ui->chkMax->isChecked();

    mbAutoRpp = ui->chkAutoRpp->isChecked();
}

void SysPref::on_buttonBox_clicked(QAbstractButton *button)
{
    //! reset
    if ( ui->buttonBox->buttonRole( button ) ==  QDialogButtonBox::ResetRole )
    {
        reset();

        updateUi();
    }
}
