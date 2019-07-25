#include "checkdelegate.h"
#include <QtWidgets>

#define CHECK_WIDGET    QCheckBox

static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)
{
    QStyleOptionButton checkBoxStyleOption;
    QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    QPoint checkBoxPoint(viewItemStyleOptions.rect.left() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2-4,
                         viewItemStyleOptions.rect.top() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2+5);

    //返回几何形状
    return QRect(checkBoxPoint, checkBoxRect.size());
}

CheckDelegate::CheckDelegate( checkShape shp,
                              QObject *parent ) : QStyledItemDelegate(parent)
{
    mAlign = Qt::AlignCenter;

    mShape = shp;

    mPrimSize.setWidth( 0 );
    mPrimSize.setHeight( 0 );

    if ( mShape == shape_check )
    {
        QCheckBox chk;
        mPrimSize = chk.sizeHint();
    }
    else if ( mShape == shape_radio )
    {
        QRadioButton rad;
        mPrimSize = rad.sizeHint();
    }
    else
    {}

    mVisibleRole = Qt::DisplayRole;
}

QWidget *CheckDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    if ( mShape == shape_check )
    {
        CHECK_WIDGET *pCheckbox = new CHECK_WIDGET( parent );

        pCheckbox->setCheckable( true );

        return pCheckbox;
    }
    else if ( mShape == shape_radio )
    {
        QRadioButton *pRadio = new QRadioButton( parent );

        return pRadio;
    }
    else
    {
        Q_ASSERT( false );
        return NULL;
    }
}

void CheckDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool b = index.model()->data(index, Qt::EditRole).toBool();

    CHECK_WIDGET *pCheckBox = static_cast<CHECK_WIDGET*>(editor);

    pCheckBox->setChecked( b );
}
void CheckDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    CHECK_WIDGET *pCheckBox = static_cast<CHECK_WIDGET*>(editor);

    model->setData( index, pCheckBox->isChecked(), Qt::EditRole );
}

void CheckDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( mAlign == Qt::AlignCenter )
    {
        QRect localRect;

        localRect = option.rect;
        localRect.setX( option.rect.left() + ( option.rect.width() - mPrimSize.width() ) / 2 );

        editor->setGeometry( localRect );
    }
    else
    {
        editor->setGeometry( option.rect );
    }
}

void CheckDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    //! item visible
    bool bVisible = true;

    QVariant var;
    if ( mVisibleRole != Qt::DisplayRole )
    {
        var = index.model()->data( index, mVisibleRole );
        if ( var.isValid() )
        { bVisible = var.toBool(); }
        else
        {}
    }

    //! item checked
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
    if ( bVisible )
    {
        QStyleOptionButton checkBoxStyleOption;
        checkBoxStyleOption.state |= QStyle::State_Enabled;
        checkBoxStyleOption.state |= checked? QStyle::State_On : QStyle::State_Off;
        checkBoxStyleOption.rect = CheckBoxRect(option);

        QStyle::ControlElement tCe;
        if ( mShape == shape_check )
        { tCe = QStyle::CE_CheckBox; }
        else
        { tCe = QStyle::CE_RadioButton; }

        QApplication::style()->drawControl( tCe,&checkBoxStyleOption,painter);
    }
    else
    {
        //! invisible do nothing
    }
}

bool CheckDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() == 8){
        if((event->type() == QEvent::MouseButtonRelease) ||
            (event->type() == QEvent::MouseButtonDblClick)){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() != Qt::LeftButton ||
                    !CheckBoxRect(option).contains(mouseEvent->pos())){
                return true;
            }
            if(event->type() == QEvent::MouseButtonDblClick){
               return true;
            }
        }else if(event->type() == QEvent::KeyPress){
            if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select){
                return false;
            }
        }else{
            return false;
        }
        bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
        return model->setData(index, !checked, Qt::EditRole);
    }else{
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}

void CheckDelegate::setShape( checkShape shp )
{ mShape = shp; }
checkShape CheckDelegate::shape()
{ return mShape; }

void CheckDelegate::setAlignment(  Qt::Alignment align )
{ mAlign = align; }
Qt::Alignment CheckDelegate::alignment()
{ return mAlign; }

void CheckDelegate::setVisibleRole( Qt::ItemDataRole tRole )
{ mVisibleRole = tRole;  }
Qt::ItemDataRole CheckDelegate::visibleRole()
{ return mVisibleRole; }
