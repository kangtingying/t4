#ifndef MEGALOGLISTMODEL_H
#define MEGALOGLISTMODEL_H

#include <QAbstractListModel>

class LogStr
{
public:
    enum eLogLevel
    {
        log_info,
        log_warning,
        log_error
    };

public:
    static QString toString( eLogLevel lev );

public:
    eLogLevel mLevel;
    QString mStr;
};

class MegaLogListModel : public QAbstractListModel
{
    Q_OBJECT    

public:
    explicit MegaLogListModel(QObject *parent = nullptr);
    ~MegaLogListModel();

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
public:
    void append( const QString &content, LogStr::eLogLevel lev=LogStr::log_info );
    void clear();

Q_SIGNALS:
    void signal_current_changed( int cur );

private:
//    QStringList mItemList;
    QList< LogStr *> mItemList;
    int mMaxCount;

};

#endif // MEGALISTMODEL_H
