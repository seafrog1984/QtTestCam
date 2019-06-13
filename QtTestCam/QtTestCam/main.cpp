#include "QtTestCam.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtTestCam w;
	w.show();
	return a.exec();
}
