#include<QLabel>
#include<QPainter>
#include<QMouseEvent>
#include <QKeyEvent>
#include <QLine>
#include"MYLABEL.h"
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace cv;

Mat img;
Mat g_dst;
int g_row, g_col;
extern 	cv::Mat imageRow;
extern 	cv::Mat imageCol;
QPoint p[4] = { QPoint(0, 0) };

extern int g_color = 0;


MyLabel::MyLabel( QWidget* parent)
:QLabel(parent)
{
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
	//connect(this, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
	p1 = QPoint(10, 20);
	p2 = QPoint(0, 0);//QPoint构造函数
	m_shapeType = 1;
	this->setAlignment(Qt::AlignCenter);

	img = imread("E://lena_color.bmp");
	cvtColor(img, img, CV_BGR2RGB);
	cv::resize(img, img, Size(240, 320));
	cvtColor(img, g_dst, CV_RGB2GRAY);
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{

	/*lines.append(QLine(p1, p2));

	p2 = ev->pos();
	int radius = abs(p2.x() - p1.x()) < abs(p2.y() - p1.y()) ? abs(p2.x() - p1.x()) : abs(p2.y() - p1.y());

	circle(img, Point(p1.x() + radius / 2, p1.y() + radius / 2), radius / 2, Scalar(255, 255, 255), 1, 8, 0);*/
//	emit clicked(ev);
}

void MyLabel::mousePressEvent(QMouseEvent *ev)
{
	//emit pressed(ev);
	//update();
	p1 = ev->pos();

	if (ev->button() == Qt::LeftButton)
	{
		setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");

	}
	// 如果是鼠标右键按下
	else if (ev->button() == Qt::RightButton)
	{

		//MyStruct mstruct;
		//mstruct.ma = 100;
		//mstruct.mi = 0;
		//mstruct.mean = 50;

		//QVariant data=QVariant::fromValue(mstruct);
		emit calData();
	/*	QMessageBox::information(this, "size", QString::number(width()) + QString::number(height()));
		setShapeType(2);*/
		//int nRet = QMessageBox::question(NULL, "询问", "是否继续？", QMessageBox::Yes, QMessageBox::No);
		//// 选择是
		//if (QMessageBox::Yes == nRet)
		//{
		//	QMessageBox::information(NULL, "Title", "Yes");
		//}

		//// 选择否
		//if (QMessageBox::No == nRet) 
		//{
		//	QMessageBox::information(NULL, "Title", "No");
		//}
	}


}

void MyLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	//emit doubled(ev);
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
	//p2 = ev->pos();

	p1 = ev->pos();
	if (p1.x() < 0) p1.setX(0);
	if (p1.x()>239) p1.setX(239);
	if (p1.y() < 0) p1.setY(0);
	if (p1.y()>319) p1.setY(319);

	p[g_color] = p1;



	cv::Scalar pen_color;
	imageRow.setTo(cv::Scalar(0, 0, 0));
	imageCol.setTo(cv::Scalar(0, 0, 0));

	for (int i = 0; i<4; i++)
	{
		g_row = p[i].y();
		g_col = p[i].x();
		if (g_row != 0 || g_col != 0)
		{
			if (i == 0)
				pen_color = cv::Scalar(255, 0, 0);
			else if (i == 1)
				pen_color = cv::Scalar(0, 0, 255);
			else if (i == 2)
				pen_color = cv::Scalar(0, 255, 0);
			else if (i == 3)
				pen_color = cv::Scalar(255, 255, 0);

			std::vector<cv::Point> points_row;
			int nRowLength = g_dst.cols;

			for (int x = 0; x < nRowLength; x++)
			{
				int y = g_dst.at<uchar>(g_row, x);
				points_row.push_back(cv::Point(x, y));
			}
			cv::polylines(imageRow, points_row, false, pen_color, 1, 8, 0);


			std::vector<cv::Point> points_col;
			int nColLength = g_dst.rows;
			for (int y = 0; y < nColLength; y++)
			{
				int x = g_dst.at<uchar>(y, g_col);
				points_col.push_back(cv::Point(x, y));
			}
			cv::polylines(imageCol, points_col, false, pen_color, 1, 8, 0);
		}
	
	}

	

	emit calHist();
	//cv::imshow("col", imageCol);
	////	cvtColor(imageCol, imageCol, CV_BGR2RGB);

	//QImage image_col = QImage((const unsigned char*)(imageCol.data), imageCol.cols, imageCol.rows, QImage::Format_RGB888);

	//QPixmap pixmap_col = QPixmap::fromImage(image_col);
	////QPixmap fitpixmap_col = pixmap_col.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	//ui.label_2->setPixmap(pixmap_col);

	//update();//刷新界面

	//this->setText(QString("坐标：（%1,%2）").arg(QCursor::pos().x()).arg(QCursor::pos().y()));
	//emit moved(ev);
}

void MyLabel::keyPressEvent(QKeyEvent *ev)
{

}

void MyLabel::paintEvent(QPaintEvent *ev)
{
	QLabel::paintEvent(ev);
	//Mat pic1;
	//img.copyTo(pic1);
	//line(pic1, Point(0, p1.y()), Point(240, p1.y()), Scalar(255, 0, 0), 1, CV_AA);
	//line(pic1, Point(p1.x(), 0), Point(p1.x(), 320), Scalar(255, 0, 0), 1, CV_AA);


	QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_RGB888);
	//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

	this->setPixmap(QPixmap::fromImage(image));
	//if (this->pixmap() == NULL) return;
	//int w = this->pixmap()->width();
	//int h = this->pixmap()->height();

	//int lw =this->width();
	//int lh = this->height();

	//int x = p2.x() - (lw - w) / 2;
	//int y = p2.y() - (lh - h) / 2;

	////int x = 50;
	////int y = 60;
	//if (x < 0) x = 0;
	//if (x >= w) x = w - 1;
	//if (y < 0) y = 0;
	//if (y >= h) y = h - 1;

	//// 画矩形框
	QPainter paint(this);
	//paint.begin(this);

	QPoint pt;
	for (int i = 0;i<4;i++)
	{
		pt = p[i];
		if (i==0)
			paint.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
		if (i == 1)
			paint.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap));
		if (i == 2)
			paint.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap));
		if (i == 3)
			paint.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap));

		paint.drawLine(0, pt.y(), 240, pt.y());
		paint.drawLine(pt.x(), 0, pt.x(), 320);
	}

	QString strText = QString::number(p1.x()) + "," + QString::number(p1.y());
	paint.drawText(10,10, strText);


	//int radius = abs(p2.x() - p1.x()) < abs(p2.y() - p1.y()) ? abs(p2.x() - p1.x()) : abs(p2.y() - p1.y());

	//paint.drawEllipse(QRect(p1.x()<p2.x() ? p1.x() : p2.x(), p1.y()<p2.y() ? p1.y() : p2.y(), radius, radius));




	//QString strText = QString::number(x )+ "," + QString::number(y);
	//paint.drawText(10, 10, strText);
	//if (m_shapeType == 1)
	//{	QLine line(p1, p2);

	//paint.drawLines(lines);
	//paint.drawLine(line);
	//}
	//else if (m_shapeType == 2)
	//{
	//	QRect rect(p1.x(), p1.y(), p2.x() - p1.x(), p2.y() - p1.y());
	//	paint.drawRect(rect);
	//}
	//paint.end();
}

void MyLabel::setShapeType(int shapeType)
{
	m_shapeType = shapeType;

}