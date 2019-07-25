#ifndef STOPWIDGET_H
#define STOPWIDGET_H

#include <QWidget>

namespace Ui {
class StopWidget;
}

class StopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StopWidget(QWidget *parent = 0);
    ~StopWidget();

Q_SIGNALS:
    void signal_stop_clicked( bool );

private:
    Ui::StopWidget *ui;
};

#endif // STOPWIDGET_H
