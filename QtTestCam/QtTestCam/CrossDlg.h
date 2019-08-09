#pragma once

#include <QWidget>
#include "ui_CrossDlg.h"
#include "MyLabel.h"

class CrossDlg : public QWidget
{
	Q_OBJECT

public:
	CrossDlg(QWidget *parent = Q_NULLPTR);
	~CrossDlg();

//	void mouseMoveEvent(QMouseEvent *ev);     //ÍÏ¶¯

private:
	Ui::CrossDlg ui;

public slots:
	void btn_cross();
	void updateHist();

};
