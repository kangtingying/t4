#include "welcomepage.h"
#include "ui_welcomepage.h"

WelcomePage::WelcomePage(QWidget *parent) :
    XPage(parent),
    ui(new Ui::WelcomePage)
{
    setContextHelp("welcome");

    ui->setupUi(this);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}
