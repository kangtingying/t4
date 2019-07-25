#ifndef SYSLOGOUT_H
#define SYSLOGOUT_H

#include <QWidget>
#include <QAbstractListModel>

namespace Ui {
class SysLogout;
}

class SysLogout : public QWidget
{
    Q_OBJECT

public:
    explicit SysLogout(QWidget *parent = 0);
    ~SysLogout();

    void attachLogModel( QAbstractListModel *pModel );

    void reTranslate();

protected:
    virtual bool event( QEvent *event );

Q_SIGNALS:
    void signal_focus_in( const QString &model, const QString &help );

protected Q_SLOTS:


protected:
    virtual void focusInEvent(QFocusEvent *event);

private:
    Ui::SysLogout *ui;
//    QSize mLastSize, mCurSize;
};

#endif // SYSLOGOUT_H
