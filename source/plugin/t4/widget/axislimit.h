#ifndef AXISLIMIT_H
#define AXISLIMIT_H

#include <QWidget>
#include <QDoubleSpinBox>

namespace Ui {
class AxisLimit;
}

class AxisLimit : public QWidget
{
    Q_OBJECT

public:
    explicit AxisLimit(QWidget *parent = 0);
    ~AxisLimit();

public:
    void setLimitOn( bool b );
    bool limitOn();

    int setRange( int id, double l, double h );
    int range( int id, double &l, double &h );

    virtual void retranslateUi();

private:
    Ui::AxisLimit *ui;

    QList<QDoubleSpinBox *> mLSpins, mHSpins;
};

#endif // AXISLIMIT_H
