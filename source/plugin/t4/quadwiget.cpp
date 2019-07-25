#include "quadwiget.h"
#include "ui_quadwiget.h"

QuadWiget::QuadWiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuadWiget)
{
    ui->setupUi(this);
}

QuadWiget::~QuadWiget()
{
    delete ui;
}
