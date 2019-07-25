#ifndef MLOG_H
#define MLOG_H

#include <QTextStream>

class MLog : public QTextStream
{
public:
    MLog( QByteArray *array, QIODevice::OpenMode openMode = QIODevice::ReadOnly );
};

#endif // MLOG_H
