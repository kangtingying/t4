#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class aboutDlg;
}

class aboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDlg(
                       QWidget *parent = 0);
    ~aboutDlg();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event );

private:
    Ui::aboutDlg *ui;
};

#endif // ABOUTDLG_H
