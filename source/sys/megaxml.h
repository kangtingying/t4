#ifndef MEGAXML_H
#define MEGAXML_H

#include <QString>

class MegaXML
{
public:
    MegaXML();

    int xmlCreate(QString fileName, bool isClear = false);
    int xmlNodeAppend(QString fileName, QString nodeName, QMap<QString, QString> mapItems);
    int xmlNodeRemove(QString fileName, QString nodeName);

    QMap<QString, QString> xmlRead(QString fileName);

    QMap<QString, QString> xmlNodeRead(QString fileName);

};

#endif // MEGAXML_H
