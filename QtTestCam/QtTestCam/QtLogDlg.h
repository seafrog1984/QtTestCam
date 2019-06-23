#pragma once

#include <QWidget>
#include "ui_QtLogDlg.h"

class QtLogDlg : public QWidget
{
	Q_OBJECT

public:
	QtLogDlg(QWidget *parent = Q_NULLPTR);
	~QtLogDlg();

private:
	Ui::QtLogDlg ui;

private slots:
	void log();

};
