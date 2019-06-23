/********************************************************************************
** Form generated from reading UI file 'QtLogDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTLOGDLG_H
#define UI_QTLOGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtLogDlg
{
public:
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;

    void setupUi(QWidget *QtLogDlg)
    {
        if (QtLogDlg->objectName().isEmpty())
            QtLogDlg->setObjectName(QStringLiteral("QtLogDlg"));
        QtLogDlg->resize(400, 300);
        lineEdit = new QLineEdit(QtLogDlg);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(130, 30, 113, 20));
        lineEdit_2 = new QLineEdit(QtLogDlg);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(130, 80, 113, 20));
        pushButton = new QPushButton(QtLogDlg);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(150, 140, 75, 23));

        retranslateUi(QtLogDlg);

        QMetaObject::connectSlotsByName(QtLogDlg);
    } // setupUi

    void retranslateUi(QWidget *QtLogDlg)
    {
        QtLogDlg->setWindowTitle(QApplication::translate("QtLogDlg", "QtLogDlg", 0));
        pushButton->setText(QApplication::translate("QtLogDlg", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class QtLogDlg: public Ui_QtLogDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTLOGDLG_H
