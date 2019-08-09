#include "QtTestCam.h"
#include <QtWidgets/QApplication>
#include "QtSetDlg.h"


int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	//QtTestCam w;
	//w.show();
	QtSetDlg w;
	w.show();

	return a.exec();
}
