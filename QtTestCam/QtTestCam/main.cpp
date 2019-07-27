#include "QtTestCam.h"
#include "QtLogDlg.h"
#include "CrossDlg.h"
#include <QtWidgets/QApplication>

#include "checksn.h"
#include <fstream>
#include <iostream>  
#include <QMessageBox>

int g_active_flag = 0;
int g_time_flag = 0;

using namespace std;

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);

	ifstream fin("ac.dat");
	char str[200];

	if (fin.fail())
	{
		QMessageBox::information(this, "Title", QString::fromLocal8Bit("³ÌÐòÎ´¼¤»î£¡"));

		//	exit(-1);
	}
	else
	{
		fin >> str;
	}
	//	AfxMessageBox(_T(str));

	char mac[200];
	getMac(mac);
	//	AfxMessageBox(_T(mac));

	if (CheckSN(mac, str))
	{
		g_active_flag = 1;
	}

	else
	{
		g_active_flag = 0;
		CActiveDlg adlg;
		adlg.DoModal();
	}

	if (CheckTime(str))
	{
		g_time_flag = 1;
	}
	else
	{
		g_time_flag = 0;
		AfxMessageBox("×¢²áÂëÒÑÊ§Ð§£¡");
		CActiveDlg adlg;
		adlg.DoModal();
	}


	QtTestCam w;
	w.show();

	//QtLogDlg w;
	//w.show();
	//CrossDlg w;
	//w.show();

	return a.exec();
}
