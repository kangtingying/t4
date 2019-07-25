#include "coordinate.h"
#include "ui_coordinate.h"

#include "t4.h"

namespace mrx_t4 {

Coordinate::Coordinate(QWidget *parent) :
    XPage(parent),
    ui(new Ui::Coordinate)
{
    ui->setupUi(this);

    setContextHelp( "coordinate" );

    spyEdited();

    set_page_rstable();
}

Coordinate::~Coordinate()
{
    delete ui;
}

void Coordinate::updateUi()
{
    ui->coordTool->setPara( selfPara->mCoordPara[0].mParas );
    ui->coordObj->setPara( selfPara->mCoordPara[1].mParas );
//    ui->coordUser->setPara( selfPara->mCoordPara[2].mParas );

    on_cmbUserCoord_currentIndexChanged( ui->cmbUserCoord->currentIndex() );
}

#define copy_paras( index )     for ( int i = 0; i < sizeof_array(paras); i++ )\
                                {\
                                    selfPara->mCoordPara[ index ].mParas[ i ] = paras[i];\
                                }
void Coordinate::updateData()
{
    double paras[6];

    ui->coordTool->getPara( paras );
    copy_paras( 0 );

    ui->coordObj->getPara( paras );
    copy_paras( 1 );

    //! \note get the current user coord
    ui->coordUser->getPara( paras );
    copy_paras( 2 + ui->cmbUserCoord->currentIndex() );
}

int Coordinate::upload()
{
    //! \todo
    return 0;
}
int Coordinate::download()
{
    //! \todo
    return 0;
}
int Coordinate::diff()
{
    //! \todo
    return 0;
}

void Coordinate::spyEdited()
{
    //! \todo
}

void Coordinate::retranslateUi()
{
    ui->retranslateUi( this );
}

void Coordinate::on_cmbUserCoord_currentIndexChanged(int index)
{
    //! invalid input
    if ( index < 0 || (index + 2) >= sizeof_array(selfPara->mCoordPara) )
    { return; }

    //! update para
    ui->coordUser->setPara( selfPara->mCoordPara[ 2 + index ].mParas );
}

}


