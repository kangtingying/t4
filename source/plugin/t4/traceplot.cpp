#include "traceplot.h"
#include "ui_traceplot.h"

namespace mrx_t4 {

TracePlot::TracePlot(QWidget *parent) :
    XPage(parent),
    ui(new Ui::TracePlot)
{
    ui->setupUi(this);
}

TracePlot::~TracePlot()
{
    delete ui;
}

}
