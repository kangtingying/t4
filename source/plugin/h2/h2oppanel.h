#ifndef H2OP_PANEL_H
#define H2OP_PANEL_H

#include <QToolButton>
#include <QWidget>

#include "../plugin/xpage.h"

//#include "diagnosismodel.h"
//#include "debugmodel.h"
//#include "xconfig.h"
//#include "megasplinechart.h"

//#include "doublespinboxdelegate.h"

namespace Ui {
class H2Ops;
}

namespace mrx_h2 {

class H2OpPanel : public XPage
{
    Q_OBJECT
public:
    explicit H2OpPanel(QAbstractListModel *pModel, QWidget *parent = 0);
    ~H2OpPanel();

private:
    Ui::H2Ops *ui;
};

}

#endif // H2OPS_H
