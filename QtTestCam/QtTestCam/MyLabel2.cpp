#include "MyLabel2.h"

#include<QMouseEvent>
#include <QKeyEvent>


MyLabel2::MyLabel2(QWidget* parent)
:QLabel(parent)
{
}

MyLabel2::~MyLabel2()
{
}


void MyLabel2::mousePressEvent(QMouseEvent *ev)
{
	//emit pressed(ev);
	//update();
	//p1 = ev->pos();

	if (ev->button() == Qt::LeftButton)
	{
		setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");
	}
	// 如果是鼠标右键按下
	else if (ev->button() == Qt::RightButton)
	{
		int nRet = QMessageBox::question(NULL, "询问", "是否继续？", QMessageBox::Yes, QMessageBox::No);
		// 选择是
		if (QMessageBox::Yes == nRet)
		{
			QMessageBox::information(NULL, "Title", "Yes");
		}

		// 选择否
		if (QMessageBox::No == nRet)
		{
			QMessageBox::information(NULL, "Title", "No");
		}
	}


}