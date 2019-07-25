
#include <QThread>
#include <QException>
#include <QDebug>
#include "assist.h"

void localSleep( int ms )
{
    while( ms > 0 )
    {
        if ( QThread::currentThread()->isInterruptionRequested() )
        {qDebug()<<__FUNCTION__<<__LINE__;
            throw QException();
//            return;
        }

        QThread::msleep( 10 );
        ms -= 10;
    }
}
