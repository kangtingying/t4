#include "xplugindesc.h"

XPluginDesc::XPluginDesc()
{
    mVi = -1;

}
void XPluginDesc::setModel( const QString &model )
{ mModel = model; }
QString XPluginDesc::getModel()
{ return mModel; }

void XPluginDesc::setAddr( const QString &addr )
{ mAddr = addr; }
QString XPluginDesc::getAddr()
{ return mAddr; }

int XPluginDesc::open()
{ return -1; }
void XPluginDesc::close()
{}

bool XPluginDesc::isOpened()
{ return ( mVi > 0 ); }

int XPluginDesc::deviceVi()
{ return mVi; }
