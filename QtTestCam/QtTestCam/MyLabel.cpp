#include<QLabel>
#include<QPainter>
#include<QMouseEvent>
#include <QKeyEvent>
#include <QLine>
#include"MYLABEL.h"


MyLabel::MyLabel( QWidget* parent)
:QLabel(parent)
{
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
	//connect(this, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
	p1 = QPoint(0, 0);
	p2 = QPoint(0, 0);//QPoint构造函数
	m_shapeType = 1;
	this->setAlignment(Qt::AlignCenter);


	//wb_Image.load("E:/lena_color.bmp");
	//this->setPixmap(QPixmap::fromImage(wb_Image));

}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{

	lines.append(QLine(p1, p2));
//	emit clicked(ev);
}

void MyLabel::mousePressEvent(QMouseEvent *ev)
{
	//emit pressed(ev);
	//update();
	p1 = ev->pos();

}

void MyLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	//emit doubled(ev);
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
	p2 = ev->pos();


	update();//刷新界面

	//this->setText(QString("坐标：（%1,%2）").arg(QCursor::pos().x()).arg(QCursor::pos().y()));
	//emit moved(ev);
}

void MyLabel::keyPressEvent(QKeyEvent *ev)
{

}

void MyLabel::paintEvent(QPaintEvent *ev)
{
	QLabel::paintEvent(ev);

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
	QPainter paint;
	paint.begin(this);

	paint.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
	paint.drawRect(QRect(p1.x(), p1.y(), p2.x()-p1.x(), p2.y()-p1.y()));

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


	paint.end();
}

void MyLabel::setShapeType(int shapeType)
{
	m_shapeType = shapeType;
}