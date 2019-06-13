/********************************************************************************
** Form generated from reading UI file 'QtSetDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSETDLG_H
#define UI_QTSETDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtSetDlg
{
public:
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *QtSetDlg)
    {
        if (QtSetDlg->objectName().isEmpty())
            QtSetDlg->setObjectName(QStringLiteral("QtSetDlg"));
        QtSetDlg->resize(400, 300);
        lineEdit = new QLineEdit(QtSetDlg);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(90, 30, 113, 20));
        pushButton = new QPushButton(QtSetDlg);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(240, 30, 75, 23));

        retranslateUi(QtSetDlg);

        QMetaObject::connectSlotsByName(QtSetDlg);
    } // setupUi

    void retranslateUi(QWidget *QtSetDlg)
    {
        QtSetDlg->setWindowTitle(QApplication::translate("QtSetDlg", "QtSetDlg", 0));
        pushButton->setText(QApplication::translate("QtSetDlg", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class QtSetDlg: public Ui_QtSetDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSETDLG_H
