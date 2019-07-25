#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include "../plugin/plugin/xpage.h"
namespace Ui {
class WelcomePage;
}

class WelcomePage : public XPage
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = 0);
    ~WelcomePage();

private:
    Ui::WelcomePage *ui;

};

#endif // WELCOMEPAGE_H
