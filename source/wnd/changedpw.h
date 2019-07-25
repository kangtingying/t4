#ifndef CHANGEDPW_H
#define CHANGEDPW_H

#include <QDialog>

namespace Ui {
class ChangedPw;
}

class ChangedPw : public QDialog
{
    Q_OBJECT

public:
    explicit ChangedPw(QWidget *parent = 0);
    ~ChangedPw();
public:
    QString getOldPw();
    QString getNewPw();

protected Q_SLOTS:
    void slot_updateControl();

private slots:

private:
    Ui::ChangedPw *ui;
};

#endif // CHANGEDPW_H
