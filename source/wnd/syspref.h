#ifndef SYSPREF_H
#define SYSPREF_H

#include <QDialog>
#include <QAbstractButton>

#include "syspara.h"

namespace Ui {
class SysPref;
}

class SysPref : public QDialog, public SysPara
{
    Q_OBJECT

public:
    explicit SysPref(QWidget *parent = 0);
    SysPref( const SysPara &para, QWidget *parent = 0 );

    ~SysPref();

public:
    virtual int exec();

protected:
    void updateUi();
    void updateData();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SysPref *ui;
};

#endif // SYSPREF_H
