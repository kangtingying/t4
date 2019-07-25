#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QWidget>
#include <QListWidget>
#include <QTreeWidget>
#include "../plugin/xpage.h"

namespace Ui {
class FileExplorer;
}

namespace mrx_t4 {
class FileExplorer : public XPage
{
    Q_OBJECT

public:
    explicit FileExplorer(QWidget *parent = 0);
    ~FileExplorer();

private:
    void setupUi();

protected:
    virtual void resizeEvent( QResizeEvent * resize );

private:
    Ui::FileExplorer *ui;
    QSplitter *m_pSplitter;
    QListWidget *m_pDirectory;
    QTreeWidget *m_pFileTree;
};
}
#endif // FILEEXPLORER_H
