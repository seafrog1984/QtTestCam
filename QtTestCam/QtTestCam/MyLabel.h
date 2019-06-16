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
//	void clicked(QMouseEvent *ev);  //̧��
//	void pressed(QMouseEvent *ev);  //����
//	void doubled(QMouseEvent *ev);  //˫��
//	void moved(QMouseEvent *ev);    //�϶�
//	//void sig_GetOneFrame(QImage img);

protected:

	void mouseReleaseEvent(QMouseEvent *ev);  //̧��
	void mousePressEvent(QMouseEvent *ev);    //����
	void mouseDoubleClickEvent(QMouseEvent *ev);  //˫��
	void mouseMoveEvent(QMouseEvent *ev);     //�϶�
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
