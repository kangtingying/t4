#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include <QTableWidgetItem>
#include "../plugin/xpage.h"
namespace Ui {
class Info;
}

namespace mrx_t4 {

class Info : public XPage
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = 0);
    ~Info();

public:
    virtual void updateUi();
    virtual void updateRole();

protected:
    virtual void retranslateUi();

private:
    Ui::Info *ui;

    QList<QTableWidgetItem *> mTableItems;  /* MRH */
    QList<QTableWidgetItem *> mMRH_T;       /* MRH-T */
    QList<QTableWidgetItem *> m_T4Items;    /* MRQ */
};

}
#endif // INFO_H
