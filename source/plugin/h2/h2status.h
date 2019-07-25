#ifndef H2STATUS_H
#define H2STATUS_H

#include <QTranslator>
#include <QWidget>

namespace Ui {
class H2Status;
}

namespace mrx_h2 {

class H2Status : public QWidget
{
    Q_OBJECT

public:
    explicit H2Status(QWidget *parent = 0);
    ~H2Status();

    void changeLanguage(QString qmFile);

signals:
    void signal_mct_checked(bool);
    void signal_power_checked(bool);
    void signal_btnAckError_clicked();

public slots:
    void on_chkMct_toggled(bool checked);
    void set_chkMct_enabled(bool isEnabled);

private:
    Ui::H2Status *ui;
    QTranslator m_translator;
};

}

#endif // H2STATUS_H
