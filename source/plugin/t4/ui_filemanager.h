/********************************************************************************
** Form generated from reading UI file 'filemanager.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEMANAGER_H
#define UI_FILEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileManager
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FileManager)
    {
        if (FileManager->objectName().isEmpty())
            FileManager->setObjectName(QStringLiteral("FileManager"));
        FileManager->resize(441, 174);
        FileManager->setMinimumSize(QSize(200, 100));
        verticalLayout = new QVBoxLayout(FileManager);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeView = new QTreeView(FileManager);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(treeView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(FileManager);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(FileManager);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setInputMethodHints(Qt::ImhLatinOnly);

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(FileManager);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(FileManager);
        QObject::connect(buttonBox, SIGNAL(accepted()), FileManager, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FileManager, SLOT(reject()));

        QMetaObject::connectSlotsByName(FileManager);
    } // setupUi

    void retranslateUi(QDialog *FileManager)
    {
        FileManager->setWindowTitle(QApplication::translate("FileManager", "Backup", nullptr));
        label->setText(QApplication::translate("FileManager", "Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileManager: public Ui_FileManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEMANAGER_H
