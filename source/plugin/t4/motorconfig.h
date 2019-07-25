#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

#include <QWidget>
#include "../plugin/xpage.h"
namespace Ui {
class MotorConfig;
}

namespace mrx_t4 {

class MotorConfig : public XPage
{
    Q_OBJECT

public:
    explicit MotorConfig(QWidget *parent = 0);
    ~MotorConfig();

    virtual void updateUi();
    virtual void updateData();

    virtual int upload();
    virtual int download();

    void onSetting( XSetting setting );
protected:
    virtual void spyEdited();
    virtual void retranslateUi();

    virtual void updateRole();

private:
    Ui::MotorConfig *ui;
};

}

#endif // MOTORCONFIG_H
