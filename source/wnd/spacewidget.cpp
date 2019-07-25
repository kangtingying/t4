#include "spacewidget.h"
#include "ui_spacewidget.h"

SpaceWidget::SpaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpaceWidget)
{
    ui->setupUi(this);
}

SpaceWidget::~SpaceWidget()
{
    delete ui;
}
