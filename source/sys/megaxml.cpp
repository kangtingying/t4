#include "megaxml.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MegaXML::MegaXML()
{

}

//创建空的xml和根节点
int MegaXML::xmlCreate(QString fileName, bool isClear)
{
    //打开或创建文件
    QFile file(fileName); //相对路径、绝对路径、资源路径都可以
    if(file.exists() && !isClear)
    {   return 0;   }
    else
    {
        //可以用QIODevice，Truncate表示清空原来的内容
        if(!file.open(QFile::WriteOnly | QFile::Truncate))
            return -1;
    }

    QDomDocument doc;
    //写入xml头部
    QDomProcessingInstruction instruction; //添加处理命令
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //添加根节点
    QDomElement rootNode = doc.createElement("root");
    doc.appendChild(rootNode);

    //添加一个子节点及其子元素
    QString nodeName = "Header";
    QDomElement childNode = doc.createElement(nodeName);
    childNode.setAttribute("Corporation", "MegaRobot Technology Ltd."); //创建属性  其中键值对的值可以是各种类型
    rootNode.appendChild(childNode);

    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream, 4); //缩进4格
    file.close();
    return 0;
}


//增加xml节点
int MegaXML::xmlNodeAppend(QString fileName, QString nodeName, QMap<QString,QString> mapItems)
{
    xmlCreate(fileName);
    //打开文件
    QFile file(fileName); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
    {   return -1; }

    //增加一个一级子节点以及元素
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return -1;
    }
    file.close();

    QDomElement rootNode = doc.documentElement();

    //添加一个子节点及其子元素
    QDomElement childNode = doc.createElement(nodeName);
    childNode.setAttribute("id", nodeName); //创建属性  其中键值对的值可以是各种类型

    QMap<QString,QString>::iterator itMap; //遍历map
    for ( itMap=mapItems.begin(); itMap != mapItems.end(); ++itMap ) {
        QDomElement element = doc.createElement(itMap.key()); //创建子元素
        QDomText text = doc.createTextNode(itMap.value()); //设置括号标签中间的值
        element.appendChild(text);
        childNode.appendChild(element);
    }

    rootNode.appendChild(childNode);

    if(!file.open(QFile::WriteOnly | QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
    {   return -1; }

    QTextStream out_stream(&file);  //输出到文件
    doc.save(out_stream, 4);        //缩进4格
    file.close();
    return 0;
}


//删减xml内容
int MegaXML::xmlNodeRemove(QString fileName, QString nodeName)
{
    //打开文件
    QFile file(fileName); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return -1;

    //删除一个一级子节点及其元素，外层节点删除内层节点于此相同
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return -1;
    }
    file.close();  //一定要记得关掉啊，不然无法完成操作

    QDomElement rootNode = doc.documentElement();
    QDomNodeList nodeList = doc.elementsByTagName(nodeName); //由标签名定位
    for(int i = 0;i<nodeList.count();i++)
    {
        QDomElement node = nodeList.at(i).toElement();
        if(node.attribute("id") == nodeName)
        {//以属性名定位，类似于hash的方式，warning：这里仅仅删除一个节点，其实可以加个break
            rootNode.removeChild(nodeList.at(i));
            break;
        }
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {   return -1; }
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream, 4);    //缩进4格
    file.close();
    return 0;
}


//更新xml内容
/** ************************************************
void MegaXML::UpdateXml(QString fileName, QString nodeName, QString itemName, QString itemText)
{
    //打开文件
    QFile file(fileName); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项, 如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

//    QDomElement root = doc.documentElement();
//    QDomNodeList list = root.elementsByTagName(nodeName);
//    QDomNode node = list.at(list.size()-1).firstChild();    //定位到第三个一级子节点的子元素
//    QDomNode oldnode = node.firstChild();   //标签之间的内容作为节点的子节点出现, 当前是Pride and Projudice
//    node.firstChild().setNodeValue("Emma");
//    QDomNode newnode = node.firstChild();
//    node.replaceChild(newnode, oldnode);

    QDomElement root = doc.documentElement();
    QDomNodeList list = doc.elementsByTagName(nodeName); //由标签名定位
    for(int i = 0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("id") == nodeName)  //以属性名定位，类似于hash的方式，warning：这里仅仅删除一个节点，其实可以加个break
            root.removeChild(list.at(i));
    }



    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream, 4); //缩进4格
    file.close();
}
*******************************************/


//读xml
QMap<QString,QString> MegaXML::xmlRead(QString fileName)
{
    QMap<QString,QString> map;

    //打开或创建文件
    QFile file(fileName); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
    {
//        qDebug() << fileName << "open failured";
        return map;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
//        qDebug() << fileName << "setContent error";
        file.close();
        return map;
    }
    file.close();

    QDomElement root = doc.documentElement(); //返回根节点
//    qDebug() << root.nodeName();
    QDomNode node = root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e  =  node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
//            qDebug()  <<  e.tagName()  <<  e.attribute("id"); //打印键值对，tagName和nodeName是一个东西
            QDomNodeList list  =  e.childNodes();
            for(int i = 0; i<list.count();i++) //遍历子元素，count和size都可以用, 可用于标签数计数
            {
                QDomNode n = list.at(i);
                if(node.isElement())
                {
//                    qDebug()  <<  n.nodeName() << ":" << n.toElement().text();
                    if(n.nodeName() != "root")
                        map.insert(n.nodeName(),n.toElement().text());
                }
            }
        }
        node = node.nextSibling(); //下一个兄弟节点, nextSiblingElement()是下一个兄弟元素，都差不多
    }
    return map;
}


//shizhong
QMap<QString,QString> MegaXML::xmlNodeRead(QString fileName)
{
    QMap<QString,QString> map;

    //打开或创建文件
    QFile file(fileName); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
    {
//        qDebug() << fileName << "open failured";
        return map;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
//        qDebug() << fileName << "setContent error";
        file.close();
        return map;
    }
    file.close();

    QDomElement root = doc.documentElement(); //返回根节点
//    qDebug() << root.nodeName();

    QDomNode node = root.firstChild(); //获得第一个子节点

#if 0    
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e  =  node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
//            qDebug()  <<  e.tagName()  <<  e.attribute("id"); //打印键值对，tagName和nodeName是一个东西
            QDomNodeList list  =  e.childNodes();
            for(int i = 0; i<list.count();i++) //遍历子元素，count和size都可以用, 可用于标签数计数
            {
                QDomNode n = list.at(i);
                if(node.isElement())
                {
//                    qDebug()  <<  n.nodeName() << ":" << n.toElement().text();
                    if(n.nodeName() != "root")
                        map.insert(n.nodeName(),n.toElement().text());
                }
            }
        }
        node = node.nextSibling(); //下一个兄弟节点, nextSiblingElement()是下一个兄弟元素，都差不多
    }
#endif
    return map;
}

