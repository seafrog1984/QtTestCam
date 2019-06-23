#include "QtTestCam.h"
#include "QtLogDlg.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	//QtTestCam w;
	//w.show();

	QtLogDlg w;
	w.show();
	return a.exec();
}
