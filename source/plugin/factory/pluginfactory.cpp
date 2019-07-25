#include "pluginfactory.h"

#include "../t4/t4.h"
#include "../h2/h2.h"

#define PLUGIN_MRX_T4_NAME  "mrx-t4"
#define PLUGIN_MRX_H2_NAME  "mrx-h2"

//! factory
XPlugin *PluginFactory::createPlugin( const QString &name,
                              const QString &addr )
{
    if ( str_is( name, PLUGIN_MRX_T4_NAME) )
    { return new MRX_T4( ); }
//    else if ( str_is( name, PLUGIN_MRX_H2_NAME) )
//    { return new MRX_H2(); }
    else
    {
        return NULL;
    }
}

PluginFactory::PluginFactory()
{

}

