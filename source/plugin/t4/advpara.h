#ifndef ADVPARA_H
#define ADVPARA_H

#include <QWidget>
#include "../plugin/xpage.h"
namespace Ui {
class AdvPara;
}

namespace mrx_t4 {


class AdvPara : public XPage
{
    Q_OBJECT

public:
    explicit AdvPara(QWidget *parent = 0);
    ~AdvPara();
public:
    virtual void updateUi();
    virtual void updateData();
protected:
    virtual void spyEdited();
    virtual void retranslateUi();

    virtual void updateRole();
private:
    Ui::AdvPara *ui;
};

}

#endif // ADVPARA_H
