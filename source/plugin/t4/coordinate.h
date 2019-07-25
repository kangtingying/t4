#ifndef COORDINATE_H
#define COORDINATE_H

#include <QWidget>
#include "../plugin/xpage.h"

namespace Ui {
class Coordinate;
}

namespace mrx_t4 {

class Coordinate : public XPage
{
    Q_OBJECT

public:
    explicit Coordinate(QWidget *parent = 0);
    ~Coordinate();

public:
    virtual void updateUi();
    virtual void updateData();

    virtual int upload();
    virtual int download();
    virtual int diff();

protected:
    virtual void spyEdited();
    virtual void retranslateUi();

private slots:
    void on_cmbUserCoord_currentIndexChanged(int index);

private:
    Ui::Coordinate *ui;
};

}

#endif // COORDINATE_H
