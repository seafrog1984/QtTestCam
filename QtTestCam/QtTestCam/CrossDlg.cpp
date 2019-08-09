#include "CrossDlg.h"
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMouseEvent>
#include <QPixmap>

using namespace cv;

extern Mat g_dst;
Mat dst_show;
Mat pic;
extern int g_row;
extern int g_col;
cv::Mat imageRow = cv::Mat::zeros(256, 240, CV_8UC3);
cv::Mat imageCol = cv::Mat::zeros(320, 256, CV_8UC3);
extern int g_color;

CrossDlg::CrossDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btn_cross, SIGNAL(clicked()), this, SLOT(btn_cross()));

	connect(ui.label, SIGNAL(calHist()), this, SLOT(updateHist()));

	Mat dst = imread("E:\\lena_color.bmp");

	cv::resize(dst, dst_show, Size(240, 320));

	cvtColor(dst_show, g_dst, CV_RGB2GRAY);

	cvtColor(dst_show, dst_show, CV_BGR2RGB);

	QImage image = QImage((const unsigned char*)(dst_show.data), dst_show.cols, dst_show.rows, QImage::Format_RGB888);

	QPixmap pixmap = QPixmap::fromImage(image);
//	QPixmap fitpixmap = pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	ui.label->setPixmap(pixmap);

	ui.label_2->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");
	ui.label_3->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");
}

CrossDlg::~CrossDlg()
{
}

void CrossDlg::updateHist()
{
	imshow("row", imageRow);
	QImage image_row = QImage((const unsigned char*)(imageRow.data), imageRow.cols, imageRow.rows, QImage::Format_RGB888);

	QPixmap pixmap_row = QPixmap::fromImage(image_row);
	//QPixmap fitpixmap = pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	ui.label_3->setPixmap(pixmap_row);


	imshow("col", imageCol);
	QImage image_col = QImage((const unsigned char*)(imageCol.data), imageCol.cols, imageCol.rows, QImage::Format_RGB888);

	QPixmap pixmap_col = QPixmap::fromImage(image_col);
	//QPixmap fitpixmap_col = pixmap_col.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	ui.label_2->setPixmap(pixmap_col);
}


void CrossDlg::btn_cross()
{

	g_color = (g_color + 1) % 4;
//	Mat pic1;
//	dst_show.copyTo(pic1);
//
//	line(pic1, Point(0, g_row), Point(240, g_row), Scalar(255, 0, 0), 1, CV_AA);
//	line(pic1, Point(g_col, 0), Point(g_col, 320), Scalar(255, 0, 0), 1, CV_AA);
//
//	QImage image_gray = QImage((const unsigned char*)(pic1.data), pic1.cols, pic1.rows, QImage::Format_RGB888);
//
//	QPixmap pixmap = QPixmap::fromImage(image_gray);
//	//	QPixmap fitpixmap = pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
//	ui.label->setPixmap(pixmap);
//
//
//	cv::Mat imageRow = cv::Mat::zeros(256, 240, CV_8UC3);
//	imageRow.setTo(cv::Scalar(0, 0, 0));
//	std::vector<cv::Point> points_row;
//	int nRowLength = g_dst.cols;
//
//	for (int x = 0; x < nRowLength; x++)
//	{
//		int y = g_dst.at<uchar>(g_row, x);
//		points_row.push_back(cv::Point(x, y));
//	}
//	cv::polylines(imageRow, points_row, false, cv::Scalar(255, 0, 0), 1, 8, 0);
//
//	cv::imshow("row", imageRow);
////	cvtColor(imageRow, imageRow, CV_BGR2RGB);
//
//	QImage image_row = QImage((const unsigned char*)(imageRow.data), imageRow.cols, imageRow.rows, QImage::Format_RGB888);
//
//	QPixmap pixmap_row = QPixmap::fromImage(image_row);
//	//QPixmap fitpixmap = pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
//	ui.label_3->setPixmap(pixmap_row);
//
//
//	cv::Mat imageCol = cv::Mat::zeros(320, 256, CV_8UC3);
//	imageRow.setTo(cv::Scalar(0, 0, 0));
//	std::vector<cv::Point> points_col;
//	int nColLength = g_dst.rows;
//	for (int y = 0; y < nColLength; y++)
//	{
//		int x = g_dst.at<uchar>(y, g_col);
//		points_col.push_back(cv::Point(x, y));
//	}
//	cv::polylines(imageCol, points_col, false, cv::Scalar(255, 0, 0), 1, 8, 0);
//
//	cv::imshow("col", imageCol);
////	cvtColor(imageCol, imageCol, CV_BGR2RGB);
//
//	QImage image_col = QImage((const unsigned char*)(imageCol.data), imageCol.cols, imageCol.rows, QImage::Format_RGB888);
//
//	QPixmap pixmap_col = QPixmap::fromImage(image_col);
//	//QPixmap fitpixmap_col = pixmap_col.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
//	ui.label_2->setPixmap(pixmap_col);



}


//void CrossDlg:: mouseMoveEvent(QMouseEvent *ev)
//{
//	Mat pic1;
//	pic.copyTo(pic1);
//	g_col = ev->pos().x();;
//	g_row = ev->pos().y();
//	line(pic1, Point(0, g_row), Point(240, g_row), Scalar(255, 255, 255), 1, CV_AA);
//	line(pic1, Point(g_col, 0), Point(g_col, 320), Scalar(255, 255, 255), 1, CV_AA);
//
//	imshow("img", pic1);
//		cv::Mat imageRow = cv::Mat::zeros(256, 240, CV_8UC3);
//		imageRow.setTo(cv::Scalar(100, 0, 0));
//		std::vector<cv::Point> points_row;
//		int nRowLength = g_dst.cols;
//
//		for (int x = 0; x < nRowLength; x++)
//		{
//			int y = g_dst.at<uchar>(g_row, x);
//			points_row.push_back(cv::Point(x, y));
//		}
//		cv::polylines(imageRow, points_row, false, cv::Scalar(255, 255, 255), 1, 8, 0);
//
//		//cv::imshow("row", imageRow);
//		QImage image_row = QImage((const unsigned char*)(imageRow.data), imageRow.cols, imageRow.rows, QImage::Format_Grayscale8);
//
//		QPixmap pixmap = QPixmap::fromImage(image_row);
//	//	QPixmap fitpixmap = pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
//		ui.label_3->setPixmap(pixmap);
//
//		cv::Mat imageCol = cv::Mat::zeros(320, 256, CV_8UC3);
//		imageRow.setTo(cv::Scalar(100, 0, 0));
//		std::vector<cv::Point> points_col;
//		int nColLength = g_dst.rows;
//		for (int y = 0; y < nColLength; y++)
//		{
//			int x = g_dst.at<uchar>(y, g_col);
//			points_col.push_back(cv::Point(x, y));
//		}
//		cv::polylines(imageCol, points_col, false, cv::Scalar(255, 255, 255), 1, 8, 0);
//
//		//cv::imshow("col", imageCol);
//
//		QImage image_col = QImage((const unsigned char*)(imageCol.data), imageCol.cols, imageCol.rows, QImage::Format_Grayscale8);
//
//		QPixmap pixmap_col = QPixmap::fromImage(image_col);
//		//QPixmap fitpixmap_col = pixmap_col.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
//		ui.label_2->setPixmap(pixmap_col);
//}