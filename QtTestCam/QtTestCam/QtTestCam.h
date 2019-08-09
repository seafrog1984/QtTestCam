#pragma once

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QPushButton>

#include<QGraphicsScene>  
#include<QGraphicsView>                   //graphicsview��
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>  //opencv����

#include <qfiledialog.h>                //getopenfilename ������
#include <qlabel.h>                     //label��
#include <QTimer>
#include <QImage>
#include<QStatusBar>

#include "IRSDK.h"
#include "QtSetDlg.h"

//#pragma comment(lib, "IRSDKlib_Win32.lib")
//#pragma comment(lib, "Shell32.lib")

using namespace cv;

#include "ui_QtTestCam.h"

enum MouseState //�Լ������״̬����
{
	L_C,//left click
	R_C,
	M_C,
	L_DC,//left double click
	R_DC,
	Wheel,//wheel move
	Release
};

class QtTestCam : public QMainWindow
{
	Q_OBJECT

public:
	QtTestCam(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent* event);
	void changeLabel();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void wheelEvent(QWheelEvent*event); //�����������¹��ֵ�ʱ������
	
private slots:
	//void on_OpenFig_clicked();
//	void ReadFrame();
	void OpenCameraClicked();
	void CloseCameraClicked();
	void btn_testClicked();
	void testMouseMoved();

	void testSingal();

	void time_update(); //ʱ����²ۺ�����״̬����ʾʱ��

private:
	Ui::QtTestCamClass ui;
	virtual void resizeEvent(QResizeEvent * event);

	QtSetDlg *dlg;
	VideoCapture capture;
	QTimer *timer;
	QLabel *currentTimeLabel;
	Mat frame;

	QString ir_ip;
	//Mat image;
	//QLabel *label;

	int j;
	int m_dragging;

	QStatusBar sBar;

public:
	void changeLabel(int totalNum, int imagePerRow);

	T_IPADDR strIPAddr;

protected:

	void mouseReleaseEvent(QMouseEvent*event);
	void mousePressEvent(QMouseEvent*event);
//	void mouseMoveEvent(QMouseEvent*event);
	void paintEvent(QPaintEvent *event);

	void magicTime();
};
