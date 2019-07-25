#ifndef ERRORMGRTABLE_H
#define ERRORMGRTABLE_H

#include <QWidget>
#include <QAbstractItemModel>

#include "../plugin/xpage.h"

#include "../model/delegate/checkdelegate.h"
#include "../model/delegate/comboxdelegate.h"

namespace Ui {
class ErrorMgrTable;
}

namespace mrx_t4 {

class ErrorMgrTable : public XPage
{
    Q_OBJECT
public:
//    static int calcResponse(int reaction, int outputAble);
//    static int parseResponse(int response,int *reaction, int *outputAble);

    enum eActionEvent
    {
        e_event_post_load = Qt::UserRole + 1
    };

public:
    explicit ErrorMgrTable(QWidget *parent = 0);
    ~ErrorMgrTable();

protected:
    virtual void retranslateUi();
    virtual void onPluginChanged();

public:
    void setModel( QAbstractTableModel *pModel );

public:
    virtual void rst();

    virtual int upload();
    virtual int download();
    virtual int diff();

protected:
    void doSave();
    void doLoad();

private:
    Ui::ErrorMgrTable *ui;

    CheckDelegate *m_pCheckDelegate;
//    *m_pRadioDelegate;
//    ComboxDelegate *m_pComboxDelegate;

protected Q_SLOTS:
//    void slot_request_save();
//    void slot_request_load();

    void slot_data_changed();
private slots:
    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);
};
}

#endif // ERRORMGRTABLE_H
