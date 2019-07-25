/********************************************************************************
** Form generated from reading UI file 'hlx.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HLX_H
#define UI_HLX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HLX
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;

    void setupUi(QWidget *HLX)
    {
        if (HLX->objectName().isEmpty())
            HLX->setObjectName(QStringLiteral("HLX"));
        HLX->resize(123, 20);
        horizontalLayout = new QHBoxLayout(HLX);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(HLX);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(HLX);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(comboBox);


        retranslateUi(HLX);

        QMetaObject::connectSlotsByName(HLX);
    } // setupUi

    void retranslateUi(QWidget *HLX)
    {
        HLX->setWindowTitle(QApplication::translate("HLX", "Form", nullptr));
        label->setText(QApplication::translate("HLX", "TextLabel", nullptr));
        comboBox->setItemText(0, QApplication::translate("HLX", "H", nullptr));
        comboBox->setItemText(1, QApplication::translate("HLX", "L", nullptr));
        comboBox->setItemText(2, QApplication::translate("HLX", "X", nullptr));

    } // retranslateUi

};

namespace Ui {
    class HLX: public Ui_HLX {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HLX_H
