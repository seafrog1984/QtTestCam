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
IplImage *img0, *img1;
Mat temp1;

//#include "IRSDK.h"
#include <imgProcDll.h>

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
	int shape_type; //0���Σ�1Բ��
	int lt_x, lt_y;//���Ͻ�����
	int rb_x, rb_y;//���½�����
	double t_max;
	double t_min;
	double t_aver;
	double t_msd;
	string comment;//ע��
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
			////��ʾ����
			////��ȡTTF�����ļ�
			CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); // "zenhei.ttf"Ϊ���峣��
			//CvxText text(""); // "zenhei.ttf"Ϊ���峣��
			//	const char *msg = "��OpenCV��������֣�";
			float p = 1;
			//CvScalar fsize;
			//////������������ �����С/�հױ���/�������/��ת�Ƕ�
			//fsize = cvScalar(20, 0.5, 0.1, 0);
			text.setMySize(10, 10);
			text.setFont(NULL, NULL, NULL, &p);
			//	rectangle(imgt, pre_pt, cur_pt, Scalar(255, 255, 255), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���
			//	putText(imgt, msg, Point(cur_pt.x,cur_pt.y+15), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255));

			text.putText(img1, temp, Point(allshape[i].rb_x, allshape[i].rb_y + 15), CV_RGB(0, 0, 0));
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
				rectangle(img, Point(allshape[i].lt_x, allshape[i].lt_y), Point(allshape[i].rb_x, allshape[i].rb_y), Scalar(255, 255, 255), 1, 8, 0);//���ݳ�ʼ��ͽ����㣬�����λ���img��
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
	connect(ui.btn_play, SIGNAL(clicked()), this, SLOT(OpenCameraClicked()));//������ͷ
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(CloseCameraClicked()));//�ر�����ͷ
	connect(ui.btn_test, SIGNAL(clicked()), this, SLOT(btn_testClicked()));//���Զϲ�

	ui.label->setBaseSize(QSize(100, 800));

	ui.label->installEventFilter(this);


	int g_picTotalNum = 12;
	int count = 0;
	for (int x = 0; x < (g_picTotalNum - 1) / 3+1; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			QLabel *lb = new QLabel;
			lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			//lb->setFixedSize(240, 320);
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

	std::string scanID = "test20190521";
	QDir dir;
	g_tempFolder = dir.currentPath() + "//Temp//" + QString::fromStdString(scanID);

	// ���Ŀ¼�Ƿ���ڣ������������½�

	if (!dir.exists(g_tempFolder))
	{
		bool res = dir.mkpath(g_tempFolder);
//		qDebug() << "�½�Ŀ¼�Ƿ�ɹ�" << res;

	}


}

bool QtTestCam::eventFilter(QObject *obj, QEvent *event)
{
	//if (qobject_cast<QLabel*>(obj) == ui.label&&event->type() == QEvent::Paint)
	//{
	//	magicTime();
	//}

	if (qobject_cast<QLabel*>(obj) == ui.label&&event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mevent = (QMouseEvent*)event;

		if (mevent->button() == Qt::LeftButton)
		{
			//	QMessageBox::information(NULL, "Filter", "left click"); 
			//setMouseState(MouseState::L_C, 0);

				int w=ui.label->pixmap()->width();
				int h = ui.label->pixmap()->height();

				m_ratio +=0.1;
				if (m_ratio > 3) m_ratio = 3;

				Mat timg;

				cv::resize(g_Img[0], timg, Size(w * m_ratio, h * m_ratio), 0, 0);

				int m_mx = w * m_ratio / 2 - w/2;
				int m_my = h * m_ratio / 2 - h/2;
				Rect rect1(m_mx, m_my, w, h);
				//	Rect rect2(224, 224, 128, 128);

				Mat roi1;
				timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

				QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
				//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

				ui.label->setPixmap(QPixmap::fromImage(image));

	
				timg.release();
				roi1.release();

		}
		// ���������Ҽ�����
		else if (mevent->button() == Qt::RightButton)
		{
			//QMessageBox::information(NULL, "Filter", "r click");
			//setMouseState(MouseState::R_C, 0);


			int w = ui.label->pixmap()->width();
			int h = ui.label->pixmap()->height();

			m_ratio -= 0.1;
			if (m_ratio < 1) m_ratio = 1;
			Mat timg;

			cv::resize(g_Img[0], timg, Size(w * m_ratio, h * m_ratio), 0, 0);

			int m_mx = w * m_ratio / 2 - w / 2;
			int m_my = h * m_ratio / 2 - h / 2;
			Rect rect1(m_mx, m_my, w, h);
			//	Rect rect2(224, 224, 128, 128);

			Mat roi1;
			timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

			QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
			//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

			ui.label->setPixmap(QPixmap::fromImage(image));


			timg.release();
			roi1.release();
		}
		
		return true;
	}
	else 
	{
		return false;
	}
}



//void QtTestCam::on_OpenFig_clicked()
//{
//	QString filename;
//	filename = QFileDialog::getOpenFileName(this,tr("ѡ��ͼ��"),	"",	tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
//	if (filename.isEmpty())
//	{
//		return;
//	}
//	else
//	{
//		string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
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
//	//��ȡͼ��֡
//	capture >> frame;
//	/*
//	//��ץȡ����֡,ת��ΪQImage��ʽ,QImage::Format_RGB888ʹ��24λRGB��ʽ��8-8-8���洢ͼ��
//	//��ʱû��ʹ��rgbSwapped()�����������صĺ�ɫ����ɫ������ֵ����ɫƫ��
//	QImage image = QImage((const uchar*)frame.data,frame.cols, frame.rows,QImage::Format_RGB888);
//	//��ͼƬ��ʾ��label��
//	ui->label->setPixmap(QPixmap::fromImage(image));
//	*/
//	//����Ƶ��ʾ��label��
//	QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();
//	ui.label->setPixmap(QPixmap::fromImage(image));
//}

//������ͷ
void QtTestCam::OpenCameraClicked()
{
	//QString filename;
	//filename = QFileDialog::getOpenFileName(this,tr("ѡ��ͼ��"),	"",	tr("videos (*.avi)"));
	//if (filename.isEmpty())
	//{
	//	return;
	//}
	//else
	//{
	//	string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
	//	capture.open(str);//������ͷ
	//	timer->start(25);//������ʱ����һ��25ms

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
	IRSDK_Connect(0); //���ӣ��豸������������������Ķ˿ڣ�IP��MAC ��Ϣ

//	int re=IRSDK_Play(0);
//	IRSDK_Connect(0);
}


//�ر�����ͷ
void QtTestCam::CloseCameraClicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this, tr("ѡ������"), "", tr("Data (*.dat)"));
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
	
	g_temper[g_picNum].create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_32FC1);

	data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH,100);


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


	imshow("view", g_dstImage3);


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

void QtTestCam::changeLabel(int totalNum, int imagePerRow)//������ʾ������
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
void QtTestCam::btn_testClicked()
{
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
	dlg = new QtSetDlg;

	dlg->show();
	

}


void QtTestCam::wheelEvent(QWheelEvent*event)
{
	//QRect tmp = this->geometry();//��ȡ���ڵ�λ���Լ���С��������tmp�С�
	//if (event->delta()>0)
	//{//����������Ϲ�
	//	tmp.setWidth(tmp.width() + 25);//���ÿ��Ϊԭ�л�����+25
	//	tmp.setHeight(tmp.height() + 15);//���ô��ڸ߶�Ϊԭ�л�����+20
	//	this->setGeometry(tmp);//Ȼ�����ô��ڴ�С��

	//}
	//else{//ͬ����
	//	tmp.setWidth(tmp.width() - 25);
	//	tmp.setHeight(tmp.height() - 15);
	//	//�����С֮��Ĵ��ڲ�С�����õ���С���ڳߴ磬��ִ�����š�
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
	//QPainter painter(this);
	//painter.drawLine(80, 100, 650, 500);
	//painter.setPen(Qt::red);
	//painter.drawRect(10, 10, 100, 400);
	//painter.setPen(QPen(Qt::green, 5));
	//painter.setBrush(Qt::blue);
	//painter.drawEllipse(50, 150, 400, 200);

	QPainter paint(this);
	QImage image;
	image.load("E:/lena_color.bmp");
//	qDebug() << image.size() << image.format() << image.depth();
	paint.drawImage(QPoint(200, 200), image);
	paint.setPen(QPen(Qt::blue, 5));
	for (int i = 0; i<4; i++)
		paint.drawPoint(X[i], Y[i]);
	paint.setPen(Qt::red);
	QPolygon pts;
	pts.setPoints(4, X[0], Y[0], X[1], Y[1], X[2], Y[2], X[3], Y[3]);
	paint.drawConvexPolygon(pts);

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

void QtTestCam::mouseMoveEvent(QMouseEvent *event)
{
	if (m_dragging == 1)
	{
		X[j] = event->pos().x();
		Y[j] = event->pos().y();
		update();
	}
}


void QtTestCam::mousePressEvent(QMouseEvent *event)
{//����
	// ���������������
	//if (event->button() == Qt::LeftButton){
	////	QMessageBox::information(NULL, "Title", "left click"); 
	//	//setMouseState(MouseState::L_C, 0);
	//	int mouse_x = QCursor::pos().x();//�������������
	//	int mouse_y = QCursor::pos().y();//�������������
	//	QWidget *action = QApplication::widgetAt(mouse_x, mouse_y);//��ȡ��������Ŀؼ�

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
	//// ���������Ҽ�����
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




