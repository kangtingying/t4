/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "treeitem.h"
#include "treemodel.h"
#include "iodelegate.h"
//! [0]
TreeModel::TreeModel(const QStringList &headers, const QString &fileName, QObject *parent_obj)
//    : MegaTableModel(parent_obj)
    : QAbstractItemModel( parent_obj )
{
    mHeaders.clear();
    foreach (QString header, headers)
        mHeaders << header;

    //! root item is header
    rootItem = new TreeItem( mHeaders );
    loadIn( fileName );

    //! connect
    connect( this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsInserted(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsRemoved(QModelIndex,int,int)),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(modelReset()),
             this, SIGNAL(signal_data_changed()));

    connect( this, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
             this, SIGNAL(signal_data_changed()));

}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole
            && role != Qt::EditRole
            && role != raw_data_role
            && role != valid_role )
        return QVariant();

    //! special column
    TreeItem *item = getItem(index);

    if ( NULL == item )
    { return QVariant(); }

    //! check valid role
    if ( role == valid_role )
    {
        return QVariant( item->isValid() );
    }
    else
    {}

    //! check the validate
    if ( item->isValid() )
    { }
    else if ( index.column() == 0 )
    {  }
    else
    { return QVariant(); }

    //! data
    if ( item->level() == 2 && index.column() == 0 )
    {
        int secId = item->parent()->childNumber() + 1;

        int childId = item->childNumber();

        return QString("%1.%2").arg( secId ).arg( childId + 1 );
    }
    else if ( item->level() == 1 && index.column() == 0 )
    {
        int childId = item->childNumber();
        return QString("%1").arg( childId + 1 );
    }
    //! display percent v
    else if ( index.column() == 7 )
    {
        if ( role == Qt::DisplayRole )
        { return QString("%1%").arg( item->data(index.column()).toDouble() ); }
        else
        {}
    }
    //!io delegate
    else if ( index.column() == 9 )
    {
        if ( role == Qt::DisplayRole )
        { return IODelegate::toString( item->data(index.column()).toInt(), 2 ); }
        else
        {}
    }
    else
    {}

    //! only for data
    if ( role == raw_data_role )
    { return item->data(index.column()); }

    //! else
    return item->data(index.column());
}

//! [3]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    //! only read
    if ( mReadonlyMap.contains( index.column()) && mReadonlyMap[index.column()] )
    { return QAbstractItemModel::flags(index); }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
//! [3]

//! [4]
TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
//! [4]

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal )
    {
        if ( role == Qt::DisplayRole )
        { return rootItem->data(section); }
        else
        {}
    }

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    //! \note by
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]

//! [6]
    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    //! \note by
    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

bool TreeModel::moveRows(const QModelIndex &sourceParent, int sourceRow,
                                  int count,
                                  const QModelIndex &destinationParent,
                                  int destinationChild )
{
    TreeItem *parentItem = getItem(sourceParent);
    TreeItem *parentItem2 = getItem(destinationParent);

    //! must be in the same child
//    if ( parentItem != parentItem2 )
//    { return false; }

    if ( sourceRow + count - 1 + 1 == destinationChild )
    {
        beginMoveRows( sourceParent, sourceRow,
                   sourceRow + count - 1,
                   destinationParent, destinationChild +1);
    }
    else
    {
        beginMoveRows( sourceParent, sourceRow,
                   sourceRow + count - 1,
                   destinationParent, destinationChild );
    }


        QList<TreeItem* > items;
        //! take
        //! \note invert the queue
        for ( int i = 0; i < count; i++ )
        {
            items.prepend( parentItem->takeChild( sourceRow ) );
        }

        //! append again
        if ( parentItem2->childCount() <= destinationChild )
        {
            foreach( TreeItem *theItem, items )
            {
                parentItem2->attach( theItem );
            }
        }
        else
        {
            foreach( TreeItem *theItem, items )
            {
                parentItem2->attach( theItem, destinationChild );
            }
        }

    endMoveRows();

    return true;
}

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::planeTree( const QModelIndex &index, QList<QVector<QVariant>> &varList )
{
    TreeItem *pItem;

    pItem = getItem( index );
    if ( NULL == pItem )
    { return; }

    planeTree( pItem, varList );
}

void TreeModel::planeTree( TreeItem *par, QList<QVector<QVariant>> &varList )
{
    if ( NULL == par )
    { return; }
    else
    {
        if ( par->childCount() > 0 )
        {
            //! \note trim the invalid
            if ( par->isValid() )
            { varList.append( par->dataSets() ); }

            //! for each child
            for( int i = 0; i < par->childCount(); i++ )
            {
                planeTree( par->child(i), varList );
            }
        }
        else
        {
            if ( par->isValid() )
            { varList.append( par->dataSets() ); }
        }
    }
}

void TreeModel::setColumnReadonly( int col, bool b )
{
    if ( mReadonlyMap.contains( col ) )
    { mReadonlyMap[col] = b; }
    else
    { mReadonlyMap.insert( col, b ); }
}

int TreeModel::loadIn( const QString &fileName )
{
    MDataSet dataSet;

    int ret;
    ret = dataSet.load( fileName );
    if ( ret != 0 )
    { logDbg();return ret; }

    return _loadIn( dataSet );
}

int TreeModel::exportOut( const QString &fileName )
{
    MDataSet dataSet;

    int ret;
    ret = buildDataSet( dataSet );
    if ( ret != 0 )
    { return ret; }

    return dataSet.save( fileName );
}

int TreeModel::loadIn( QTextStream &stream )
{
    MDataSet dataSet;

    int ret;
    ret = dataSet.load( stream );
    if ( ret != 0 )
    { logDbg();return ret; }

    return _loadIn( dataSet );
}

int TreeModel::exportOut( QTextStream &stream )
{
    MDataSet dataSet;

    int ret;
    ret = buildDataSet( dataSet );
    if ( ret != 0 )
    { return ret; }

    return dataSet.save( stream );
}

int TreeModel::buildDataSet( MDataSet &dataSet )
{
    dataSet.setModel( "MRX-T4" );
    QStringList headers;
    headers<<"valid"
           <<"id"<<"type"
           //<<"coordinate"<<"para"
           <<"x"<<"y"<<"z"<<"w"<<"h"<<"v"<<"mode"
           <<"do"
           <<"delay"
           <<"percent"   /* max(250) * v */
           <<"realW"
           <<"realH"
           <<"comment";
    dataSet.setHeaders( headers );

    MDataSection *pSection;
    TreeItem *pItem;
    QStringList sectionStrs;
    int ret;
    for ( int i = 0; i < rootItem->childCount(); i++ )
    {
        //! new section
        pSection = dataSet.addSection();
        if ( NULL == pSection )
        { return -1; }

        //! section content
        pItem = rootItem->child( i );
        sectionStrs.clear();
        ret = _fmtSection( pItem, sectionStrs );
        if ( ret != 0 )
        { return ret; }

        foreach( QString rowStr, sectionStrs )
        {
            if ( pSection->addRow( rowStr ) )
            {}
            else
            { return -1; }
        }
    }

    return 0;
}

void TreeModel::setTerminalSpeed(double str)
{
    mMaxTerminalSpeed = str;
}
void TreeModel::setJointSpeeds(QList<double> list )
{
    mMaxJointSpeeds = list;
}

int TreeModel::_loadIn( MDataSet &dataSet )
{
    int ret = 0;

    //! extract the data
    int secCnt = dataSet.sections();
    if ( secCnt < 1 )
    { logDbg();return -1; }

    //! get the datasets
    MDataSection *pSection;

    //! the same header
    TreeItem *pLocalRoot = new TreeItem( mHeaders );
    if ( NULL == pLocalRoot )
    { return -1; }

    for ( int i = 0; i < dataSet.sections(); i++ )
    {
        pSection = dataSet.section( i );
        if ( NULL == pSection )
        {
            ret = -1;
            break;
        }
//        logDbg()<<i<<dataSet.sections();
        ret = _loadIn( &dataSet, pSection, pLocalRoot );
        if ( ret != 0 )
        { break; }
    }

    if ( ret != 0 )
    {
        delete pLocalRoot;
    }
    else
    {

        beginResetModel();

            delete rootItem;
            rootItem = pLocalRoot;

        endResetModel();
    }
    return 0;
}

#define _get_v( colName, row, def, local )  col = pDataSet->columnIndex( colName );\
                                            if ( col < 0 ){ return -1; }\
                                            bRet = pSection->cellValue( row, col, local, def );\
                                            if ( !bRet ){ logDbg(); return -1; }

#define get_v( colName, row, def, local )   _get_v(colName, row, def, local);\
                                   vars<<local;

#define get_str( colName, row, def )   get_v( colName, row, def, strV )
#define get_float( colName, row, def )   get_v( colName, row, def, fV )
#define get_double( colName, row, def )   get_v( colName, row, def, dV )
#define get_int( colName, row, def )   get_v( colName, row, def, iV )

#define get_bool( colName, row, def )   get_v( colName, row, def, bV )
#define _get_bool( colName, row, def )   _get_v( colName, row, def, bV )

int TreeModel::_loadIn( MDataSet *pDataSet,
                        MDataSection *pSection,
                        TreeItem *pParent )
{
    TreeItem *pItem = NULL;
    TreeItem *pFirst;
    bool bRet;
    int col;

    QString strV;
    int iV;
    double dV;
    float fV;
    bool bV;

    //! section
    QVector<QVariant> vars;
    for ( int i = 0; i < pSection->rows(); i++ )
    {
        vars.clear();

        //! col
        col = 0;

        get_int( "id", i, 0 );
        get_str( "type", i, "PA" );
//        get_str( "coordinate", i, "" );
//        get_str( "para", i, "" );
//        get_int( i, 0 );


        get_double( "x",i, 0 );
        get_double( "y",i, 0 );
        get_double( "z",i, 0 );
        get_double( "w",i, 0 );
        get_double( "h",i, 0 );

        //get_double( "v",i, 0 );
        get_double( "percent",i, 0 );
        get_bool( "mode",i, 0 );

        get_int( "do", i, 0 );
        get_double( "delay",i, 0 );

        get_str( "comment",i, 0 );

        if ( i == 0 )
        {
            pFirst = new TreeItem( vars, pParent );
            pItem = pFirst;
        }
        else
        { pItem = new TreeItem( vars, pFirst ); }

        if ( NULL == pItem )
        { return -1; }

        //! valid
        _get_bool( "valid", i, true );
        pItem->setValid( bV );
    }

    return 0;
}

int TreeModel::_fmtSection( TreeItem *section,
                 QStringList &ary )
{
    QString str;

    int ret = 0;
    ret = _fmtItem( section, str );
    if ( ret != 0 )
    { return ret; }

    ary<<str;

    //! for the child
    for( int i = 0; i < section->childCount(); i++ )
    {
        ret = _fmtItem( section->child(i), str );
        if ( ret != 0 )
        { return ret; }

        ary<<str;
    }

    return 0;
}

int TreeModel::_fmtItem( TreeItem *pItem,
                         QString &ary )
{
    ary = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16")
            .arg( pItem->isValid() )
            .arg( pItem->data(0).toInt() )
            .arg( pItem->data(1).toString() )
            .arg( pItem->data(2).toString() )
            .arg( pItem->data(3).toString() )
            .arg( pItem->data(4).toDouble() )
            .arg( pItem->data(5).toDouble() )
            .arg( pItem->data(6).toDouble() )
            //.arg( pItem->data(7).toDouble() )
            .arg( pItem->data(7).toDouble() * mMaxTerminalSpeed / 100 )
            .arg( pItem->data(8).toInt() )
            .arg( pItem->data(9).toInt() )
            .arg( pItem->data(10).toDouble() )
            .arg( pItem->data(7).toDouble() )
            .arg( pItem->data(7).toDouble() * mMaxJointSpeeds[3] / 100 )
            .arg( pItem->data(7).toDouble() * mMaxJointSpeeds[4] / 100 )
            .arg( pItem->data(11).toString() )
            ;
    return 0;
}

