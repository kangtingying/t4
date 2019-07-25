#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QDialog>
#include <QWidget>
#include <QMap>


class XPlugin;

enum MODE{
    BACKUP,
    RESTORE,
    EXPORT,
    CLEAR,
    SAVEAS,
    OPENACTIONTABLE,
    REMOVEACTIONTABLE
};

namespace Ui {
class FileManager;
}

class FileManager : public QDialog
{
    Q_OBJECT

public:
    explicit FileManager(QWidget *parent = 0);
    ~FileManager();

    void showEvent(QShowEvent *e);

    void attachPlugin(XPlugin *p);

    void setPath( const QString &path ){m_Path = path;}
    QString path(){ return m_Path; }

    void setTitle(const QString title = QString("Backup"));
    QString title();

    QString fileName();

    int iGetFileContent(const QString &path, const QString &name, QByteArray &_ba);

    bool hasDir(){ return m_dirList.size()==0;}

    bool hasFile(){ return m_fileList.size()==0; }

    void setMode(MODE mode);
    MODE mode();

    void on_mode_changed(MODE m);

    QString strResult();

Q_SIGNALS:

    void sigAccept(QString text);

protected:
    virtual void doStartWork();

    virtual void _doStarkWork(QString dir);

private:
    int getDirInfo();

    void clear();

private slots:

    void on_lineEdit_textChanged(const QString &arg1);

    void on_treeView_activated(const QModelIndex &index);



    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::FileManager *ui;
    XPlugin *m_pPlugin;
    QStringList m_dirList;
    QStringList m_fileList;
    QStringList mrpFileList;
    QString m_Path;

    QString m_result;

    MODE m_mode;
};

#endif // FILEMANAGER_H
