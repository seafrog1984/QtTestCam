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
    QLineEdit *lineEdit_user;
    QLineEdit *lineEdit_pw;
    QPushButton *btn_log;

    void setupUi(QWidget *QtLogDlg)
    {
        if (QtLogDlg->objectName().isEmpty())
            QtLogDlg->setObjectName(QStringLiteral("QtLogDlg"));
        QtLogDlg->resize(400, 300);
        lineEdit_user = new QLineEdit(QtLogDlg);
        lineEdit_user->setObjectName(QStringLiteral("lineEdit_user"));
        lineEdit_user->setGeometry(QRect(130, 30, 113, 20));
        lineEdit_pw = new QLineEdit(QtLogDlg);
        lineEdit_pw->setObjectName(QStringLiteral("lineEdit_pw"));
        lineEdit_pw->setGeometry(QRect(130, 80, 113, 20));
        btn_log = new QPushButton(QtLogDlg);
        btn_log->setObjectName(QStringLiteral("btn_log"));
        btn_log->setGeometry(QRect(150, 140, 75, 23));

        retranslateUi(QtLogDlg);

        QMetaObject::connectSlotsByName(QtLogDlg);
    } // setupUi

    void retranslateUi(QWidget *QtLogDlg)
    {
        QtLogDlg->setWindowTitle(QApplication::translate("QtLogDlg", "QtLogDlg", 0));
        btn_log->setText(QApplication::translate("QtLogDlg", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class QtLogDlg: public Ui_QtLogDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTLOGDLG_H
