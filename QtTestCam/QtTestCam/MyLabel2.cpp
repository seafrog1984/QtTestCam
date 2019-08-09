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
	// ���������Ҽ�����
	else if (ev->button() == Qt::RightButton)
	{
		int nRet = QMessageBox::question(NULL, "ѯ��", "�Ƿ������", QMessageBox::Yes, QMessageBox::No);
		// ѡ����
		if (QMessageBox::Yes == nRet)
		{
			QMessageBox::information(NULL, "Title", "Yes");
		}

		// ѡ���
		if (QMessageBox::No == nRet)
		{
			QMessageBox::information(NULL, "Title", "No");
		}
	}


}