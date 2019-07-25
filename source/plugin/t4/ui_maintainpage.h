/********************************************************************************
** Form generated from reading UI file 'maintainpage.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINTAINPAGE_H
#define UI_MAINTAINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaintainPage
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QGridLayout *gridLayout;
    QPushButton *btnDemo;
    QComboBox *cmbDemo;
    QTextEdit *txtDemoInfo;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer;
    QPushButton *btnHistory;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnFold;
    QPushButton *btnUpdate;
    QPushButton *btnReset;
    QPushButton *btnBuild;
    QWidget *tab_5;
    QGridLayout *gridLayout_5;
    QPushButton *btnClearBackup;
    QPushButton *btnRestore;
    QPushButton *btnBackup;
    QSpacerItem *verticalSpacer_4;
    QPushButton *btnExport;
    QSpacerItem *horizontalSpacer_5;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QPushButton *btnRestart;
    QPushButton *btnShutDown;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QWidget *tab_4;
    QGridLayout *gridLayout_4;
    QComboBox *cmbRstUser;
    QPushButton *btnChange;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnResetPw;
    QLabel *label;
    QComboBox *cmbUser;
    QCheckBox *chkAutoLogin;
    QLabel *label_2;

    void setupUi(QWidget *MaintainPage)
    {
        if (MaintainPage->objectName().isEmpty())
            MaintainPage->setObjectName(QStringLiteral("MaintainPage"));
        MaintainPage->resize(463, 245);
        verticalLayout = new QVBoxLayout(MaintainPage);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(MaintainPage);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout = new QGridLayout(tab_3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnDemo = new QPushButton(tab_3);
        btnDemo->setObjectName(QStringLiteral("btnDemo"));
        btnDemo->setMaximumSize(QSize(100, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/image/icon/play-circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDemo->setIcon(icon);

        gridLayout->addWidget(btnDemo, 0, 1, 1, 1);

        cmbDemo = new QComboBox(tab_3);
        cmbDemo->addItem(QString());
        cmbDemo->addItem(QString());
        cmbDemo->setObjectName(QStringLiteral("cmbDemo"));

        gridLayout->addWidget(cmbDemo, 0, 0, 1, 1);

        txtDemoInfo = new QTextEdit(tab_3);
        txtDemoInfo->setObjectName(QStringLiteral("txtDemoInfo"));
        txtDemoInfo->setEnabled(false);

        gridLayout->addWidget(txtDemoInfo, 1, 0, 1, 3);

        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setScaledContents(false);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(label_3, 1, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 2);

        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalSpacer = new QSpacerItem(355, 149, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 5, 0, 1, 3);

        btnHistory = new QPushButton(tab);
        btnHistory->setObjectName(QStringLiteral("btnHistory"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/res/image/icon/history.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnHistory->setIcon(icon1);

        gridLayout_3->addWidget(btnHistory, 1, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        btnFold = new QPushButton(tab);
        btnFold->setObjectName(QStringLiteral("btnFold"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/res/image/icon/201.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnFold->setIcon(icon2);

        gridLayout_3->addWidget(btnFold, 2, 0, 1, 1);

        btnUpdate = new QPushButton(tab);
        btnUpdate->setObjectName(QStringLiteral("btnUpdate"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/res/image/icon/upgrade.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUpdate->setIcon(icon3);

        gridLayout_3->addWidget(btnUpdate, 0, 0, 1, 1);

        btnReset = new QPushButton(tab);
        btnReset->setObjectName(QStringLiteral("btnReset"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/res/image/icon/ErrorSetting.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnReset->setIcon(icon4);

        gridLayout_3->addWidget(btnReset, 3, 0, 1, 1);

        btnBuild = new QPushButton(tab);
        btnBuild->setObjectName(QStringLiteral("btnBuild"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/res/image/icon/build.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBuild->setIcon(icon5);

        gridLayout_3->addWidget(btnBuild, 4, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        gridLayout_5 = new QGridLayout(tab_5);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        btnClearBackup = new QPushButton(tab_5);
        btnClearBackup->setObjectName(QStringLiteral("btnClearBackup"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/res/image/icon/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClearBackup->setIcon(icon6);

        gridLayout_5->addWidget(btnClearBackup, 4, 0, 1, 2);

        btnRestore = new QPushButton(tab_5);
        btnRestore->setObjectName(QStringLiteral("btnRestore"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/res/image/icon/guanlianshebei.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRestore->setIcon(icon7);

        gridLayout_5->addWidget(btnRestore, 1, 0, 2, 2);

        btnBackup = new QPushButton(tab_5);
        btnBackup->setObjectName(QStringLiteral("btnBackup"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/res/image/icon/fuzhi.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBackup->setIcon(icon8);

        gridLayout_5->addWidget(btnBackup, 0, 0, 1, 2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_4, 5, 0, 1, 1);

        btnExport = new QPushButton(tab_5);
        btnExport->setObjectName(QStringLiteral("btnExport"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/res/image/icon/icon_baocun.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExport->setIcon(icon9);

        gridLayout_5->addWidget(btnExport, 3, 0, 1, 2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        tabWidget->addTab(tab_5, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        btnRestart = new QPushButton(tab_2);
        btnRestart->setObjectName(QStringLiteral("btnRestart"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/res/image/icon/332.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRestart->setIcon(icon10);

        gridLayout_2->addWidget(btnRestart, 0, 0, 1, 1);

        btnShutDown = new QPushButton(tab_2);
        btnShutDown->setObjectName(QStringLiteral("btnShutDown"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/res/image/icon/246.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnShutDown->setIcon(icon11);

        gridLayout_2->addWidget(btnShutDown, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        gridLayout_4 = new QGridLayout(tab_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        cmbRstUser = new QComboBox(tab_4);
        cmbRstUser->addItem(QString());
        cmbRstUser->addItem(QString());
        cmbRstUser->setObjectName(QStringLiteral("cmbRstUser"));

        gridLayout_4->addWidget(cmbRstUser, 3, 0, 1, 1);

        btnChange = new QPushButton(tab_4);
        btnChange->setObjectName(QStringLiteral("btnChange"));

        gridLayout_4->addWidget(btnChange, 2, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 4, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 1, 3, 1, 1);

        btnResetPw = new QPushButton(tab_4);
        btnResetPw->setObjectName(QStringLiteral("btnResetPw"));

        gridLayout_4->addWidget(btnResetPw, 3, 1, 1, 1);

        label = new QLabel(tab_4);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_4->addWidget(label, 1, 0, 1, 1);

        cmbUser = new QComboBox(tab_4);
        cmbUser->addItem(QString());
        cmbUser->addItem(QString());
        cmbUser->setObjectName(QStringLiteral("cmbUser"));

        gridLayout_4->addWidget(cmbUser, 1, 1, 1, 1);

        chkAutoLogin = new QCheckBox(tab_4);
        chkAutoLogin->setObjectName(QStringLiteral("chkAutoLogin"));
        chkAutoLogin->setLayoutDirection(Qt::RightToLeft);

        gridLayout_4->addWidget(chkAutoLogin, 0, 0, 1, 1);

        label_2 = new QLabel(tab_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(100, 100));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/res/image/icon/51.png")));
        label_2->setScaledContents(true);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_4->addWidget(label_2, 0, 2, 4, 1);

        tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(MaintainPage);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MaintainPage);
    } // setupUi

    void retranslateUi(QWidget *MaintainPage)
    {
        MaintainPage->setWindowTitle(QApplication::translate("MaintainPage", "Form", nullptr));
#ifndef QT_NO_TOOLTIP
        btnDemo->setToolTip(QApplication::translate("MaintainPage", "Run the slected demo", nullptr));
#endif // QT_NO_TOOLTIP
        btnDemo->setText(QApplication::translate("MaintainPage", "start", nullptr));
        cmbDemo->setItemText(0, QApplication::translate("MaintainPage", "Demo1", nullptr));
        cmbDemo->setItemText(1, QApplication::translate("MaintainPage", "Demo2", nullptr));

        label_3->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MaintainPage", "Demo", nullptr));
#ifndef QT_NO_TOOLTIP
        btnHistory->setToolTip(QApplication::translate("MaintainPage", "Show the version history", nullptr));
#endif // QT_NO_TOOLTIP
        btnHistory->setText(QApplication::translate("MaintainPage", "History", nullptr));
#ifndef QT_NO_TOOLTIP
        btnFold->setToolTip(QApplication::translate("MaintainPage", "Fold the device for package", nullptr));
#endif // QT_NO_TOOLTIP
        btnFold->setText(QApplication::translate("MaintainPage", "Fold", nullptr));
#ifndef QT_NO_TOOLTIP
        btnUpdate->setToolTip(QApplication::translate("MaintainPage", "Update the device", nullptr));
#endif // QT_NO_TOOLTIP
        btnUpdate->setText(QApplication::translate("MaintainPage", "Update", nullptr));
#ifndef QT_NO_TOOLTIP
        btnReset->setToolTip(QApplication::translate("MaintainPage", "Restore the factory settings", nullptr));
#endif // QT_NO_TOOLTIP
        btnReset->setText(QApplication::translate("MaintainPage", "Reset", nullptr));
#ifndef QT_NO_TOOLTIP
        btnBuild->setToolTip(QApplication::translate("MaintainPage", "Match the components of the device", nullptr));
#endif // QT_NO_TOOLTIP
        btnBuild->setText(QApplication::translate("MaintainPage", "Build", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MaintainPage", "Operate", nullptr));
#ifndef QT_NO_TOOLTIP
        btnClearBackup->setToolTip(QApplication::translate("MaintainPage", "Clear all backups", nullptr));
#endif // QT_NO_TOOLTIP
        btnClearBackup->setText(QApplication::translate("MaintainPage", "Clear Backup", nullptr));
#ifndef QT_NO_TOOLTIP
        btnRestore->setToolTip(QApplication::translate("MaintainPage", "Restore the settings from backup", nullptr));
#endif // QT_NO_TOOLTIP
        btnRestore->setText(QApplication::translate("MaintainPage", "Restore", nullptr));
#ifndef QT_NO_TOOLTIP
        btnBackup->setToolTip(QApplication::translate("MaintainPage", "Backup the device settings", nullptr));
#endif // QT_NO_TOOLTIP
        btnBackup->setText(QApplication::translate("MaintainPage", "Backup", nullptr));
#ifndef QT_NO_TOOLTIP
        btnExport->setToolTip(QApplication::translate("MaintainPage", "Export the setting to local disk", nullptr));
#endif // QT_NO_TOOLTIP
        btnExport->setText(QApplication::translate("MaintainPage", "Export", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MaintainPage", "Backup", nullptr));
#ifndef QT_NO_TOOLTIP
        btnRestart->setToolTip(QApplication::translate("MaintainPage", "Reboot the device", nullptr));
#endif // QT_NO_TOOLTIP
        btnRestart->setText(QApplication::translate("MaintainPage", "Restart", nullptr));
#ifndef QT_NO_TOOLTIP
        btnShutDown->setToolTip(QApplication::translate("MaintainPage", "Shutdown the device", nullptr));
#endif // QT_NO_TOOLTIP
        btnShutDown->setText(QApplication::translate("MaintainPage", "Shutdown", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MaintainPage", "Power", nullptr));
        cmbRstUser->setItemText(0, QApplication::translate("MaintainPage", "Operator", nullptr));
        cmbRstUser->setItemText(1, QApplication::translate("MaintainPage", "Administrator", nullptr));

#ifndef QT_NO_TOOLTIP
        cmbRstUser->setToolTip(QApplication::translate("MaintainPage", "Reset user", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        btnChange->setToolTip(QApplication::translate("MaintainPage", "Change password", nullptr));
#endif // QT_NO_TOOLTIP
        btnChange->setText(QApplication::translate("MaintainPage", "Change Pw", nullptr));
#ifndef QT_NO_TOOLTIP
        btnResetPw->setToolTip(QApplication::translate("MaintainPage", "Reset the password", nullptr));
#endif // QT_NO_TOOLTIP
        btnResetPw->setText(QApplication::translate("MaintainPage", "Reset Pw", nullptr));
        label->setText(QApplication::translate("MaintainPage", "User", nullptr));
        cmbUser->setItemText(0, QApplication::translate("MaintainPage", "Operator", nullptr));
        cmbUser->setItemText(1, QApplication::translate("MaintainPage", "Administrator", nullptr));

#ifndef QT_NO_TOOLTIP
        cmbUser->setToolTip(QApplication::translate("MaintainPage", "Current user role", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        chkAutoLogin->setToolTip(QApplication::translate("MaintainPage", "Auto login", nullptr));
#endif // QT_NO_TOOLTIP
        chkAutoLogin->setText(QApplication::translate("MaintainPage", "Auto login", nullptr));
        label_2->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MaintainPage", "User", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaintainPage: public Ui_MaintainPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINTAINPAGE_H
