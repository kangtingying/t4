#ifndef IOSTATE_H
#define IOSTATE_H

#include <QWidget>

namespace Ui {
class IoState;
}

class IoState : public QWidget
{
    Q_OBJECT

public:
    explicit IoState(QWidget *parent = 0);
    ~IoState();

    void setName(QString name );
    QString Name() const { return _name;}

    void setChecked( bool b );
    void setCheckable( bool b );

private:
    Ui::IoState *ui;

    QString _name;
};

#endif // IOSTATE_H
