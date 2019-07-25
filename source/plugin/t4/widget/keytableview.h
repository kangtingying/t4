#ifndef KEYTABLEVIEW_H
#define KEYTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>

class KeyTableView : public QTableView
{
    Q_OBJECT
public:
    KeyTableView( QWidget *parent = Q_NULLPTR );

protected:
    virtual void keyPressEvent( QKeyEvent *event );

Q_SIGNALS:
    void signal_key_delete();
    void signal_key_insert();
};

#endif // KEYTABLEVIEW_H
