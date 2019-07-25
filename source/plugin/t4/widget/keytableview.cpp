#include "keytableview.h"

KeyTableView::KeyTableView( QWidget *parent ) : QTableView( parent )
{

}

void KeyTableView::keyPressEvent( QKeyEvent *event )
{
    if ( event->key()==Qt::Key_Insert )
    {
        emit signal_key_insert();
    }
    else if ( event->key()==Qt::Key_Delete )
    {
        emit signal_key_delete();
    }
    else
    { QTableView::keyPressEvent( event ); }
}
