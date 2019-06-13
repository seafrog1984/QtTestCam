/********************************************************************************
** Form generated from reading UI file 'QtTestCam.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTESTCAM_H
#define UI_QTTESTCAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtTestCamClass
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_IP;
    QComboBox *cbox_ip;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_connect;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QToolBox *toolBox;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btn_play;
    QPushButton *btn_stop;
    QHBoxLayout *horizontalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QGridLayout *gridshow;
    QPushButton *btn_test;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtTestCamClass)
    {
        if (QtTestCamClass->objectName().isEmpty())
            QtTestCamClass->setObjectName(QStringLiteral("QtTestCamClass"));
        QtTestCamClass->resize(1153, 562);
        centralWidget = new QWidget(QtTestCamClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(50, 30, 461, 391));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(531, 31, 241, 381));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setMinimumSize(QSize(180, 0));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_IP = new QLabel(groupBox);
        label_IP->setObjectName(QStringLiteral("label_IP"));

        horizontalLayout->addWidget(label_IP);

        cbox_ip = new QComboBox(groupBox);
        cbox_ip->setObjectName(QStringLiteral("cbox_ip"));
        cbox_ip->setEnabled(true);
        cbox_ip->setEditable(true);

        horizontalLayout->addWidget(cbox_ip);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btn_connect = new QPushButton(groupBox);
        btn_connect->setObjectName(QStringLiteral("btn_connect"));

        horizontalLayout_2->addWidget(btn_connect);

        horizontalLayout_2->setStretch(0, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        toolBox = new QToolBox(groupBox_2);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        page_3->setGeometry(QRect(0, 0, 199, 222));
        verticalLayout_4 = new QVBoxLayout(page_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        btn_play = new QPushButton(page_3);
        btn_play->setObjectName(QStringLiteral("btn_play"));
        btn_play->setMinimumSize(QSize(50, 50));

        verticalLayout_4->addWidget(btn_play);

        btn_stop = new QPushButton(page_3);
        btn_stop->setObjectName(QStringLiteral("btn_stop"));
        btn_stop->setMinimumSize(QSize(50, 50));

        verticalLayout_4->addWidget(btn_stop);

        toolBox->addItem(page_3, QString::fromUtf8("\350\256\276\345\244\207\346\216\247\345\210\266"));

        verticalLayout_3->addWidget(toolBox);


        verticalLayout->addWidget(groupBox_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(2, 8);

        verticalLayout_2->addWidget(groupBox);

        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(40, 20, 471, 411));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 469, 409));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 0, 451, 401));
        scrollArea->setWidget(scrollAreaWidgetContents);
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(780, 20, 351, 441));
        widget_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 170, 0);"));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridshow = new QGridLayout();
        gridshow->setSpacing(6);
        gridshow->setObjectName(QStringLiteral("gridshow"));

        gridLayout_2->addLayout(gridshow, 0, 0, 1, 1);

        btn_test = new QPushButton(centralWidget);
        btn_test->setObjectName(QStringLiteral("btn_test"));
        btn_test->setGeometry(QRect(610, 430, 75, 23));
        QtTestCamClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtTestCamClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1153, 23));
        QtTestCamClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtTestCamClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtTestCamClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtTestCamClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtTestCamClass->setStatusBar(statusBar);

        retranslateUi(QtTestCamClass);
        QObject::connect(btn_play, SIGNAL(clicked()), QtTestCamClass, SLOT(on_OpenFig_clicked()));

        cbox_ip->setCurrentIndex(-1);
        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QtTestCamClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtTestCamClass)
    {
        QtTestCamClass->setWindowTitle(QApplication::translate("QtTestCamClass", "QtTestCam", 0));
        groupBox->setTitle(QApplication::translate("QtTestCamClass", "\350\256\276\345\244\207\350\277\236\346\216\245", 0));
        label_IP->setText(QApplication::translate("QtTestCamClass", "IP:", 0));
        cbox_ip->setCurrentText(QString());
        btn_connect->setText(QApplication::translate("QtTestCamClass", "\350\277\236\346\216\245", 0));
        groupBox_2->setTitle(QString());
        btn_play->setText(QApplication::translate("QtTestCamClass", "\346\222\255\346\224\276", 0));
        btn_stop->setText(QApplication::translate("QtTestCamClass", "\345\201\234\346\255\242", 0));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("QtTestCamClass", "\350\256\276\345\244\207\346\216\247\345\210\266", 0));
        label->setText(QApplication::translate("QtTestCamClass", "TextLabel", 0));
        btn_test->setText(QApplication::translate("QtTestCamClass", "\346\226\255\345\261\202\346\265\213\350\257\225", 0));
    } // retranslateUi

};

namespace Ui {
    class QtTestCamClass: public Ui_QtTestCamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTESTCAM_H
