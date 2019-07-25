#ifndef QUADWIGET_H
#define QUADWIGET_H

#include <QWidget>

namespace Ui {
class QuadWiget;
}

class QuadWiget : public QWidget
{
    Q_OBJECT

public:
    explicit QuadWiget(QWidget *parent = 0);
    ~QuadWiget();

private:
    Ui::QuadWiget *ui;
};

#endif // QUADWIGET_H
