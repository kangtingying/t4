#ifndef IOINDICATOR_H
#define IOINDICATOR_H

#include <QWidget>

namespace Ui {
class IoIndicator;
}

class IoIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit IoIndicator(QWidget *parent = 0);
    ~IoIndicator();

    void setClickAble( bool b );
    bool isClickAble();

    void setIndication( const QStringList &indi );

    void setChecked( bool b );
    bool isChecked( );

    void setText( const QString &text );
    QString text();

Q_SIGNALS:
    void signal_clicked();

protected Q_SLOTS:
    void slot_rad_clicked();

private:
    Ui::IoIndicator *ui;

    QStringList mIOIndiction;
};

#endif // IOINDICATOR_H
