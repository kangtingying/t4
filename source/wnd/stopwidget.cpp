#include "stopwidget.h"
#include "ui_stopwidget.h"

StopWidget::StopWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StopWidget)
{
    ui->setupUi(this);

    connect( ui->toolButton, SIGNAL(clicked(bool)),
             this, SIGNAL(signal_stop_clicked(bool)) );
}

StopWidget::~StopWidget()
{
    delete ui;
}
