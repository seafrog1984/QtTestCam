#pragma once

#include <QWidget>
#include "ui_QtSetDlg.h"
#include "IRSDK.h"

class QtSetDlg : public QWidget
{
	Q_OBJECT

public:
	QtSetDlg(QWidget *parent = Q_NULLPTR);
	~QtSetDlg();

private:
	Ui::QtSetDlg ui;

private slots:
	void setIP();

};
