#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>
#include "../plugin/xpage.h"
namespace Ui {
class Config;
}

namespace mrx_t4 {

class Config : public XPage
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

    virtual void setOperAble( bool b );
    virtual void setOpened( bool b );

    virtual void updateUi();
    virtual void updateData();

    virtual int upload();
    virtual int download();
    virtual int diff();

protected:
    virtual void spyEdited();
    virtual void retranslateUi();

    virtual void updateRole();

private slots:
    void on_cmbTypeTerminal_currentIndexChanged(int index);

private:
    Ui::Config *ui;
};

//enum TERMINAL_TYPE {
//    MRX_F2,
//    MRX_F3,
//    MRX_TIP,
//    MRX_AXIS5,
//    USER
//};

}

#endif // CONFIG_H
