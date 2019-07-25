#include "fileexplorer.h"
#include "ui_fileexplorer.h"

namespace mrx_t4 {
FileExplorer::FileExplorer(QWidget *parent) :
    XPage(parent),
    ui(new Ui::FileExplorer)
{
    ui->setupUi(this);

    setupUi();

    setContextHelp("explorer");
}

FileExplorer::~FileExplorer()
{
    delete ui;
}

void FileExplorer::setupUi()
{
    m_pSplitter = new QSplitter(ui->widget);

    m_pFileTree = new QTreeWidget();
    m_pDirectory = new QListWidget();

    m_pFileTree->setObjectName("tree_filetree");
    m_pDirectory->setObjectName("list_directory");

    m_pSplitter->addWidget( m_pFileTree );
    m_pSplitter->addWidget( m_pDirectory );

    m_pSplitter->setStretchFactor( 0, 1 );
    m_pSplitter->setStretchFactor( 1, 3 );
}

void FileExplorer::resizeEvent( QResizeEvent * resize )
{
    m_pSplitter->resize( ui->widget->size() );
}

}
