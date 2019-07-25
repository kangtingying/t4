#ifndef TRACEPLOT_H
#define TRACEPLOT_H

#include <QWidget>
#include "../plugin/xpage.h"
namespace Ui {
class TracePlot;
}

namespace mrx_t4 {

class TracePlot : public XPage
{
    Q_OBJECT

public:
    explicit TracePlot(QWidget *parent = 0);
    ~TracePlot();

private:
    Ui::TracePlot *ui;
};

}

#endif // TRACEPLOT_H
