#ifndef DLGIO_H
#define DLGIO_H

#include <QDialog>

namespace Ui {
class DlgIO;
}

namespace mrx_t4 {

class DlgIO : public QDialog
{
    Q_OBJECT

public:
    static QString fmtDo( quint32 _do, int w = 4 );
    static QString fmtDi( quint32 _di, int w = 4 );

public:
    explicit DlgIO(QWidget *parent = 0);
    ~DlgIO();

public:
    void setDo( quint32 _do );
    quint32 getDo();

    void setDi( quint32 _di );
    quint32 getDi();

private:
    Ui::DlgIO *ui;

protected:
    quint32 mDo, mDi;
};

}

#endif // DLGIO_H
