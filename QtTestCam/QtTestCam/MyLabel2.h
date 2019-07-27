#ifndef MYLABEL2_H
#define MYLABEL2_H
#include<QLabel>
#include<QString>
#include<QWidget>
#include<QMainWindow>
#include<QLine>
#include"QMessageBox"

#include <QLabel>

class MyLabel2 : public QLabel
{
	Q_OBJECT

public:
	MyLabel2(QWidget *parent = 0);
	~MyLabel2();

	void mousePressEvent(QMouseEvent *ev);    //°´ÏÂ

};
#endif // MYLABEL_H