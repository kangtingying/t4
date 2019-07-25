#ifndef SPACEWIDGET_H
#define SPACEWIDGET_H

#include <QWidget>

namespace Ui {
class SpaceWidget;
}

class SpaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpaceWidget(QWidget *parent = 0);
    ~SpaceWidget();

private:
    Ui::SpaceWidget *ui;
};

#endif // SPACEWIDGET_H
