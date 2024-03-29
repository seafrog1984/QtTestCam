#include "QtTestCam.h"
#include <fstream>
#include <iostream>  
#include <stdlib.h>  
#include <string>
#include <math.h>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<QMessageBox>
#include<QLineEdit>
#include<QWidget>
#include<QSize>
#include<QMouseEvent>
#include<QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <CvxText.h>
#include <QDateTime>
IplImage *img0, *img1;
Mat temp1;

//#include "IRSDK.h"
#include <imgProcDll.h>
#include "MyLabel.h"

using namespace cv;
using namespace std;


#define IMAGE_WIDTH  384
#define IMAGE_HEIGHT 288
//#define IMAGE_WIDTH  320
//#define IMAGE_HEIGHT 240

//unsigned short* g_pDst = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];;
//unsigned char* g_pRgb = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT * 3];;
//float   g_contranst;
//float   g_bright;
//int	PalType;
//int Pal;

struct shape{
	bool del_flag;
	int shape_type; //0矩形，1圆形
	int lt_x, lt_y;//左上角坐标
	int rb_x, rb_y;//右下角坐标
	double t_max;
	double t_min;
	double t_aver;
	double t_msd;
	string comment;//注释
};
typedef struct shape Shape;

Shape allshape[8][30] = { { false, 0, 0, 0, 0, 0, 0, 0, 0, 0, "None" } };
int shape_no = 0;

Frame g_frame;

int X[4] = { 50, 200, 300, 50 }, Y[4] = { 100, 100, 200, 200 };

int g_flag_play = 1;
int g_picNum = 0;

unsigned short *g_pData[12];
Mat g_Img[12];
Mat g_temper[12];
QImage g_qImgShow[12];
QString g_tempFolder;
QString g_IP = "192.168.1.60";

float g_referTemper = 22;

float m_ratio = 1;

void draw_shape(Mat& img, Shape allshape[], int shape_no)
{
	char temp[50];
	img0 = &IplImage(temp1);
	img1 = &IplImage(temp1);
	for (int i = 0; i < shape_no; i++)
	{
		if (allshape[i].comment != "None"&&allshape[i].del_flag == false)
		{
			char temp[50];

			sprintf_s(temp, ("%s"), allshape[i].comment);
			////显示文字
			////读取TTF字体文件
			//CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); // "zenhei.ttf"为黑体常规
			////CvxText text(""); // "zenhei.ttf"为黑体常规
			////	const char *msg = "在OpenCV中输出汉字！";
			//float p = 1;
			////CvScalar fsize;
			////////设置字体属性 字体大小/空白比例/间隔比例/旋转角度
			////fsize = cvScalar(20, 0.5, 0.1, 0);
			//text.setMySize(10, 10);
			//text.setFont(NULL, NULL, NULL, &p);
			////	rectangle(imgt, pre_pt, cur_pt, Scalar(255, 255, 255), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形
			////	putText(imgt, msg, Point(cur_pt.x,cur_pt.y+15), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255));

			//text.putText(img1, temp, Point(allshape[i].rb_x, allshape[i].rb_y + 15), CV_RGB(0, 0, 0));
		}
	}
	Mat imgt;
	img = imgt = Mat(img1);

	for (int i = 0; i < shape_no; i++)
	{
		if (allshape[i].del_flag == false)
		{

			if (allshape[i].shape_type == 0)
			{
				rectangle(img, Point(allshape[i].lt_x, allshape[i].lt_y), Point(allshape[i].rb_x, allshape[i].rb_y), Scalar(255, 255, 255), 1, 8, 0);//根据初始点和结束点，将矩形画到img上
			}
			else if (allshape[i].shape_type == 1)
			{
				int radius = sqrt(pow(allshape[i].lt_x - allshape[i].rb_x, 2) + pow(allshape[i].lt_y - allshape[i].rb_y, 2)) / 2;
				circle(imgt, Point((allshape[i].lt_x + allshape[i].rb_x) / 2, (allshape[i].lt_y + allshape[i].rb_y) / 2), radius, Scalar(255, 255, 255), 1, 8, 0);
			}
			else if (allshape[i].shape_type == 2)
			{
				ellipse(imgt, Point((allshape[i].lt_x + allshape[i].rb_x) / 2, (allshape[i].lt_y + allshape[i].rb_y) / 2), Size(abs(allshape[i].lt_x - allshape[i].rb_x), abs(allshape[i].lt_y - allshape[i].rb_y)), 0, 0, 360, Scalar(255, 255, 255), 1, 8, 0);
			}

			sprintf_s(temp, "([%02d]-%.2lf,%.2lf,%.2lf,%.2f)", i + 1, allshape[i].t_max, allshape[i].t_min, allshape[i].t_aver, allshape[i].t_msd);
			putText(imgt, temp, Point(allshape[i].rb_x, allshape[i].rb_y), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 0));
		}
	}
}


long FrameProc(long hFrame, long lParam)
{

	Frame* pFrame = (Frame*)hFrame;

	STAT_TEMPER sFull_Temp;


	memcpy(&g_frame, pFrame, sizeof(Frame));


	Mat img;
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

	data2Img(g_frame.buffer, img, IMAGE_HEIGHT, IMAGE_WIDTH, 16, 2, 2,25);


	cv::resize(img, img, cv::Size(480, 640));
	QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_RGB888);

	Ui::QtTestCamClass *pui = (Ui::QtTestCamClass*)lParam;

	pui->label->setPixmap(QPixmap::fromImage(image));

	return 1;
}


QtTestCam::QtTestCam(QWidget *parent)
: QMainWindow(parent)
{
	//	Mat img = imread("E:\\lena_color.bmp");

	//namedWindow("img");
	//imshow("img", img);
	//waitKey(0);
	ui.setupUi(this);


	//	timer = new QTimer(this);
	//	connect(timer, SIGNAL(timeout()), this, SLOT(ReadFrame()));
	connect(ui.btn_play, SIGNAL(clicked()), this, SLOT(OpenCameraClicked()));//打开摄像头
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(CloseCameraClicked()));//关闭摄像头
	connect(ui.btn_test, SIGNAL(clicked()), this, SLOT(btn_testClicked()));//测试断层

	connect(ui.label, SIGNAL(clicked()), this, SLOT(testMouseMoved()));

//	ui.label->setBaseSize(QSize(100, 800));

	//ui.label->installEventFilter(this);


	int g_picTotalNum = 12;
	int count = 0;
	for (int x = 0; x < (g_picTotalNum - 1) / 3+1; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			MyLabel *lb = new MyLabel;
		//	lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			lb->setFixedSize(60, 80);
			lb->setText(QString::number(x * 3 + y+1));
			lb->setObjectName(QString::number(x * 3 + y ));
			lb->setFrameShape(QFrame::Box);
			ui.gridshow->addWidget(lb, x, y);
			lb->setStyleSheet("border-width: 4px;border-style: solid;border-color: rgb(255, 170, 0);");
			lb->setAlignment(Qt::AlignCenter);
			count++;
			if (count >= g_picTotalNum) break;
		}
		if (count >= g_picTotalNum) break;
	}


	QGridLayout *topLeftLayout = new QGridLayout;
	topLeftLayout->addWidget(ui.scrollArea,0,0);
	topLeftLayout->addLayout(ui.verticalLayout,0,1);
	topLeftLayout->addWidget(ui.widget_2,0,2);

	ui.centralWidget->setLayout(topLeftLayout);
	
	std::string scanID = "test20190521";
	QDir dir;
	g_tempFolder = dir.currentPath() + "//Temp//" + QString::fromStdString(scanID);

	// 检查目录是否存在，若不存在则新建

	if (!dir.exists(g_tempFolder))
	{
		bool res = dir.mkpath(g_tempFolder);
//		qDebug() << "新建目录是否成功" << res;

	}

	//QLabel *msgLable = new QLabel;
	//msgLable->setMinimumSize(150, 20/*msgLable->sizeHint()*/);
	//msgLable->setAlignment(Qt::AlignHCenter);
	//statusBar()->addWidget(msgLable);
	//statusBar()->showMessage("lin shi xin xi ");//3s后关闭


	currentTimeLabel = new QLabel; // 创建QLabel控件
	//ui.statusBar->addWidget(currentTimeLabel); //在状态栏添加此控件
	ui.statusBar->addPermanentWidget(currentTimeLabel); //在状态栏添加此控件
	QTimer *timer = new QTimer(this);
	timer->start(1000); //每隔1000ms发送timeout的信号
	connect(timer, SIGNAL(timeout()), this, SLOT(time_update()));



}


void QtTestCam::time_update()
{
	//[1] 获取时间
	QDateTime current_time = QDateTime::currentDateTime();
	QString timestr = QString::fromLocal8Bit("温度：  湿度： ")+current_time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示的格式

	currentTimeLabel->setText(timestr); //设置label的文本内容为时间

}



bool QtTestCam::eventFilter(QObject *obj, QEvent *event)
{
	//if (qobject_cast<QLabel*>(obj) == ui.label&&event->type() == QEvent::Paint)
	//{
	//	magicTime();
	//}
	Mat imgt;
	g_Img[0].copyTo(imgt);

	char label[1000];
	char label2[1000];

	if (event->type() == QEvent::MouseMove)
	{

		QMessageBox::information(NULL, "Title", qobject_cast<QLabel*>(obj)->text());
	}


	if (qobject_cast<QLabel*>(obj) == ui.label&&event->type() == QEvent::MouseMove)
	{
		imshow("move", imgt);
	}

	if (qobject_cast<QLabel*>(obj) == ui.label&&event->type()==QEvent::MouseButtonPress)
	{

		QMouseEvent * mevent = (QMouseEvent *)event;
		if (mevent->button() == Qt::LeftButton)
		{
			int x = mevent->pos().x();
			int y = mevent->pos().y();

			if (x < 0) x = 0;
			if (x >= imgt.cols) x = imgt.cols - 1;
			if (y < 0) y = 0;
			if (y >= imgt.rows) y = imgt.rows - 1;

	//		int tmp = imgt.at<uchar>(x, y);
			float temper = g_temper[0].at<float>(y, x);
			sprintf(label, "温度:%.2f)", temper);
			sprintf(label2, "坐标:(%d, %d)", x, y);	//获取坐标点位置
			rectangle(imgt, cvPoint(x, y - 12), cvPoint(x + 180, y + 20),
				CV_RGB(0, 0, 0), CV_FILLED, 8, 0);
		/*	putText(imgt, label, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.2, Scalar(255, 23, 0), 1, 8);
			putText(imgt, label2, Point(x, y + 20), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 23, 0), 1, 8);*/

			CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); //指定字体
			cv::Scalar size{ 12, 0, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }
			text.setFont(nullptr, &size, nullptr, 0);
			text.putText(imgt, label, Point(x, y), Scalar(255, 23, 0));
			text.putText(imgt, label2, Point(x, y + 20), Scalar(255, 23, 0));

			//CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); //指定字体
			//cv::Scalar size1{ 100, 0.5, 0.1, 0 }, size2{ 100, 0, 0.1, 0 }, size3{ 50, 0, 1, 0 }, size4{ 50, 0, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }

			//text.setFont(nullptr, &size1, nullptr, 0);
			//text.putText(imgt, "中国", cv::Point(50, 100));

			//text.setFont(nullptr, &size2, nullptr, 0);
			//text.putText(imgt, "北京", cv::Point(50, 200), cv::Scalar(255, 0, 0));

			//text.setFont(nullptr, &size3, nullptr, 0);
			//text.putText(imgt, "China", cv::Point(50, 250), cv::Scalar(0, 255, 0));

			//text.setFont(nullptr, &size4, nullptr, 0);
			//text.putText(imgt, "BeiJing", cv::Point(50, 300), cv::Scalar(0, 0, 255));



			QImage image = QImage((const unsigned char*)(imgt.data), imgt.cols, imgt.rows, QImage::Format_RGB888);
			ui.label->setPixmap(QPixmap::fromImage(image));

		}

		return true;
	}
	else
	{
		return false;
	}

	//if (qobject_cast<QLabel*>(obj) == ui.label&&event->type() == QEvent::MouseButtonPress)
	//{
	//	QMouseEvent* mevent = (QMouseEvent*)event;

	//	if (mevent->button() == Qt::LeftButton)
	//	{
	//		//	QMessageBox::information(NULL, "Filter", "left click"); 
	//		//setMouseState(MouseState::L_C, 0);

	//			int w=ui.label->pixmap()->width();
	//			int h = ui.label->pixmap()->height();

	//			m_ratio +=0.1;
	//			if (m_ratio > 3) m_ratio = 3;

	//			Mat timg;

	//			cv::resize(g_Img[0], timg, Size(w * m_ratio, h * m_ratio), 0, 0);

	//			int m_mx = w * m_ratio / 2 - w/2;
	//			int m_my = h * m_ratio / 2 - h/2;
	//			Rect rect1(m_mx, m_my, w, h);
	//			//	Rect rect2(224, 224, 128, 128);

	//			Mat roi1;
	//			timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	//			QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
	//			//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

	//			ui.label->setPixmap(QPixmap::fromImage(image));

	//
	//			timg.release();
	//			roi1.release();

	//	}
	//	// 如果是鼠标右键按下
	//	else if (mevent->button() == Qt::RightButton)
	//	{
	//		//QMessageBox::information(NULL, "Filter", "r click");
	//		//setMouseState(MouseState::R_C, 0);


	//		int w = ui.label->pixmap()->width();
	//		int h = ui.label->pixmap()->height();

	//		m_ratio -= 0.1;
	//		if (m_ratio < 1) m_ratio = 1;
	//		Mat timg;

	//		cv::resize(g_Img[0], timg, Size(w * m_ratio, h * m_ratio), 0, 0);

	//		int m_mx = w * m_ratio / 2 - w / 2;
	//		int m_my = h * m_ratio / 2 - h / 2;
	//		Rect rect1(m_mx, m_my, w, h);
	//		//	Rect rect2(224, 224, 128, 128);

	//		Mat roi1;
	//		timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	//		QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
	//		//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

	//		ui.label->setPixmap(QPixmap::fromImage(image));


	//		timg.release();
	//		roi1.release();
	//	}
	//	
	//	return true;
	//}
	//else 
	//{
	//	return false;
	//}
}



//void QtTestCam::on_OpenFig_clicked()
//{
//	QString filename;
//	filename = QFileDialog::getOpenFileName(this,tr("选择图像"),	"",	tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
//	if (filename.isEmpty())
//	{
//		return;
//	}
//	else
//	{
//		string str = filename.toStdString();  // 将filename转变为string类型；
//		image = imread(str);
//		//image=imread(fileName.toLatin1().data);
//		cvtColor(image, image, CV_BGR2RGB);
//		cv::resize(image, image,cv:: Size(300, 200));
//		QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB888);
//
//		label = new QLabel();
//		label->setPixmap(QPixmap::fromImage(img));
//		label->resize(QSize(img.width(), img.height()));
//		ui.scrollArea->setWidget(label);
//
//	}
//}
//

//void QtTestCam::ReadFrame()
//{
//	//获取图像帧
//	capture >> frame;
//	/*
//	//将抓取到的帧,转换为QImage格式,QImage::Format_RGB888使用24位RGB格式（8-8-8）存储图像
//	//此时没有使用rgbSwapped()交换所有像素的红色和蓝色分量的值，底色偏蓝
//	QImage image = QImage((const uchar*)frame.data,frame.cols, frame.rows,QImage::Format_RGB888);
//	//将图片显示到label上
//	ui->label->setPixmap(QPixmap::fromImage(image));
//	*/
//	//将视频显示到label上
//	QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();
//	ui.label->setPixmap(QPixmap::fromImage(image));
//}

//打开摄像头
void QtTestCam::OpenCameraClicked()
{
	//QString filename;
	//filename = QFileDialog::getOpenFileName(this,tr("选择图像"),	"",	tr("videos (*.avi)"));
	//if (filename.isEmpty())
	//{
	//	return;
	//}
	//else
	//{
	//	string str = filename.toStdString();  // 将filename转变为string类型；
	//	capture.open(str);//打开摄像头
	//	timer->start(25);//开启定时器，一次25ms

	//}
	IRSDK_InqureIP(&strIPAddr, 100);
	QString str = QString(QLatin1String(strIPAddr.IPAddr));
	g_IP = str;

	if (str.isEmpty())
	{
		QMessageBox::information(this, tr("Tips"), tr("Please Connect the Camera First!!!"), tr("OK"));
		return;
	}

	qint32 i32Port;
	ir_ip = str;
	QStringList strlist = ir_ip.split(".");
	i32Port = strlist.at(3).toInt() * 10 + 30005;

	T_IPADDR IpInfo;
	IpInfo.DataPort = i32Port;

	strcpy(IpInfo.IPAddr, ir_ip.toLocal8Bit().data());

	long ret = IRSDK_Create(0, IpInfo, (CBF_IR)FrameProc, NULL, NULL, (void *)(&ui));
	//IRSDK_DeviceCfg(0, 1);
	IRSDK_Connect(0); //连接，设备根据这个包解析主机的端口，IP，MAC 信息

//	int re=IRSDK_Play(0);
//	IRSDK_Connect(0);
}


//关闭摄像头
void QtTestCam::CloseCameraClicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this, tr("选择数据"), "", tr("Data (*.dat)"));
	if (filename.isEmpty())
	{
		return;
	}
	char*  path;
	QByteArray t = filename.toLatin1(); // must
	path = t.data();

	Mat img;
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

	unsigned short *tmp = (unsigned short*)malloc(IMAGE_HEIGHT*IMAGE_WIDTH * sizeof(short));

	dataRead(tmp, path, IMAGE_HEIGHT* IMAGE_WIDTH);

	g_pData[g_picNum] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];
	memcpy(g_pData[g_picNum], tmp, IMAGE_HEIGHT*IMAGE_WIDTH*sizeof(short));

	data2Img(g_pData[g_picNum], img, IMAGE_HEIGHT, IMAGE_WIDTH,16, 2, 2,g_referTemper);

	img.copyTo(g_Img[g_picNum]);
	imshow("r", g_Img[g_picNum]);
	
	g_temper[g_picNum].create(IMAGE_WIDTH ,IMAGE_HEIGHT, CV_32FC1);

	data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH,100);
	

	Mat tshow(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC1);
	int	bottomvalue = 99999;
	int	topvalue = -99999;
	
	for (int i = 0; i < g_temper[g_picNum].rows; i++)
	{
		float *p_tData = g_temper[g_picNum].ptr<float>(i);
		for (int j = 0; j < g_temper[g_picNum].cols; j++)
		{
			float value = *(p_tData + j);
			//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
			bottomvalue = bottomvalue<value ? bottomvalue : value;
			topvalue = topvalue>value ? topvalue : value;
		}

	}
	float range = topvalue - bottomvalue;
	int displayValue;

	for (int i = 0; i < g_temper[g_picNum].rows; i++)
	{
		uchar *p_tTData =tshow.ptr<uchar>(i);
		float *p_tData = g_temper[g_picNum].ptr<float>(i);
		for (int j = 0; j < g_temper[g_picNum].cols; j++)
		{
			float value = *(p_tData + j);
			//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
			displayValue = ((value - bottomvalue) * 255) / range;

			*(p_tTData + j) = displayValue;
		}

	}




	imshow("rt", tshow);

	Mat g_dstImage3;
	img.copyTo(g_dstImage3);

	int thickness = 2;
	int lineType = 8;
	cv::line(g_dstImage3, Point(g_dstImage3.cols / 2, 0),
		Point(g_dstImage3.cols / 2, g_dstImage3.rows - 1),
		Scalar(255, 255, 255),
		thickness,
		lineType);
	int step = g_dstImage3.rows / 4;

	for (int i = 0; i<4; i++)
	{
		cv::line(g_dstImage3,
			Point(g_dstImage3.cols / 2, 0 + i*step),
			Point(0, g_dstImage3.cols / 2 * 1.73 + i*step),
			Scalar(255, 255, 255),
			thickness,
			lineType);

		cv::line(g_dstImage3,
			Point(g_dstImage3.cols / 2, 0 + i*step),
			Point(g_dstImage3.cols - 1, g_dstImage3.cols / 2 * 1.73 + i*step),
			Scalar(255, 255, 255),
			thickness,
			lineType);
	}


//	imshow("view", g_dstImage3);


	//float tm = g_temper[0].at<float>(0, 0);

	//QString b = QString("%1").arg(tm);

	//QMessageBox::information(NULL, "Title",b );

	QImage image = QImage((const unsigned char*)(g_dstImage3.data), img.cols, img.rows, QImage::Format_RGB888);
	//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

	ui.label->setPixmap(QPixmap::fromImage(image));

	QLabel *p = ui.widget_2->findChild<QLabel*>(QString::number(g_picNum));


	QImage newImg = image.scaled(p->width()-2, p->height()-2);

	p->setPixmap(QPixmap::fromImage(newImg));


	p->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");
	g_picNum=g_picNum%12+1;


}

void QtTestCam::closeEvent(QCloseEvent* event)
{
//	QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	IRSDK_Stop(0);
	IRSDK_Destroy(0);
	IRSDK_Quit();
}

void QtTestCam::changeLabel(int totalNum, int imagePerRow)//调整显示窗口数
{
	for (int i = 0; i < totalNum; i++)
	{
		QLabel *p = ui.widget_2->findChild<QLabel*>(QString::number(i));
		if (p != NULL) delete p;
	}

	int rows = (totalNum - 1) / imagePerRow + 1;
	int count = 0;
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < imagePerRow; y++)
		{
			QLabel *lb = new QLabel;
			lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			//lb->setFixedSize(240, 320);
			lb->setText(QString::number(x * imagePerRow + y));
			lb->setObjectName(QString::number(x * imagePerRow + y));
			lb->setFrameShape(QFrame::Box);
			ui.gridLayout_2->addWidget(lb, x, y);
			lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
			lb->setAlignment(Qt::AlignCenter);
			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

	count = 0;
	for (int x = 0; x <rows; x++)
	{
		for (int y = 0; y < imagePerRow; y++)
		{
			QPushButton *bt = new QPushButton;
			//lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

			bt->setText(QString::number(x *imagePerRow + y));
			ui.gridLayout_2->addWidget(bt, x, y, Qt::AlignRight | Qt::AlignTop);
			bt->setMinimumSize(32, 32);
			bt->setMaximumSize(32, 32);
			bt->setObjectName("bt" + QString::number(x * imagePerRow + y));
			bt->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255)"));
			connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));

			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

}

void QtTestCam::changeLabel()
{
	qDeleteAll(ui.widget_2->findChildren<QLabel*>());

	int g_picTotalNum = 12;

	int rows = (g_picTotalNum - 1) / 3 + 1;
	/*int hei = ui.widget_2->height() / rows - 10;
	int wid = hei * 3 / 4;*/

	int count = 0;
	for (int x = 0; x < (g_picTotalNum - 1) / 3 + 1; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			MyLabel *lb = new MyLabel;
			lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	/*		lb->setFixedSize(wid, hei);*/
			lb->setText(QString::number(x * 3 + y + 1));
			lb->setObjectName(QString::number(x * 3 + y));
			lb->setFrameShape(QFrame::Box);
			ui.gridshow->addWidget(lb, x, y);
			lb->setStyleSheet("border-width: 4px;border-style: solid;border-color: rgb(255, 170, 0);");
			lb->setAlignment(Qt::AlignCenter);
		//	lb->setMaximumSize(480, 640);
			//lb->setBaseSize(240, 320);
			//lb->setMinimumSize(wid, hei);
			connect(lb, SIGNAL(calData()), this, SLOT(testSingal()));
			//lb->show();
			count++;
			if (count >= g_picTotalNum) break;
		}
		if (count >= g_picTotalNum) break;
	}
}

void QtTestCam::testSingal()
{
	MyLabel *optBtn = qobject_cast<MyLabel *>(sender());
	QString name = sender()->objectName();//发送信号者的对象名

	QMessageBox::information(this, "size", name );

	/*MyStruct ts = data.value<MyStruct>();


	QMessageBox::information(this, "size", QString::number(ts.ma) + "," + QString::number(ts.mi) + "," + QString::number(ts.mean));*/

}


void QtTestCam::btn_testClicked()
{
//	MyLabel *p = ui.widget_2->findChild<MyLabel *>("0");
//
//	QMessageBox::information(this, "size", QString::number(p->width()) + QString::number(p->height()));
//	
//	changeLabel();
//
////	QRect  rc = ui.gridshow->cellRect(0, 0);
//
//
//
//	MyLabel *p1 = ui.widget_2->findChild<MyLabel *>("0");
//
//	QMessageBox::information(this, "size", QString::number(p1->width()) + QString::number(p1->height()));

	

	//QString m_msg;
	//std::vector<string> vecFiles;
	//QDir *dir = new QDir(g_tempFolder);
	//QStringList filter;
	//filter << "*.dat";
	//dir->setNameFilters(filter);
	//QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));

	//for (int i = 0; i < fileInfo->size(); i++)
	//{
	//	vecFiles.push_back(fileInfo->at(i).filePath().toStdString());
	//}


	//ui.label->setShapeType(2);


	//QPainter painter(ui.label);
	//QImage wb_Image;
	//wb_Image.load("E:/lena_color.bmp");
	//if (wb_Image.size().width() <= 0) return;
	/////将图像按比例缩放成和窗口一样大小
	//QImage img = wb_Image.scaled(QSize(ui.label->width(), ui.label->height()), Qt::IgnoreAspectRatio);
	//int x = this->width() - img.width();
	//int y = this->height() - img.height();
	//painter.drawImage(QPoint(x, y), img);

	//// 画矩形框
	//QPainter paint;
	//paint.begin(this);

	//paint.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
	//paint.drawRect(QRect(50, 50, 200, 200));

	//QString strText = QString::fromLocal8Bit("用双手成就你的梦想");;
	//paint.drawText(250, 50, strText);

	//paint.end();

	//Mat imgt;
	//g_Img[0].copyTo(imgt);


	//char label[1000];
	//char label2[1000];

	//CvFont font, font2;
	//	
	//	int x = 50;
	//	int y =70;

	//	if (x < 0) x = 0;
	//	if (x >= imgt.cols) x = imgt.cols - 1;
	//	if (y < 0) y = 0;
	//	if (y >= imgt.rows) y = imgt.rows - 1;

	//	int tmp = imgt.at<uchar>(x, y);
	//	float temper = g_temper[0].at<float>(x, y);
	//	sprintf(label, "Temperature:%.2f)", temper);
	//	sprintf(label2, "Pixel:(%d, %d)", x, y);	//获取坐标点位置
	//	rectangle(imgt, cvPoint(x, y - 12), cvPoint(x + 180, y + 20),
	//		CV_RGB(0, 0, 0), CV_FILLED, 8, 0);
	//	putText(imgt, label, Point(x, y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 23, 0), 4, 8);
	//	putText(imgt, label2, Point(x, y + 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 23, 0), 4, 8);
	//
	//
	//	QImage image = QImage((const unsigned char*)(imgt.data), imgt.cols, imgt.rows, QImage::Format_RGB888);
	//	ui.label->setPixmap(QPixmap::fromImage(image));
	//changeLabel(10, 5);
	//g_referTemper = g_referTemper + 1;

	//Mat img;
	//img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

	//data2Img(g_pData[g_picNum], img, IMAGE_HEIGHT, IMAGE_WIDTH, 30, 16, 2, 2,g_referTemper);

	//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_RGB888);

	//ui.label->setPixmap(QPixmap::fromImage(image));

	//QLabel *p = ui.widget_2->findChild<QLabel*>(QString::number(g_picNum));


	//QImage newImg = image.scaled(p->width(), p->height());

	//p->setPixmap(QPixmap::fromImage(newImg));
	//dlg = new QtSetDlg;

	//dlg->show();
	

}


void QtTestCam::wheelEvent(QWheelEvent*event)
{
	//QRect tmp = this->geometry();//获取窗口的位置以及大小并保存在tmp中。
	//if (event->delta()>0)
	//{//如果滚轮往上滚
	//	tmp.setWidth(tmp.width() + 25);//设置宽度为原有基础上+25
	//	tmp.setHeight(tmp.height() + 15);//设置窗口高度为原有基础上+20
	//	this->setGeometry(tmp);//然后设置窗口大小。

	//}
	//else{//同样的
	//	tmp.setWidth(tmp.width() - 25);
	//	tmp.setHeight(tmp.height() - 15);
	//	//如果缩小之后的窗口不小于设置的最小窗口尺寸，则执行缩放。
	//	if (this->minimumSize().height()<tmp.height() && this->minimumSize().width()<tmp.width())
	//		this->setGeometry(tmp);
	//}

	g_referTemper += 1.0*event->delta()/1200;

	Mat img;
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

	data2Img(g_pData[g_picNum-1], img, IMAGE_HEIGHT, IMAGE_WIDTH,  16, 2, 2, g_referTemper);

	QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_RGB888);

	ui.label->setPixmap(QPixmap::fromImage(image));

}


void QtTestCam::paintEvent(QPaintEvent *)
{
	QPainter painter(ui.label);
	QImage wb_Image;
	wb_Image.load("E:/lena_color.bmp");
	if (wb_Image.size().width() <= 0) return;
	///将图像按比例缩放成和窗口一样大小
	QImage img = wb_Image.scaled(QSize(ui.label->width(), ui.label->height()), Qt::IgnoreAspectRatio);
	int x = this->width() - img.width();
	int y = this->height() - img.height();
	painter.drawImage(QPoint(x, y), img);

	// 画矩形框
	QPainter paint;
	paint.begin(this);

	paint.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
	paint.drawRect(QRect(50, 50, 200, 200));

	QString strText = QString::fromLocal8Bit("用双手成就你的梦想");;
	paint.drawText(250, 50, strText);

	paint.end();

	//QPainter painter(this);
	//painter.drawLine(80, 100, 650, 500);
	//painter.setPen(Qt::red);
	//painter.drawRect(10, 10, 100, 400);
	//painter.setPen(QPen(Qt::green, 5));
	//painter.setBrush(Qt::blue);
	//painter.drawEllipse(50, 150, 400, 200);

//	QPainter paint(this);
//	QImage image;
//	image.load("E:/lena_color.bmp");
////	qDebug() << image.size() << image.format() << image.depth();
//	paint.drawImage(QPoint(200, 200), image);
//	paint.setPen(QPen(Qt::blue, 5));
//	for (int i = 0; i<4; i++)
//		paint.drawPoint(X[i], Y[i]);
//	paint.setPen(Qt::red);
//	QPolygon pts;
//	pts.setPoints(4, X[0], Y[0], X[1], Y[1], X[2], Y[2], X[3], Y[3]);
//	paint.drawConvexPolygon(pts);

}


void QtTestCam::magicTime()
{
	QPainter painter(ui.label);
	//painter.setPen(Qt::gray);
	//painter.setBrush(Qt::green);
	//painter.drawRect(10, 10, 200, 200);
	painter.drawLine(80, 100, 650, 500);
	painter.setPen(Qt::red);
	painter.drawRect(10, 10, 100, 400);
	painter.setPen(QPen(Qt::green, 5));
	painter.setBrush(Qt::blue);
	painter.drawEllipse(50, 150, 400, 200);
}

void QtTestCam::mouseReleaseEvent(QMouseEvent *event)
{
	m_dragging = 0;
}

//void QtTestCam::mouseMoveEvent(QMouseEvent *event)
//{
//	if (m_dragging == 1)
//	{
//		X[j] = event->pos().x();
//		Y[j] = event->pos().y();
//		update();
//	}
//}


void QtTestCam::mousePressEvent(QMouseEvent *event)
{//单击
	// 如果是鼠标左键按下
	//if (event->button() == Qt::LeftButton){
	////	QMessageBox::information(NULL, "Title", "left click"); 
	//	//setMouseState(MouseState::L_C, 0);
	//	int mouse_x = QCursor::pos().x();//鼠标点击处横坐标
	//	int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标
	//	QWidget *action = QApplication::widgetAt(mouse_x, mouse_y);//获取鼠标点击处的控件

	//	//QMessageBox::information(NULL, "Title", action->objectName());

	//	QLabel *tempLabel = this->findChild<QLabel *>(action->objectName());

	//	int w = tempLabel->pixmap()->width();
	//	int h = tempLabel->pixmap()->height();

	//	m_ratio += 0.1;
	//	if (m_ratio > 3) m_ratio = 3;

	//	Mat timg;

	//	cv::resize(g_Img[0], timg, Size(w * m_ratio, h * m_ratio), 0, 0);

	//	int m_mx = w * m_ratio / 2 - w / 2;
	//	int m_my = h * m_ratio / 2 - h / 2;
	//	Rect rect1(m_mx, m_my, w, h);
	//	//	Rect rect2(224, 224, 128, 128);

	//	Mat roi1;
	//	timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	//	QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
	//	//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

	//	tempLabel->setPixmap(QPixmap::fromImage(image));


	//	timg.release();
	//	roi1.release();
	//}
	//// 如果是鼠标右键按下
	//else if (event->button() == Qt::RightButton){
	////	QMessageBox::information(NULL, "Title", "r click"); 
	//	//setMouseState(MouseState::R_C, 0);
	//}
	//else if (event->button() == Qt::MidButton){
	////	QMessageBox::information(NULL, "Title", "m click");
	//	//setMouseState(MouseState::M_C, 0);
	//}


	for (int i = 0; i<4; i++)
	{
		if (event->pos().x() == X[i] && event->pos().y() == Y[i])
		{
			m_dragging = 1;
			j = i;
			break;
		}
		else continue;
	}


}


void QtTestCam::testMouseMoved()
{
	//ui.label->setText(QString("坐标：（%1,%2）").arg(QCursor::pos().x()).arg(QCursor::pos().y()));
	
}


void QtTestCam::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);

	//changeLabel();


}



