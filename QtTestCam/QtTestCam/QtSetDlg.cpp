#include "QtSetDlg.h"


extern QString g_IP;

QtSetDlg::QtSetDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.lineEdit->setText(g_IP);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(setIP()));

}

QtSetDlg::~QtSetDlg()
{
}


void QtSetDlg::setIP()
{
	g_IP=ui.lineEdit->text();

}