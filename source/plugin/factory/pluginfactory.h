#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H

#include <QtCore>

#include "../plugin/xplugin.h"

class PluginFactory
{
public:
    static XPlugin *createPlugin( const QString &name,
                                  const QString &addr );
public:
    PluginFactory();
};

#endif // PLUGINFACTORY_H
