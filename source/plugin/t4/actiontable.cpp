#include <float.h>
#include "actiontable.h"
#include "ui_actiontable.h"

#include "../../plugin/t4/t4.h"
#include "MegaGateway.h"
#include "t4oppanel.h"  //! for SequenceItem

#include "../model/treeitem.h"
#include "../model/treemodel.h"

namespace mrx_t4 {

ActionTable::ActionTable(QWidget *parent) :
    XPage(parent),
    ui(new Ui::ActionTable)
{
    ui->setupUi(this);

    mRawEditTriggers = ui->view->editTriggers();

    ui->view->setColumnWidth( 0, 20 );
    for ( int i = 1; i < 11; i++ )
    {
        ui->view->setColumnWidth( i, 20 );
    }

    setContextHelp( "record" );

    set_page_rstable();

    setUri( "record" );

    m_pDelegateXYZ = new dSpinDelegate( -1000, 1000, this );
    m_pDelegatePw = new dSpinDelegate( -360, 360, this );
    m_pDelegateH = new dSpinDelegate( -360, 360, this );
    m_pDelegateV = new dSpinDelegate( 1, 100, this );
    m_pDelegateV->setSuffix( "%" );

    m_pODelegate = new IODelegate( 2 );

    m_pDelegateTime = new dSpinDelegate( 0, 1000, this );
    m_pDelegateTime->setSuffix( "s" );

    m_pCheckDelegate = new CheckDelegate( shape_check, this );
    m_pCheckDelegate->setAlignment( Qt::AlignCenter );
    //! \note set treemodel
    m_pCheckDelegate->setVisibleRole( (Qt::ItemDataRole)(Qt::UserRole + 2) );

    ui->view->setItemDelegateForColumn( 2, m_pDelegateXYZ );
    ui->view->setItemDelegateForColumn( 3, m_pDelegateXYZ );
    ui->view->setItemDelegateForColumn( 4, m_pDelegateXYZ );

    ui->view->setItemDelegateForColumn( 5, m_pDelegatePw );
    ui->view->setItemDelegateForColumn( 6, m_pDelegateH );

    ui->view->setItemDelegateForColumn( 7, m_pDelegateV );
    ui->view->setItemDelegateForColumn( 8, m_pCheckDelegate );

    ui->view->setItemDelegateForColumn( 9, m_pODelegate );
    ui->view->setItemDelegateForColumn( 10, m_pDelegateTime );

    m_pContextMenu = NULL;

    m_pActionToHere = NULL;
    m_pActionAddBefore = NULL;
    m_pActionAddBelow = NULL;
    m_pActionDelete = NULL;

    m_pActionUp = NULL;
    m_pActionDown = NULL;

    m_pActionExpandAll = NULL;
    m_pActionCollapseAll = NULL;

    m_pActionHoming = NULL;

    m_pActionResize = NULL;

    m_pTypeDelegate = new ComboxDelegate( 0, this );

    QStringList strList;
//    strList<<"PA"<<"PRA"<<"PRN"<<"PRLA";
    strList<<"PA"<<"PRA"<<"PRN";
    m_pTypeDelegate->setItems( strList );

    ui->view->setItemDelegateForColumn( 1, m_pTypeDelegate );

    buildConnection();

    spySetting( MRX_T4::e_add_record );
    spySetting( MRX_T4::e_edit_record );
}

ActionTable::~ActionTable()
{
    delete ui;
}

void ActionTable::keyReleaseEvent(QKeyEvent *event)
{
    if ( event->key()==Qt::Key_Insert )
    {
        if ( ui->toolInsert->isEnabled() )
        { on_toolInsert_clicked(); }
        else
        {}
    }
    else if ( event->key()==Qt::Key_Delete )
    {
        if ( ui->toolDel->isEnabled() )
        { on_toolDel_clicked(); }
        else
        {}
    }
    else
    { XPage::keyReleaseEvent( event ); }
}

bool ActionTable::event(QEvent *event)
{
    if ( event->type() == e_event_post_load )
    {
        doLoad();

        return true;
    }

    return XPage::event( event );
}

void ActionTable::setOperAble( bool b )
{
    //! operate
    if ( b )
    { exitMission(); }
    else
    { enterMission(); }
}

void ActionTable::setOpened( bool b )
{
    //! operate
    if ( b )
    { exitMission(); }
    else
    { enterMission(); }
}

void ActionTable::retranslateUi()
{
    ui->retranslateUi( this );
}

void ActionTable::buildConnection()
{
    //! connection
    connect( ui->view, SIGNAL(customContextMenuRequested(const QPoint &)),
             this, SLOT(slot_customContextMenuRequested(const QPoint &)));

    connect( ui->view, SIGNAL(activated(const QModelIndex &)),
             this, SLOT(on_view_clicked(const QModelIndex &)));

    connect( ui->view, SIGNAL(doubleClicked(const QModelIndex &)),
             this, SLOT(on_view_clicked(const QModelIndex &)));

    connect( ui->view, SIGNAL(entered(const QModelIndex &)),
             this, SLOT(on_view_clicked(const QModelIndex &)));

    connect( ui->view, SIGNAL(pressed(const QModelIndex &)),
             this, SLOT(on_view_clicked(const QModelIndex &)));

    //! post timer
    connect( &mPostSaveTimer, SIGNAL(timeout()),
             this, SLOT(slot_post_save_timeout()) );
    mPostSaveTimer.setSingleShot( true );
    mPostSaveTimer.setInterval( 1000 );
}

void ActionTable::setModel( QAbstractItemModel *pModel )
{
    Q_ASSERT( NULL != pModel );
    Q_ASSERT( NULL != m_pPlugin);
    MRX_T4 *pRobo = (MRX_T4*)m_pPlugin;

    ui->view->setModel( pModel );

    TreeModel *pMegaModel = (TreeModel*)pModel;
    pMegaModel->setTerminalSpeed( pRobo->mMaxTerminalSpeed );
    pMegaModel->setJointSpeeds( pRobo->mMaxJointSpeeds );

    if ( NULL != pMegaModel )
    {
        connect( pMegaModel, SIGNAL(signal_data_changed()),
                 this, SLOT(slot_request_save()) );

        connect( pMegaModel, SIGNAL(signal_data_changed()),
                 this, SLOT(slot_data_changed()) );
    }

    slot_resize();

    bool hasCurrent = ui->view->selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        ui->view->closePersistentEditor( ui->view->selectionModel()->currentIndex());
    }
}

void ActionTable::onSetting(XSetting setting)
{
    XPage::onSetting( setting );

    if ( (int)setting.mSetting == (int)MRX_T4::e_add_record )
    {
        addRecord( setting );
    }
    else if ( (int)setting.mSetting == (int)MRX_T4::e_edit_record )
    {
        editRecord( setting );
    }
    else
    {}
}

void ActionTable::rst()
{
    MRX_T4 *pRobo = (MRX_T4*)m_pPlugin;
    if ( NULL == pRobo )
    { return; }

    pRobo->rstRecordTable();
}

int ActionTable::upload()
{
    check_connect_ret( -1 );

    QByteArray ary;

    int capacity = 1024*1024;
    ary.reserve( capacity );

    do
    {
        //! query
        QByteArray fileListAry;
        int fileListCapacity = 1024*1024;
        fileListAry.reserve( fileListCapacity );
        int fileListLen;
        fileListLen = mrgStorageMotionFileQuery( pRobo->deviceVi(),
                                   0,
                                   fileListAry.data(),
                                   fileListCapacity );
        if ( fileListLen < fileListCapacity )
        {}
        else
        {
            sysError( tr("Too many files") );
            return -1;
        }

        //! file list
        QString fileList( QByteArray( fileListAry, fileListLen ) );

        //! check the file exist
        QString fileAry = QString(record_file_name);
        if ( fileList.contains( fileAry.toLatin1().data() ) )
        {}
        else
        {
            sysError( tr("File do not exist") + " " + fileAry );
            return -1;
        }

        //! read data
        int ret = mrgStorageMotionFileContextRead( pRobo->deviceVi(),
                                               fileAry.toLatin1().data(),
                                               ary.data());
        if ( ret <= 0 )
        {
            sysError( tr("File read fail") + " " + fileAry );
            break;
        }
        ary.resize( ret );

        ret = mrgStorageWriteFile( plugin_root_dir(),
                             (record_file_name),
                             (quint8*)ary.data(),
                             ary.length() );

        //! read success + reload
        postDoLoad();

        return 0;

    }while( 0 );

    sysError( tr("Record load fail") );
    return -1;
}
int ActionTable::download()
{
    check_connect_ret( -1 );

    QByteArray ba;
    QTextStream stream( &ba );

    TreeModel *model = (TreeModel *)ui->view->model();
    int ret = model->exportOut( stream );
    stream.flush();
    if ( ret != 0 )
    { return ret; }

    QString fileOutName = QString( record_file_name );

    ret = mrgStorageMotionFileSaveContext(pRobo->deviceVi(),
                                          ba.data(),
                                          ba.size(),
                                          fileOutName.toLatin1().data());
    if ( ret != 0 )
    {
        sysError( tr("Record save fail") );
    }

    return ret;
}
int ActionTable::diff()
{
    return 0;
}

int ActionTable::requestLoad( const QString &path, const QString &name )
{
    QString fileName = path + "/" + name;

    int ret = -1;
    do
    {
        TreeModel *pTable = (TreeModel*)ui->view->model();
        if ( NULL == pTable )
        {
            ret = -1;
            break;
        }

        //! read
        QByteArray theAry;
        theAry.reserve( max_file_size );
        ret = mrgStorageReadFile( m_pPlugin->deviceVi(),
                                  0,
                                  path.toLatin1().data(),
                                  name.toLatin1().data(),
                                  (quint8*)theAry.data() );
        if ( ret <= 0 )
        {
            ret = -1;
            break;
        }

        theAry.resize( ret );

        QTextStream stream( theAry );

        //! load
        ret = pTable->loadIn( stream );
        if ( ret != 0 )
        { break; }

    }while( 0 );

    if ( ret != 0 )
    {
        sysError( fileName + " " + tr("load fail") );
    }

    return ret;
}

void ActionTable::enterMission()
{
    if ( NULL != m_pContextMenu )
    { m_pContextMenu->setEnabled( false ); }

    ui->view->setEditTriggers( QAbstractItemView::NoEditTriggers );

    ui->groupBox->setEnabled( false );
}
void ActionTable::exitMission()
{
    if ( NULL != m_pContextMenu )
    { m_pContextMenu->setEnabled( true ); }

    ui->view->setEditTriggers( mRawEditTriggers );

    ui->groupBox->setEnabled( true );
}

//! \note return the parent row
int ActionTable::currentIndex()
{
    if ( ui->view->currentIndex().isValid() )
    {
        TreeItem* pItem = (TreeItem*)ui->view->currentIndex().internalPointer();
        if ( pItem == NULL )
        { return -1; }

        if ( pItem->level() == 0 )
        {
            return -1;
        }
        else if ( pItem->level() == 1 )
        {
            return ui->view->currentIndex().row();
        }
        else if ( pItem->level() == 2 )
        {
            //! \todo notify
            if ( ui->view->currentIndex().parent().isValid() )
            { return ui->view->currentIndex().parent().row(); }
            else
            { return -1; }
        }
        else
        { return -1; }
    }
    else
    { return -1; }
}

void ActionTable::addRecord( XSetting setting )
{
    //! add below
    slot_add_below();

    //! edit the data
    editRecord( setting );
}

//! keep the v and acc
void ActionTable::editRecord( XSetting setting )
{
    QList<QVariant> coords;
    QList<double> vals;
    coords = setting.mPara1.toList();

    bool bOk;
    foreach (QVariant var, coords)
    {
        vals<<var.toDouble( &bOk );
        if ( bOk )
        {}
        else
        { return; }
    }

    QAbstractItemModel *pModel;
    pModel = ui->view->model();
    if ( NULL == pModel )
    { return; }

    //! current row
    int cRow;
    QModelIndex index = ui->view->currentIndex();
    if ( index.isValid() )
    { cRow = index.row(); }
    else
    { return; }

    //! from index
    QModelIndex modelIndex;

    modelIndex = pModel->index( cRow, 1, index.parent() );
    pModel->setData( modelIndex, "PA" );

    //! x/y/z/pw/h
    for ( int i = 0; i < 5; i++ )
    {
        modelIndex = pModel->index( cRow, i + 2, index.parent() );
        pModel->setData( modelIndex, vals.at( i ) );
    }

    //! \note keep vel,acc

}

void ActionTable::doSave()
{
    //! delay save
    mPostSaveTimer.start();
}

void ActionTable::doLoad()
{
    int ret = -1;

    ret = _doLoad( record_file_name );

    if ( ret != 0 )
    {
        sysError( QString(record_file_name) + QString(" ") + tr("load fail") );
    }
}

void ActionTable::postDoLoad()
{
    QEvent *pEvent = new QEvent( (QEvent::Type)e_event_post_load );
    if ( NULL != pEvent )
    {
        qApp->postEvent( this, pEvent );
    }
}

int ActionTable::_doSave( const QString &name )
{
    QString fileName = m_pPlugin->selfPath() + "/" + name;

    int ret = -1;

    do
    {
        TreeModel *pTable = (TreeModel*)ui->view->model();
        if ( NULL == pTable )
        {
            ret = -1;
            break;
        }

        //! export
        QByteArray theAry;
        QTextStream stream( &theAry );
        ret = pTable->exportOut( stream );
        if ( ret != 0 )
        { break; }
        stream.flush();

        //! write
        ret = mrgStorageWriteFile( plugin_root_dir(),
                                   name.toLatin1().data(),
                                   (quint8*)theAry.data(),
                                   theAry.length() );
        if ( ret != 0 )
        { break; }

    }while( 0 );

    return ret;
}
int ActionTable::_doLoad( const QString &name )
{
    QString fileName = m_pPlugin->selfPath() + "/" + name;

    int ret = -1;
    do
    {
        TreeModel *pTable = (TreeModel*)ui->view->model();
        if ( NULL == pTable )
        {
            ret = -1;
            break;
        }

        //! read
        QByteArray theAry;
        theAry.reserve( max_file_size );
        ret = mrgStorageReadFile( plugin_root_dir(),
                                  name.toLatin1().data(),
                                  (quint8*)theAry.data() );
        if ( ret <= 0 )
        {
            ret = -1;
            break;
        }

        theAry.resize( ret );

        QTextStream stream( theAry );

        //! load
        ret = pTable->loadIn( stream );
        if ( ret != 0 )
        { break; }

    }while( 0 );

    return ret;
}

int ActionTable::postSaveProc(void *pContext)
{
    if ( NULL == pContext )
    { return -1; }

    int ret = -1;

    ret = _doSave( mPostSaveFileName );

    if ( ret != 0 )
    { sysError( QString(mPostSaveFileName) + QString(" ") + tr("save fail") ); }

    return ret;
}

//! change the status by the current item
void ActionTable::updateControl()
{
    if ( ui->groupBox->isEnabled() )
    {}
    else
    { return; }

    QModelIndex index = ui->view->currentIndex();
    if ( index.isValid() )
    {}
    else
    {
        ui->toolUp->setEnabled( false );
        ui->toolDown->setEnabled( false );
        return;
    }

    TreeItem *pItem = static_cast<TreeItem*>( index.internalPointer() );
    if ( NULL == pItem )
    { return; }

    TreeItem *pParent = pItem->parent();
    if ( NULL == pParent )
    { return; }

    //! menu
    if ( m_pContextMenu != NULL )
    {
        if ( pItem->level() == 1 )
        {
            m_pActionAddBefore->setEnabled( false );
            m_pActionAddBelow->setEnabled( true );
            m_pActionDelete->setEnabled( true );
        }
        else if ( pItem->level() == 2 )
        {
            m_pActionAddBefore->setEnabled( true );
            m_pActionAddBelow->setEnabled( true );
            m_pActionDelete->setEnabled( true );
        }
        else
        { return; }

        //! rst
        m_pActionUp->setEnabled( true );
        m_pActionDown->setEnabled( true );
        if ( index.row() < 1 )
        { m_pActionUp->setEnabled( false ); }

        if ( index.row() >= pParent->childCount() - 1 )
        { m_pActionDown->setEnabled( false ); }

        //! change the to here
        QString strToHereSuffix;
        strToHereSuffix = QString("(%1,%2,%3)").arg( pItem->data(2).toDouble(),0,'f',2 )
                                               .arg( pItem->data(3).toDouble(),0,'f',2 )
                                               .arg( pItem->data(4).toDouble(),0,'f',2 );

        //! PA && RA
        if ( str_is( pItem->data( 1 ).toString(), "PA") )
        { m_pActionToHere->setText( tr("To:") + " " + strToHereSuffix ); }
        else
        { m_pActionToHere->setText( tr("Step:") + " " + strToHereSuffix );}

        m_pActionToHere->setVisible( pItem->isValid() );
    }

    //! tool button
    {
        if ( pItem->level() == 1 )
        { ui->toolDel->setEnabled( true ); }
        else if ( pItem->level() == 2 )
        { ui->toolDel->setEnabled( true ); }
        else
        {}

        //! rst
        ui->toolUp->setEnabled( true );
        ui->toolDown->setEnabled( true );
        if ( index.row() < 1 )
        { ui->toolUp->setEnabled( false ); }

        if ( index.row() >= pParent->childCount() - 1 )
        { ui->toolDown->setEnabled( false ); }
    }
}

//! x,y,z,pw,h,v,a
int ActionTable::onToHere( QVariant var )
{
    check_connect_ret( -1 );

    int ret;
    QList<QVariant> vars;

    vars = var.toList();

    if ( str_is( vars.at(0).toString(), "PA" ) )
    {
        ret = absToHere( vars );
    }
    else if ( str_is( vars.at(0).toString(), "PRA" )
              || str_is( vars.at(0).toString(), "PRN" ))
    {
        ret = relToHere( vars );
    }
    //! \todo the last abs position
    else if ( str_is( vars.at(0).toString(), "PRLA" ) )
    {
        ret = rellabsToHere( vars );
    }
    else
    {
        sysError( tr("Invalid input") );
        return -1;
    }

    if ( ret != 0 )
    { return ret; }

    //! terminal
    ret = terminalAction( vars );

    return ret;

}
int ActionTable::onHoming( QVariant var )
{
    check_connect_ret( -1 );

    int ret;

    ret = mrgRobotGoHome( robot_var(),
                          pRobo->mHomeTimeout*1000 );

    return ret;
}

int ActionTable::terminalAction( QList<QVariant> &vars )
{
    check_connect_ret( -1 );

    int ret;

    //! Wrist
    float speed = pRobo->mMaxJointSpeeds.at(3) * vars.at(6).toDouble() / 100.0;

    ret = mrgSetRobotWristPose(robot_var(), 0, vars.at(4).toDouble(), speed, guess_dist_time_ms( 180/speed, 180 ));
    if( ret != 0 )
        return ret;

    //! \note the terminal is rel
    speed = pRobo->mMaxJointSpeeds.at(4) * vars.at(6).toDouble() / 100.0;
    double dis = vars.at(5).toDouble();
    if( qAbs( dis )>FLT_EPSILON ){
        speed = pRobo->mMaxJointSpeeds.at(4) * vars.at(6).toDouble() / 100.0;
        ret = mrgRobotToolExe(robot_var(),
                              dis,
                              qAbs(dis)/speed,
                              guess_dist_time_ms( qAbs( dis )/speed, qAbs(dis))
                              );
        if(ret!=0){return ret;}
    }

    return ret;
}

int ActionTable::relToHere( QList<QVariant> &vars )
{
    check_connect_ret( -1 );

    int ret;
    //! \note the speed
    ret = pRobo->relMove( "",
                          vars.at(1).toDouble(), vars.at(2).toDouble(), vars.at(3).toDouble(),
                          vars.at(4).toDouble(), vars.at(5).toDouble(),
                          rel_to_abs_speed( vars.at(6).toDouble() ), vars.at(7).toBool()
                          );
    if( ret != 0 )
        return ret;

    //! \todo the terminal is rel

    return ret;
}
int ActionTable::absToHere( QList<QVariant> &vars )
{
    check_connect_ret( -1 );

    int ret;
    ret = pRobo->absMove( "",
                          vars.at(1).toDouble(), vars.at(2).toDouble(), vars.at(3).toDouble(),
                          vars.at(4).toDouble(), vars.at(5).toDouble(),
                          rel_to_abs_speed( vars.at(6).toDouble() ), vars.at(7).toBool()
                          );
    if( ret != 0 )
        return ret;

    //! marker
    pRobo->setAbsMarker( vars.at(1).toDouble(), vars.at(2).toDouble(), vars.at(3).toDouble(),
                         vars.at(4).toDouble(), vars.at(5).toDouble(),
                         rel_to_abs_speed( vars.at(6).toDouble() ), vars.at(7).toBool() );

    return ret;
}

int ActionTable::rellabsToHere( QList<QVariant> &vars )
{
    logDbg();
    check_connect_ret( -1 );

    //! check the abs marker
    if ( pRobo->absMarker()->bValid )
    {}
    else
    {
        sysError( tr("Invalid absolute position") );
        return -1;
    }

    int ret;
    ret = pRobo->absMove( "",
                          pRobo->absMarker()->x + vars.at(1).toDouble(),
                          pRobo->absMarker()->y + vars.at(2).toDouble(),
                          pRobo->absMarker()->z + vars.at(3).toDouble(),

                          vars.at(4).toDouble(), vars.at(5).toDouble(),
                          rel_to_abs_speed( vars.at(6).toDouble() ), vars.at(7).toBool()
                          );
    return  ret;
}

void ActionTable::slot_request_save()
{
    doSave();
}

void ActionTable::slot_request_load()
{ postDoLoad(); }

void ActionTable::slot_data_changed()
{
    //! add to the record
    Q_ASSERT( NULL != m_pPlugin );

    QVariant var;

    var.setValue( ui->view->model()->rowCount() );

    m_pPlugin->emit_setting_changed( (eXSetting)(MRX_T4::e_setting_record), var );

    updateControl();
}

#define get_data( val, col )    val = pItem->dataSets().at( col );\
vars<<val;
void ActionTable::slot_toHere()
{
    QModelIndex index = ui->view->currentIndex();
    if ( index.isValid() )
    {}
    else
    { return; }

    QVariant type, x, y, z, pw, h, v, line;
    QList<QVariant> vars;

    TreeItem *pItem = (TreeItem*)index.internalPointer();
    if ( NULL == pItem )
    { return; }

//    QVector<QVariant> varDataset = pItem->dataSets();

    get_data( type, 2 );
    get_data( x, 3 );
    get_data( y, 4 );
    get_data( z, 5 );
    get_data( pw, 6 );

    get_data( h, 7 );
    get_data( v, 8 );
    get_data( line, 9 );

    QVariant var( vars );

    Q_ASSERT( NULL != m_pPlugin );
    //! action
    on_post_setting( ActionTable, onToHere, tr("To here") );
}

void ActionTable::slot_add_before()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();

    //! get data
    QVector<QVariant> vars;
    QModelIndex iterIndex;
    for ( int i = 0; i < ui->view->model()->columnCount(); i++ )
    {
        iterIndex = ui->view->model()->index( index.row(), i, index.parent() );
        vars<<ui->view->model()->data( iterIndex, raw_data_role );
    }

    if ( ui->view->model()->insertRow( index.row(), index.parent() ) )
    {}
    else
    { return; }

    //! add data
    for ( int i = 0; i < ui->view->model()->columnCount(); i++ )
    {
        iterIndex = ui->view->model()->index( index.row(), i, index.parent() );
        ui->view->model()->setData( iterIndex, vars.at(i) );
    }

    //! current
    ui->view->selectionModel()->setCurrentIndex( ui->view->model()->index( index.row(), 0, index.parent() ),
                               QItemSelectionModel::ClearAndSelect );

    return;
}
void ActionTable::slot_add_below()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();

    //! parent?
    TreeItem *pItem = static_cast<TreeItem*>( index.internalPointer() );
    if ( NULL == pItem )
    { return; }

    //! get data
    QVector<QVariant> vars;
    QModelIndex iterIndex;
    for ( int i = 0; i < ui->view->model()->columnCount(); i++ )
    {
        iterIndex = ui->view->model()->index( index.row(), i, index.parent() );
        vars<<ui->view->model()->data( iterIndex, raw_data_role );
    }

    //! section
    int dstRow;
    QModelIndex parIndex;
    if ( pItem->level() == 1 )
    {
        //! parIndex
        parIndex = ui->view->model()->index( index.row(), 0, index.parent() );

        //! insert child
        if ( ui->view->model()->insertRow( 0, parIndex ) )
        { }
        else
        { return; }

        dstRow = 0;
    }
    //! sibling
    else
    {
        if ( ui->view->model()->insertRow( index.row()+1, index.parent() ) )
        {}
        else
        { return; }

        parIndex = index.parent();
        dstRow = index.row() + 1;
    }

    ui->view->selectionModel()->setCurrentIndex( ui->view->model()->index( dstRow, 0, parIndex ),
                               QItemSelectionModel::ClearAndSelect );

    //! add data
    bool bRet;
    for ( int i = 0; i < ui->view->model()->columnCount(); i++ )
    {
        iterIndex = ui->view->model()->index( dstRow, i, parIndex );

        bRet = ui->view->model()->setData( iterIndex, vars.at(i) );
        if ( !bRet )
        { logDbg()<<iterIndex<<parIndex<<vars.at(i); return; }
    }

    return;
}

void ActionTable::slot_delete()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    if ( ui->view->model()->removeRow( index.row(), index.parent() ) )
    {}
    else
    { return; }

    return;
}

void ActionTable::slot_up()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();

    ui->view->model()->moveRow( index.parent(),
                                index.row(),
                                index.parent(),
                                index.row() - 1 );

    ui->view->setCurrentIndex( ui->view->model()->index( index.row()-1, 0, index.parent() ) );

    updateControl();
}
void ActionTable::slot_down()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();

    ui->view->model()->moveRow( index.parent(),
                                index.row(),
                                index.parent(),
                                index.row() + 1 );

    ui->view->setCurrentIndex( ui->view->model()->index( index.row()+1, 0, index.parent() ) );

    updateControl();
}

void ActionTable::slot_expandAll()
{
    ui->view->expandAll();
}
void ActionTable::slot_collapseAll()
{
    ui->view->collapseAll();
}

void ActionTable::slot_homing()
{
    QVariant var;

    m_pPlugin->attachMissionWorking( this, (XPage::onMsg)(&ActionTable::onHoming), var );
}
void ActionTable::slot_resize()
{
}

void ActionTable::slot_post_save_timeout()
{
    //! \note attach to working
    mPostSaveFileName = record_file_name;
    attachUpdateWorking( (XPage::procDo)( &ActionTable::postSaveProc ),
                         WorkingApi::e_work_single,
                         tr("save mrp"),
                         &mPostSaveFileName,
                         0 );

}

void ActionTable::slot_customContextMenuRequested(const QPoint &pos)
{
    if ( mbMissionWorking )
    { return; }

    //! plugin on line
    if ( m_pPlugin->isOnLine() )
    {}
    else
    { return; }

    //! valid
    if ( ui->view->currentIndex().isValid() )
    {
        if(m_pContextMenu == NULL )
        {
            m_pContextMenu = new QMenu(ui->view);
            if ( NULL == m_pContextMenu )
            { return; }

            //! create
            m_pActionToHere = m_pContextMenu->addAction(tr("To Here"));
            if ( NULL == m_pActionToHere )
            { return; }
            m_pActionToHere->setIcon( QIcon(":/res/image/icon/skip.png") );

            m_pContextMenu->addSeparator();
            m_pActionAddBefore = m_pContextMenu->addAction( tr("Add Before") );
            m_pActionAddBelow = m_pContextMenu->addAction( tr("Add Below") );
            m_pActionDelete = m_pContextMenu->addAction( tr("Delete") );

            m_pActionAddBefore->setIcon( QIcon(":/res/image/icon/xiangshangzhanhang.png") );
            m_pActionAddBelow->setIcon( QIcon(":/res/image/icon/xiangxiazhanhang.png") );
            m_pActionDelete->setIcon( QIcon(":/res/image/icon/delete.png") );

            m_pContextMenu->addSeparator();
            m_pActionUp = m_pContextMenu->addAction( tr("Up") );
            m_pActionDown = m_pContextMenu->addAction( tr("Down") );

            m_pActionUp->setIcon( QIcon(":/res/image/icon/arrowup.png") );
            m_pActionDown->setIcon( QIcon(":/res/image/icon/arrowdown.png") );

            m_pContextMenu->addSeparator();
            m_pActionExpandAll = m_pContextMenu->addAction( tr("All Expand") );
            m_pActionCollapseAll = m_pContextMenu->addAction( tr("All Collapse") );

            m_pContextMenu->addSeparator();
            m_pActionHoming = m_pContextMenu->addAction( tr("Homing") );

            //! connect
            connect( m_pActionToHere, SIGNAL(triggered(bool)),
                     this, SLOT(slot_toHere()) );

            connect(m_pActionAddBefore, SIGNAL(triggered(bool)),
                    this, SLOT( slot_add_before( ) ) );
            connect(m_pActionAddBelow, SIGNAL(triggered(bool)),
                    this, SLOT( slot_add_below( ) ) );
            connect(m_pActionDelete, SIGNAL(triggered(bool)),
                    this, SLOT( slot_delete( ) ) );

            connect(m_pActionUp, SIGNAL(triggered(bool)),
                    this, SLOT( slot_up( ) ) );
            connect(m_pActionDown, SIGNAL(triggered(bool)),
                    this, SLOT( slot_down( ) ) );

            connect(m_pActionExpandAll, SIGNAL(triggered(bool)),
                    this, SLOT( slot_expandAll() ) );
            connect(m_pActionCollapseAll, SIGNAL(triggered(bool)),
                    this, SLOT( slot_collapseAll() ) );

            connect(m_pActionHoming, SIGNAL(triggered(bool)),
                    this, SLOT( slot_homing() ) );

        }
        else
        {}

        updateControl();

        m_pContextMenu->exec(QCursor::pos());
    }
}

void ActionTable::on_toolExport_clicked()
{
    QString str = QFileDialog::getSaveFileName( this, tr("Save"), m_pPlugin->homePath(), tr( "Record (*.mrp)") );
    if ( str.isEmpty() )
    { return; }

    //! do save
    TreeModel *pTable = (TreeModel*)ui->view->model();
    if ( NULL == pTable )
    {
        sysError( tr("Save record fail") );
        return;
    }

    int ret = pTable->exportOut( str );
    if ( ret != 0 )
    {
        sysError( tr("Save record fail") );
    }
}

void ActionTable::on_toolImport_clicked()
{
    QString str = QFileDialog::getOpenFileName( this, tr("Open"), m_pPlugin->homePath(), tr( "Record (*.mrp)") );
    if ( str.isEmpty() )
    { return; }logDbg()<<str;

    //! do load
    TreeModel *pTable = (TreeModel*)ui->view->model();
    if ( NULL == pTable )
    {
        sysError( tr("Load record fail") );
        return;
    }

    int ret = pTable->loadIn( str );
    if ( ret != 0 )
    { sysError( tr("Load record fail") ); }
}

void ActionTable::on_toolOpen_clicked()
{
    FileManager manager;
    manager.setMode(OPENACTIONTABLE);
    manager.attachPlugin(m_pPlugin);
    manager.setPath(m_pPlugin->selfPath()+"/");
    if(manager.exec()== QDialog::Accepted ){

    }else{ return; }

    QString strResult = manager.strResult();
    strResult = strResult.simplified();
    if ( strResult.isEmpty() )
    {
        sysPrompt( tr("Invalid File"), 0 );
        return;
    }

    int ret = _doLoad(strResult);
    if ( ret != 0 )
    { sysError( tr("Load record fail") ); }

}

//! \todo list the files
void ActionTable::on_toolSaveAs_clicked()
{
    FileManager manager;
    manager.setMode(SAVEAS);
    manager.attachPlugin(m_pPlugin);
    manager.setPath(m_pPlugin->selfPath()+"/");
    if( manager.exec() == QDialog::Accepted ){

    }else{ return; }

    QString strResult = manager.strResult();
    strResult = strResult.simplified();
    if ( strResult.isEmpty() )
    {
        sysPrompt( tr("Invalid description"), 0 );
        return;
    }

    //! clear
    if ( strResult.endsWith(".mrp", Qt::CaseInsensitive) )
    {
        strResult.remove( strResult.length() - 4, 4 );
    }
    else
    {}
    strResult.append( ".mrp" );

    //! \note attach to working
    mPostSaveFileName = strResult;
    attachUpdateWorking( (XPage::procDo)( &ActionTable::postSaveProc ),
                         WorkingApi::e_work_single,
                         tr("save mrp"),
                         &mPostSaveFileName,
                         0 );
}

void ActionTable::on_toolUp_clicked()
{
    slot_up();
}

void ActionTable::on_toolDown_clicked()
{
    slot_down();
}

void ActionTable::on_toolDel_clicked()
{
    slot_delete();
}

void ActionTable::on_toolClr_clicked()
{
    //! remove all
    //! keep the node
    ui->view->model()->removeRows( 0, ui->view->model()->rowCount() );
}

void ActionTable::on_toolInsert_clicked()
{
    slot_add_below();
}

void ActionTable::on_view_clicked(const QModelIndex &index)
{
    updateControl();
}

}





