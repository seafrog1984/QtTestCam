/********************************************************************************
** Form generated from reading UI file 'CrossDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CROSSDLG_H
#define UI_CROSSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "mylabel.h"

QT_BEGIN_NAMESPACE

class Ui_CrossDlg
{
public:
    MyLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *btn_cross;

    void setupUi(QWidget *CrossDlg)
    {
        if (CrossDlg->objectName().isEmpty())
            CrossDlg->setObjectName(QStringLiteral("CrossDlg"));
        CrossDlg->resize(705, 839);
        label = new MyLabel(CrossDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 40, 240, 320));
        label_2 = new QLabel(CrossDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(350, 40, 256, 320));
        label_2->setMinimumSize(QSize(256, 320));
        label_2->setMaximumSize(QSize(256, 320));
        label_2->setStyleSheet(QLatin1String("border-color: rgb(255, 85, 0);\n"
"border-width: 2px;"));
        label_2->setMargin(2);
        label_3 = new QLabel(CrossDlg);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 440, 240, 256));
        label_3->setMinimumSize(QSize(240, 256));
        label_3->setMaximumSize(QSize(240, 256));
        btn_cross = new QPushButton(CrossDlg);
        btn_cross->setObjectName(QStringLiteral("btn_cross"));
        btn_cross->setGeometry(QRect(520, 560, 75, 23));

        retranslateUi(CrossDlg);

        QMetaObject::connectSlotsByName(CrossDlg);
    } // setupUi

    void retranslateUi(QWidget *CrossDlg)
    {
        CrossDlg->setWindowTitle(QApplication::translate("CrossDlg", "CrossDlg", 0));
        label->setText(QApplication::translate("CrossDlg", "TextLabel", 0));
        label_2->setText(QApplication::translate("CrossDlg", "TextLabel", 0));
        label_3->setText(QApplication::translate("CrossDlg", "TextLabel", 0));
        btn_cross->setText(QApplication::translate("CrossDlg", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class CrossDlg: public Ui_CrossDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CROSSDLG_H
