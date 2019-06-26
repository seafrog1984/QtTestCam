#include "QtLogDlg.h"
#include "QtTestCam.h"
#include <fstream>
#include <iostream> 
#include "./incl/client.h"

QString m_ip;
QString m_port ;
QString g_uport;

int g_control_port;
int g_card_port;

QString m_user = "test";
QString m_passwd = "test@1234";

int iTestFlag = 0;
client_t m_cli;
int g_log_flag = 0;
QString m_msg;

using namespace std;




QtLogDlg::QtLogDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.btn_log, SIGNAL(clicked()), this, SLOT(log()));

	ifstream fin("config.ini");

	if (fin.fail())
	{
		QMessageBox::information(NULL, "Title","No file!");
		exit(-1);
	}

	string str1, str2, str3;

	int i_control_port, i_card_port;

	fin >> str1 >> str2 >> str3 >> i_control_port >> i_card_port;
	 m_ip = QString::fromStdString(str1);
	 m_port = QString::fromStdString(str2);
	 g_uport = QString::fromStdString(str3);;

	 g_control_port = i_control_port;
	 g_card_port = i_card_port;

	fin.close();

	// TODO:  在此添加额外的初始化

	//m_ip = "10.70.77.81";
	//m_ip="192.168.31.157";

	m_user = "test";
	m_passwd = "test@1234";

	ui.lineEdit_user->setText(m_user);
	ui.lineEdit_pw->setText(m_passwd);

	iTestFlag = 1;




}

QtLogDlg::~QtLogDlg()
{
}

void QtLogDlg::log()
{

	if (m_cli.init(m_ip.toStdString(), atoi(m_port.toStdString().c_str())))
	{
		m_msg = QString::fromLocal8Bit("连接成功");

		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("连接失败\n请确认IP或端口号");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	//2-
	std::string sPermissions;
	// 注意： 0表示测试客户端， 1表示正式客户端
	int iRet = m_cli.login_auth(m_user.toStdString().c_str(), m_passwd.toStdString().c_str(), sPermissions, iTestFlag);
	if (0 > iRet)
	{
		m_msg = QString::fromLocal8Bit("获取授权码失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		m_cli.close();
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("获取授权码成功\n");
		QString sAuth = QString::fromStdString(m_cli.get_auth().c_str());
		QString sRep1 = sAuth.mid(8, 16);
		QString sRep2('*', sRep1.length());
		sAuth.replace(sRep1, sRep2);
		if (0 == iRet)
		{
			m_msg.append(QString::fromLocal8Bit("测试环境: "));

		}
		m_msg.append(sAuth);

		int iPermissions = atoi(sPermissions.c_str());
		permissions_t pt;
		if (iPermissions & 0x01)
		{
			pt.p1 = true;
		}
		if (iPermissions & 0x02)
		{
			pt.p2 = true;
		}
		if (iPermissions & 0x04)
		{
			pt.p3 = true;
		}

		//	m_msg.Append("\n权限: ");
		//	m_msg.Append("图像扫描");
		if (pt.p1 || pt.p3)
		{
			g_log_flag = 1;

		}
		else
		{
			m_msg.append(QString::fromLocal8Bit("没有权限"));
			QMessageBox::information(NULL, "Title", m_msg);
		}

	}

	//QString sScanID = "SCAN001";

	//map<std::string, std::string> mapUserInfoResp;
	//int ret = m_cli.get_info(sScanID.toStdString().c_str(), mapUserInfoResp);
	//if (-1 == ret)
	//{
	//	m_msg = QString::fromLocal8Bit("获取用户信息失败\n");
	//	m_msg.append(m_cli.get_msg().c_str());
	//	m_cli.close();
	//	//mb_conn.EnableWindow(TRUE);
	//}
	//else if (0 == ret)
	//{
	//	m_msg = QString::fromLocal8Bit("获取用户信息为空");
	//}
	//else
	//{
	//	//m_msg = "获取用户信息成功";
	//	//	MessageBox(m_msg);
	//	std::map<std::string, std::string>::iterator it = mapUserInfoResp.begin();
	//	//for (; it != mapUserInfoResp.end(); ++it)
	//	//{
	//	//m_msg = "获取 ";
	//	//m_msg.Append(it->first.c_str());
	//	//m_msg.Append(_T("\n"));
	//	//m_msg.Append(it->second.c_str());
	//	//	MessageBox(m_msg);
	//	//}
	//	it = mapUserInfoResp.begin();
	//	CString rectmp;
	//	rectmp = it->second.c_str();
	//	m_age = atoi(rectmp);//年龄
	//	it++;
	//	rectmp = rectmp = it->second.c_str();
	//	m_ID = rectmp;//证件号
	//	it++;
	//	rectmp = rectmp = it->second.c_str();
	//	m_name = rectmp;//姓名
	//	it++;
	//	rectmp = rectmp = it->second.c_str();//图像
	//	it++;
	//	rectmp = rectmp = it->second.c_str();
	//	m_NO = rectmp; //编号
	//	it++;

	//	rectmp = rectmp = it->second.c_str();
	//	if (rectmp == "0")
	//		m_sex = "男";//性别
	//	else
	//		m_sex = "女";


	//	UpdateData(FALSE);

	//	if (mapUserInfoResp.end() != mapUserInfoResp.find("pic"))
	//	{
	//		vecPngIDResp.clear();
	//		int size = split_vec(mapUserInfoResp["pic"].c_str(), vecPngIDResp, ',');
	//	}
	//	return;
	//}
	//MessageBox(m_msg);






	QtTestCam *w=new QtTestCam;
	w->show();

	this->setHidden(true);
}