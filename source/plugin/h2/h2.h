#ifndef MRX_H2_H
#define MRX_H2_H

#include <QObject>

#include "../plugin/xplugin.h"

#include "h2para.h"

class MRX_H2 : public XPlugin, public H2Para
{
public:
    MRX_H2( QObject *parent = nullptr );

public:
    virtual QTreeWidgetItem* createPrefPages( QStackedWidget *stack );
    virtual QWidget *createOpsPanel( QAbstractListModel *pModel,
                                     QWidget *parent=nullptr );
};

#endif // H2_H
