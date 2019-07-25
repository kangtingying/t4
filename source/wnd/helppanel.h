#ifndef HELPPANEL_H
#define HELPPANEL_H

#include <QDialog>

namespace Ui {
class HelpPanel;
}

class HelpPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HelpPanel(QWidget *parent = 0);
    ~HelpPanel();

public:
    void setFile( const QString &path );
    QString file( );

protected Q_SLOTS:
    void slot_file_changed();

private:
    Ui::HelpPanel *ui;

    QString mPath;
};

#endif // HELPPANEL_H
