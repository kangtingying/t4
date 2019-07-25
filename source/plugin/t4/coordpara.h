#ifndef COORDPARA_H
#define COORDPARA_H

#include <QWidget>

namespace Ui {
class CoordPara;
}

class CoordPara : public QWidget
{
    Q_OBJECT

public:
    explicit CoordPara(QWidget *parent = 0);
    ~CoordPara();

public:
    void setPara( double paras[6] );
    void getPara( double paras[6] );

private:
    Ui::CoordPara *ui;
};

#endif // COORDPARA_H
