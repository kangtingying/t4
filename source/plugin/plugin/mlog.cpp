#include "mlog.h"

MLog::MLog( QByteArray *array, QIODevice::OpenMode openMode ) : QTextStream( array, openMode )
{
}

