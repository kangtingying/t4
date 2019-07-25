#ifndef DEBUGITEM_H
#define DEBUGITEM_H

#include <QtCore>

class DebugItem
{
public:
    static int columns();
    static QString header( int col );
public:
    DebugItem();

public:
    int mId;
    double mDelay;
};

#endif // DEBUGITEM_H
