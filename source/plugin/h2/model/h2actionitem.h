#ifndef H2ACTION_ITEM_H
#define H2ACTION_ITEM_H

#include <QtCore>

class H2ActionItem
{
public:
    static int columns();
    static QString header( int col );

public:
    H2ActionItem();

public:
    QString mType;
    double mX, mY;
    double mVel,mAcc;
    QString mComment;
};

#endif // H2ACTION_H
