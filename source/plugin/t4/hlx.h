#ifndef HLX_H
#define HLX_H

#include <QWidget>

namespace Ui {
class HLX;
}

class HLX : public QWidget
{
    Q_OBJECT
public:
    enum hlxValue
    {
        e_hlx_h,
        e_hlx_l,
        e_hlx_x,
    };

public:
    explicit HLX(QWidget *parent = 0);
    ~HLX();

public:
    void setName( const QString &name);
    QString name();

    void setValue( HLX::hlxValue val );
    HLX::hlxValue value();

private:
    Ui::HLX *ui;
};

#endif // HLX_H
