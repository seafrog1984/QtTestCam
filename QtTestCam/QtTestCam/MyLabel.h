#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>
#include<QString>
#include<QWidget>
#include<QMainWindow>
#include<QLine>
#include"QMessageBox"

class MyLabel :public QLabel
{
	Q_OBJECT
public:
	MyLabel( QWidget *parent = 0);
	~MyLabel(){}

	void setShapeType(int shapeType);
//signals:
//
//	void clicked(QMouseEvent *ev);  //抬起
//	void pressed(QMouseEvent *ev);  //按下
//	void doubled(QMouseEvent *ev);  //双击
//	void moved(QMouseEvent *ev);    //拖动
//	//void sig_GetOneFrame(QImage img);

protected:

	void mouseReleaseEvent(QMouseEvent *ev);  //抬起
	void mousePressEvent(QMouseEvent *ev);    //按下
	void mouseDoubleClickEvent(QMouseEvent *ev);  //双击
	void mouseMoveEvent(QMouseEvent *ev);     //拖动
	void paintEvent(QPaintEvent *ev);
	void keyPressEvent(QKeyEvent *ev);

	int m_shapeType;
	QPoint p1, p2;
	QVector <QLine> lines;
	//private slots :
	//void  slotGetOneFrame(QImage img);

	QImage wb_Image;
};

#endif // MYLABEL_H
