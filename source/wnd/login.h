#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = 0);
    ~LogIn();

public:
    QString getPw();
    int getUserRole();

private slots:
    void on_edtPw_textChanged(const QString &arg1);

private:
    Ui::LogIn *ui;

};

#endif // LOGIN_H
