#ifndef LOGOUTWIDGET_H
#define LOGOUTWIDGET_H

#include <QListWidget>
#include <QMenu>
#include <QContextMenuEvent>

namespace Ui {
class LogoutWidget;
}

class LogoutWidget : public QListView
{
    Q_OBJECT

public:
    explicit LogoutWidget(QWidget *parent = 0);
    ~LogoutWidget();

    virtual void setModel(QAbstractItemModel *model);
protected:
    void setupUi();
    void buildConnection();
    void retranslateUi();

protected:


protected Q_SLOTS:
    void slot_current_changed( int );
    void slot_contextmenu( const QPoint &pt );

    void slot_clear();
    void slot_selectall();
    void slot_copy();


private:
    Ui::LogoutWidget *ui;

    QMenu *m_pContextMenu;

    QAction *m_pClearAction, *m_pSelectAllAction, *m_pCopyAction;
};

#endif // LOGOUTWIDGET_H
