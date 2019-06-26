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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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
		m_msg = QString::fromLocal8Bit("���ӳɹ�");

		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("����ʧ��\n��ȷ��IP��˿ں�");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	//2-
	std::string sPermissions;
	// ע�⣺ 0��ʾ���Կͻ��ˣ� 1��ʾ��ʽ�ͻ���
	int iRet = m_cli.login_auth(m_user.toStdString().c_str(), m_passwd.toStdString().c_str(), sPermissions, iTestFlag);
	if (0 > iRet)
	{
		m_msg = QString::fromLocal8Bit("��ȡ��Ȩ��ʧ��\n");
		m_msg.append(m_cli.get_msg().c_str());
		m_cli.close();
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("��ȡ��Ȩ��ɹ�\n");
		QString sAuth = QString::fromStdString(m_cli.get_auth().c_str());
		QString sRep1 = sAuth.mid(8, 16);
		QString sRep2('*', sRep1.length());
		sAuth.replace(sRep1, sRep2);
		if (0 == iRet)
		{
			m_msg.append(QString::fromLocal8Bit("���Ի���: "));

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

		//	m_msg.Append("\nȨ��: ");
		//	m_msg.Append("ͼ��ɨ��");
		if (pt.p1 || pt.p3)
		{
			g_log_flag = 1;

		}
		else
		{
			m_msg.append(QString::fromLocal8Bit("û��Ȩ��"));
			QMessageBox::information(NULL, "Title", m_msg);
		}

	}

	//QString sScanID = "SCAN001";

	//map<std::string, std::string> mapUserInfoResp;
	//int ret = m_cli.get_info(sScanID.toStdString().c_str(), mapUserInfoResp);
	//if (-1 == ret)
	//{
	//	m_msg = QString::fromLocal8Bit("��ȡ�û���Ϣʧ��\n");
	//	m_msg.append(m_cli.get_msg().c_str());
	//	m_cli.close();
	//	//mb_conn.EnableWindow(TRUE);
	//}
	//else if (0 == ret)
	//{
	//	m_msg = QString::fromLocal8Bit("��ȡ�û���ϢΪ��");
	//}
	//else
	//{
	//	//m_msg = "��ȡ�û���Ϣ�ɹ�";
	//	//	MessageBox(m_msg);
	//	std::map<std::string, std::string>::iterator it = mapUserInfoResp.begin();
	//	//for (; it != mapUserInfoResp.end(); ++it)
	//	//{
	//	//m_msg = "��ȡ ";
	//	//m_msg.Append(it->first.c_str());
	//	//m_msg.Append(_T("\n"));
	//	//m_msg.Append(it->second.c_str());
	//	//	MessageBox(m_msg);
	//	//}
	//	it = mapUserInfoResp.begin();
	//	CString rectmp;
	//	rectmp = it->second.c_str();
	//	m_age = atoi(rectmp);//����
	//	it++;
	//	rectmp = rectmp = it->second.c_str();
	//	m_ID = rectmp;//֤����
	//	it++;
	//	rectmp = rectmp = it->second.c_str();
	//	m_name = rectmp;//����
	//	it++;
	//	rectmp = rectmp = it->second.c_str();//ͼ��
	//	it++;
	//	rectmp = rectmp = it->second.c_str();
	//	m_NO = rectmp; //���
	//	it++;

	//	rectmp = rectmp = it->second.c_str();
	//	if (rectmp == "0")
	//		m_sex = "��";//�Ա�
	//	else
	//		m_sex = "Ů";


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