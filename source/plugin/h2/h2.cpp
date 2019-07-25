#include "h2.h"

#include "info.h"
#include "h2oppanel.h"

MRX_H2::MRX_H2( QObject *parent ) : XPlugin( parent )
{
}

QTreeWidgetItem* MRX_H2::createPrefPages( QStackedWidget *stack )
{
    //! root
    m_pRootWidgetItem = new QTreeWidgetItem();
    QTreeWidgetItem *pRoot;
    pRoot = m_pRootWidgetItem;

    //! attach user role
    m_pRootWidgetItem->setData( 0, Qt::UserRole+1, QVariant::fromValue( this ) );
logDbg();
    //! pages
    XPage *pWig;
    new_root_widget( mrx_h2::Info, pWig, "MRX-H2", ":/res/image/icon/205.png", m_pRootWidgetItem );
logDbg();
    return pRoot;
}

QWidget *MRX_H2::createOpsPanel( QAbstractListModel *pModel,
                                 QWidget *parent )
{
    mrx_h2::H2OpPanel *pPanel;

    pPanel = new mrx_h2::H2OpPanel( pModel, parent );
    Q_ASSERT( NULL != pPanel );
    pPanel->setObjectName( "panel" );

    //! manage the plugin
    mPluginWidgets.append( pPanel );

    pPanel->attachPlugin( this );
    pPanel->attachWorkings();

    return pPanel;
}

