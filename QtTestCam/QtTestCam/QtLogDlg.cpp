#include "QtLogDlg.h"
#include "QtTestCam.h"

QtLogDlg::QtLogDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(log()));


}

QtLogDlg::~QtLogDlg()
{
}

void QtLogDlg::log()
{
	QtTestCam *w=new QtTestCam;
	w->show();

	this->~QtLogDlg();
}