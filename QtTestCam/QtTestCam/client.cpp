#include <stdafx.h>
#include "./utils/pc/pc_info.h"
#include "./utils/md5/md5.h"
#include "./incl/client.h"

#include <ctime>


bool _client_t::init(const std::string &host, short port)
{
	if (!sock.Connect(host.c_str(), port))
	{
		char buf[128] = { 0 };
		sprintf_s(buf, "connect %s:%d fail", host.c_str(), port);
		sMsg = buf;
		return false;
	}

	bConnection = true;
	return true;
}


void _client_t::set_req_head(req_head_t &head, const int &cmd)
{
	head.version = 10001;
	head.cmd = cmd;
	time_t now_time = time(NULL);
	head.timestamp = (int)now_time;
}


bool _client_t::recv_data(char *buf, int buf_len)
{
	int recv_len = sock.Read(buf, buf_len);
	if (0 >= recv_len)
		return false;

	int tmp_len = 0;
	while (recv_len < buf_len)
	{
		tmp_len = sock.Read(&buf[recv_len], buf_len - recv_len);
		if (0 >= tmp_len)
			return false;

		recv_len += tmp_len;
	}
	return true;
}


int _client_t::recv_resp(char *buf)
{
	char resp[MAX_DATA] = { 0 };
	if (!recv_data(resp, sizeof(resp_head_t)))
	{
		sMsg = "get resp head fail 1";
		return -1;
	}
	//����ͷ��
	resp_head_t *resp_head = new resp_head_t();
	memcpy(resp_head, resp, sizeof(resp_head_t));
	if (10000000 < resp_head->length)
	{
		sMsg = "get resp head fail 2";
		return -1;
	}
	//printf("anth head: %d, %d, %d\n", 
	//	resp_head->length, resp_head->result, resp_head->timestamp);

	//char tmp[26] = {0};
	//sprintf(tmp, "%d-%d-%d", resp_head->result, resp_head->length, resp_head->timestamp);
	//sprintf(tmp, "%d-%d-%d", resp_head->length, data_len, recv);
	//sMsg = tmp;
	//return -1;

	if (0 == resp_head->length)
		return 2;

	if (!recv_data(buf, resp_head->length))
	{
		sMsg = "get resp data fail";
		return -1;
	}

	if (0 != resp_head->result)
		return 0;
	else
		return 1;
}


std::string _client_t::get_local_auth(std::string &mac, std::string &ip, std::string &device)
{
	int len = device.length();

	char re[2] = { 0 };
	re[0] = mac[0];
	device.replace(0, 1, re);
	re[0] = mac[4];
	device.replace(4, 1, re);
	int pos1 = len - 1;
	re[0] = mac[mac.length() - 1];
	device.replace(pos1, 1, re);
	pos1 = len - 4;
	re[0] = mac[mac.length() - 4];
	device.replace(pos1, 1, re);
	re[0] = ip[ip.length() - 1];
	device.insert(0, re);
	re[0] = ip[0];
	device.append(re);
	//58eb053ee0bd03425dc91f1d612df7ba

	return MD5(device).toString();
}


int _client_t::login_auth(string user, string passwd, std::string &permissions, int flag)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	//��ȡ��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_GET_AUTH);

	//����ͻ��˱�ʶ
	//mac+ip�ļ��ܴ�(��ϣ|md5|�����㷨)
	std::string sMac = GetMAC();
	std::string sIP = GetIP();
	std::string sDevice = sMac + "&" + sIP;
	sDevice = bytesToHexstring(sDevice.c_str(), sDevice.length());
	int len = sDevice.length();

	string sUser = user;
	int len2 = sUser.length();
	string sPasswd = passwd;
	int len3 = sPasswd.length();

	//��ȡ��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 4 * sizeof(int)+len + len2 + len3 + 1;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sDevice.c_str(), len);
	pos += len;
	//�û���
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sUser.c_str(), len2);
	pos += len2;
	//����
	memcpy(&data[pos], &len3, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sPasswd.c_str(), len3);
	pos += len3;
	//0��ʾ���ԣ�1��ʾ��ʽ
	string sFlag = (flag == 0) ? "0" : "1";
	len = 1;
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sFlag.c_str(), len);
	pos += len;

	//�������δ��Ȩ�򷵻ش�����Ϣ
	//�����������Ȩ�򷵻���Ȩ��Ϣ����Ȩ����ܺ󱣴浽����
	//��Ȩ��Ϊ���ݿͻ��˱�ʶ���ݼ��ܵõ��ģ���ͻ��˱�ʶһһ��Ӧ
	if (!sock.Send(data, pos))
	{
		sMsg = "send auth msg fail";
		return -1;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return -1;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);

	std::string msg(&resp[pos], data_len);
	pos += data_len;
	//printf("auth body: %d, %s\n", data_len, msg.c_str());

	if (0 == ret)
	{
		sMsg = "auth fail, " + msg;
		return -1;
	}

	if ("test case" == msg)
	{
		sMsg = msg;

		memcpy(&data_len, &resp[pos], sizeof(int));
		pos += sizeof(int);
		std::string msg1(&resp[pos], data_len);
		if (msg1 != get_local_auth(sMac, sIP, sDevice))
		{
			sMsg = "auth fail";
			return -1;
		}
		sAuth = msg1;
		return 0;
	}

	if (msg != get_local_auth(sMac, sIP, sDevice))
	{
		sMsg = "auth fail";
		return -1;
	}
	sAuth = msg;

	memcpy(&data_len, &resp[pos], sizeof(int));
	pos += sizeof(int);
	std::string msg2(&resp[pos], data_len);
	permissions = msg2;

	return 1;
}


int _client_t::check_auth(int flag)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return -1;
	}

	//��֤��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_CHECK_AUTH);

	//����ͻ��˱�ʶ
	std::string sMac = GetMAC();
	std::string sDevice = bytesToHexstring(sMac.c_str(), sMac.length());
	int len = sDevice.length();
	//��֤��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 3 * sizeof(int)+len + sAuth.length() + 1;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sDevice.c_str(), len);
	pos += len;
	len = sAuth.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sAuth.c_str(), len);
	pos += len;
	//0��ʾ���ԣ�1��ʾ��ʽ
	string sFlag = (flag == 0) ? "0" : "1";
	len = 1;
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sFlag.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "send check msg fail";
		return -1;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "check auth fail, " + msg;
		return -1;
	}

	if ("test case" == msg)
	{
		sMsg = msg;
		return 0;
	}
	return 1;
}


int _client_t::set_scanid(const std::string &card_id, std::string &scan_id, const std::string &user)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return -1;
	}

	if ("" == scan_id || "" == card_id)
	{
		sMsg = "card_id or scan_id is blank";
		return -1;
	}

	//��֤��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_SET_SCANID);

	//��֤��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 4 * sizeof(int)+card_id.length() + scan_id.length() + user.length();
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//card
	int len = card_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], card_id.c_str(), len);
	pos += len;
	//scan
	len = scan_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len);
	pos += len;
	//user
	len = user.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], user.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "set scanid msg fail";
		return -1;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return -1;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "set scanid fail, " + msg;
		return -1;
	}
	return 1;
}


int _client_t::get_scanid(const std::string &card_id, std::string &params, std::map <std::string, std::string> &mapScanID)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return -1;
	}

	if ("" == card_id || "" == params)
	{
		sMsg = "card_id or params is blank";
		return -1;
	}

	//��֤��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_GET_SCANID);

	//��֤��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 3 * sizeof(int)+card_id.length() + params.length();
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//card
	int len = card_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], card_id.c_str(), len);
	pos += len;
	//scan
	len = params.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], params.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "get scanid msg fail";
		return -1;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return -1;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "get scanid fail, " + msg;
		return -1;
	}

	if ("ok" == msg)
	{
		return 0;
	}

	split_map(msg.c_str(), mapScanID, '=', '&');
	return 1;
}


int _client_t::set_cardid(std::map<std::string, std::string> &mapCardInfo, const std::string &user)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return -1;
	}

	//������Ϣ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_SET_CARDID);

	std::string sData = map_join(mapCardInfo, '&', '=');
	int len1 = sData.length();
	int len2 = user.length();
	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 2 * sizeof(int)+len1 + len2;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//data
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sData.c_str(), len1);
	pos += len1;
	//user
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], user.c_str(), len2);
	pos += len2;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "set cardid msg fail";
		return -1;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return -1;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "cardid send fail, " + msg;
		return -1;
	}
	return 1;
}


bool _client_t::send_png(const std::string &scan_id, unsigned short *png, int png_len, std::vector<std::string> &vecPngID)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return false;
	}

	//ͼƬ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_SEND_PNG);


	//std::string sPng = bytesToHexstring(png, png_len);
	//png_len *= sizeof(short);

	int len1 = scan_id.length();
	int len2 = 1;
	int len3 = png_len * sizeof(short);


	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 3 * sizeof(int)+len1 + len2 + len3;
	//�������ݿ���
	char data[MAX_DATA + PIC_SIZE * 2] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len1);
	pos += len1;
	//type
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], "0", 1);
	pos += 1;
	//data
	memcpy(&data[pos], &png_len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], png, len3);
	pos += len3;

	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "send png msg fail";
		return false;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	//pos += data_len;

	if (0 == ret)
	{
		sMsg = "png send fail, " + msg;
		return false;
	}
	vecPngID.push_back(msg);

	return true;
}


bool _client_t::send_info(std::map<std::string, std::string> &mapUserInfo)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return false;
	}

	//������Ϣ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_SEND_DATA);

	std::string sData = map_join(mapUserInfo, '&', '=');
	int len = sData.length();
	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = sizeof(int)+len;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sData.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "send user msg fail";
		return false;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "user info send fail, " + msg;
		return false;
	}
	return true;
}


int _client_t::get_info(const std::string &scan_id, std::map<std::string, std::string> &mapUserInfo)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return -1;
	}

	//������Ϣ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_GET_DATA);

	int len = scan_id.length();
	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = sizeof(int)+len;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "get user msg fail";
		return -1;
	}
	char resp[MAX_DATA] = { 0 };

	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return -1;
	}
	if (2 == ret)
	{
		return 0;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "get info send fail, " + msg;
		return -1;
	}
	split_map(msg.c_str(), mapUserInfo, '=', '&');

	return 1;
}

bool _client_t::get_png(const std::string &scan_id, const std::string &png_id, unsigned short *pic, int pic_len)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return false;
	}

	//ͼƬ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_GET_PNG);

	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 2 * sizeof(int)+scan_id.length() + png_id.length();
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	int len = scan_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len);
	pos += len;
	//data
	len = png_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], png_id.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "get png msg fail";
		return false;
	}

	char resp[MAX_DATA + PIC_SIZE * 2] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);

	if (0 == ret)
	{
		//std::string msg(&resp[pos], data_len);
		sMsg = "png get fail";
		return false;
	}

	int cnt = data_len / sizeof(unsigned short);
	if (cnt > pic_len)
		cnt = pic_len;
	for (int i = 0; i<cnt; ++i)
	{
		pic[i] = *((unsigned short*)(&resp[pos] + i * sizeof(unsigned short)));
	}

	return true;
}


bool _client_t::send_result_dat(const std::string &scan_id, unsigned short *png, int png_len, const std::string &info, const std::string &user)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return false;
	}

	//ͼƬ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_SEND_RESULT);

	int len1 = scan_id.length();
	int len2 = user.length();
	int len3 = png_len * sizeof(short);
	int len4 = info.length();
	int len5 = 1;
	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 5 * sizeof(int)+len1 + len2 + len3 + len4 + len5;
	//�������ݿ���
	char data[MAX_DATA + PIC_SIZE * 2] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len1);
	pos += len1;
	//user
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], user.c_str(), len2);
	pos += len2;
	//type
	memcpy(&data[pos], &len5, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], "0", 1);
	pos += 1;
	//data
	memcpy(&data[pos], &png_len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], png, len3);
	pos += len3;
	//info
	memcpy(&data[pos], &len4, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], info.c_str(), len4);
	pos += len4;

	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "send result png msg fail";
		return false;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "result png send fail, " + msg;
		return false;
	}
	return true;
}


bool _client_t::send_result_bmp(const std::string &scan_id, std::string &png, const std::string &info, const std::string &user)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return false;
	}

	//ͼƬ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_SEND_RESULT);

	int len1 = scan_id.length();
	int len2 = user.length();
	int len3 = png.length();
	int len4 = info.length();
	int len5 = 1;
	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 5 * sizeof(int)+len1 + len2 + len3 + len4 + len5;
	//�������ݿ���
	char *data = new char[MAX_DATA + len3];
	memset(data, 0, MAX_DATA + len3);
	//char data[MAX_DATA+PIC_SIZE*2] = {0};
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len1);
	pos += len1;
	//user
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], user.c_str(), len2);
	pos += len2;
	//type
	memcpy(&data[pos], &len5, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], "1", 1);
	pos += 1;
	//data
	memcpy(&data[pos], &len3, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], png.c_str(), len3);
	pos += len3;

	//std::ofstream ofs("1.txt", std::ios::out|std::ios::binary);
	//ofs.write(png.c_str(), png.length());
	//ofs.close();

	//info
	memcpy(&data[pos], &len4, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], info.c_str(), len4);
	pos += len4;

	//�������δ��Ȩ�򷵻ش�����Ϣ
	bool bRet = sock.Send(data, pos);
	delete[] data;
	data = NULL;
	if (!bRet)
	{
		sMsg = "send result png msg fail";
		return false;
	}
	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "result png send fail, " + msg;
		return false;
	}
	return true;
}


//��ȡ�û��б�
bool _client_t::get_users(string &user, string &passwd, std::map<std::string, std::string> &mapUserInfo, int &iLevel)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	//��ȡ��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CONTROL_GET_LIST);

	string sUser = user;
	int len1 = sUser.length();
	string sPasswd = passwd;
	int len2 = sPasswd.length();

	//��ȡ��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 2 * sizeof(int)+len1 + len2;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//�û���
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sUser.c_str(), len1);
	pos += len1;
	//����
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sPasswd.c_str(), len2);
	pos += len2;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	//�����������Ȩ�򷵻���Ȩ��Ϣ����Ȩ����ܺ󱣴浽����
	//��Ȩ��Ϊ���ݿͻ��˱�ʶ���ݼ��ܵõ��ģ���ͻ��˱�ʶһһ��Ӧ
	if (!sock.Send(data, pos))
	{
		sMsg = "send get userlist msg fail";
		return false;
	}

	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "get userlist fail, " + msg;
		return false;
	}

	split_map(msg.c_str(), mapUserInfo, '=', '&');

	memcpy(&data_len, &resp[pos], sizeof(int));
	pos += sizeof(int);
	std::string level(&resp[pos], data_len);
	iLevel = atoi(level.c_str());

	return true;
}

//�����û�
bool _client_t::new_user(string &user, string &passwd, int &level, int &permissions, string &loginuser, string &loginpasswd)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	//��ȡ��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CONTROL_NEW_USER);

	string sUser = user;
	int len1 = sUser.length();
	string sPasswd = passwd;
	int len2 = sPasswd.length();
	string sTmp;
	sTmp=std::to_string(level);
	string sLevel = sTmp;
	int len3 = sLevel.length();
	sTmp=std::to_string(permissions);
	string sPermissions = sTmp;
	int len4 = sPermissions.length();
	string sLoginUser = loginuser;
	int len5 = sLoginUser.length();
	string sLoginPasswd = loginpasswd;
	int len6 = sLoginPasswd.length();
	//��ȡ��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 6 * sizeof(int)+len1 + len2 + len3 + len4 + len5 + len6;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//�û���
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sUser.c_str(), len1);
	pos += len1;
	//����
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sPasswd.c_str(), len2);
	pos += len2;
	//����
	memcpy(&data[pos], &len3, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLevel.c_str(), len3);
	pos += len3;
	memcpy(&data[pos], &len4, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sPermissions.c_str(), len4);
	pos += len4;
	//��¼
	memcpy(&data[pos], &len5, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLoginUser.c_str(), len5);
	pos += len5;
	//����
	memcpy(&data[pos], &len6, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLoginPasswd.c_str(), len6);
	pos += len6;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	//�����������Ȩ�򷵻���Ȩ��Ϣ����Ȩ����ܺ󱣴浽����
	//��Ȩ��Ϊ���ݿͻ��˱�ʶ���ݼ��ܵõ��ģ���ͻ��˱�ʶһһ��Ӧ
	if (!sock.Send(data, pos))
	{
		sMsg = "send new user msg fail";
		return false;
	}

	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "new user fail, " + msg;
		return false;
	}

	return true;
}

//ɾ���û�
bool _client_t::del_user(string &user, string &loginuser, string &loginpasswd)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	//��ȡ��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CONTROL_DEL_USER);

	string sUser = user;
	int len1 = sUser.length();
	string sLoginUser = loginuser;
	int len2 = sLoginUser.length();
	string sLoginPasswd = loginpasswd;
	int len3 = sLoginPasswd.length();
	//��ȡ��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 3 * sizeof(int)+len1 + len2 + len3;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//�û���
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sUser.c_str(), len1);
	pos += len1;
	//��¼
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLoginUser.c_str(), len2);
	pos += len2;
	//����
	memcpy(&data[pos], &len3, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLoginPasswd.c_str(), len3);
	pos += len3;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	//�����������Ȩ�򷵻���Ȩ��Ϣ����Ȩ����ܺ󱣴浽����
	//��Ȩ��Ϊ���ݿͻ��˱�ʶ���ݼ��ܵõ��ģ���ͻ��˱�ʶһһ��Ӧ
	if (!sock.Send(data, pos))
	{
		sMsg = "send new user msg fail";
		return false;
	}

	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "del user fail, " + msg;
		return false;
	}

	return true;
}

//�޸��û�
bool _client_t::update_user(string &user, string &passwd, int &permissions, string &loginuser, string &loginpasswd)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	//��ȡ��Ȩ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CONTROL_UPD_USER);

	string sUser = user;
	int len1 = sUser.length();
	string sPasswd = passwd;
	int len2 = sPasswd.length();
	string sTmp;
	sTmp=std::to_string(permissions);
	string sPermissions = sTmp;
	int len3 = sPermissions.length();
	string sLoginUser = loginuser;
	int len4 = sLoginUser.length();
	string sLoginPasswd = loginpasswd;
	int len5 = sLoginPasswd.length();
	//��ȡ��Ȩ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 5 * sizeof(int)+len1 + len2 + len3 + len4 + len5;
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	//�û���
	memcpy(&data[pos], &len1, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sUser.c_str(), len1);
	pos += len1;
	//����
	memcpy(&data[pos], &len2, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sPasswd.c_str(), len2);
	pos += len2;
	//����
	memcpy(&data[pos], &len3, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sPermissions.c_str(), len3);
	pos += len3;
	//��¼
	memcpy(&data[pos], &len4, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLoginUser.c_str(), len4);
	pos += len4;
	//����
	memcpy(&data[pos], &len5, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], sLoginPasswd.c_str(), len5);
	pos += len5;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	//�����������Ȩ�򷵻���Ȩ��Ϣ����Ȩ����ܺ󱣴浽����
	//��Ȩ��Ϊ���ݿͻ��˱�ʶ���ݼ��ܵõ��ģ���ͻ��˱�ʶһһ��Ӧ
	if (!sock.Send(data, pos))
	{
		sMsg = "send update user msg fail";
		return false;
	}

	char resp[MAX_DATA] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg(&resp[pos], data_len);
	pos += data_len;

	if (0 == ret)
	{
		sMsg = "update user fail, " + msg;
		return false;
	}

	return true;
}

//
int _client_t::get_result_list(const std::string &scan_id, std::string &pic_id)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return -1;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return -1;
	}

	//ͼƬ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_GET_RESULT_LIST);

	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = sizeof(int)+scan_id.length();
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	int len = scan_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "get resultlist msg fail";
		return -1;
	}

	char resp[MAX_DATA + PIC_SIZE * 2] = { 0 };
	int ret = recv_resp(resp);
	if (0 > ret)
	{
		return -1;
	}

	if (0 == ret)
	{
		//std::string msg(&resp[pos], data_len);
		sMsg = "resultlist get fail";
		return -1;
	}

	//����body
	int data_len;
	memcpy(&data_len, resp, sizeof(int));
	pos = sizeof(int);
	std::string msg1(&resp[pos], data_len);
	pos += data_len;

	if ("ok" == msg1)
	{
		return 0;
	}
	pic_id = msg1;

	return 1;
}

//
bool _client_t::get_result(const std::string &scan_id, std::string &pic_id, unsigned short *pic, int pic_len, std::string &pic_s, std::string &info)
{
	if (!bConnection)
	{
		sMsg = "no init connection";
		return false;
	}

	if ("" == sAuth)
	{
		sMsg = "invalid auth";
		return false;
	}

	//ͼƬ����ʵ��
	req_head_t tHead;
	set_req_head(tHead, REQ_CMD_GET_RESULT);

	//ͼƬ��������
	//ͷ��+�ͻ��˱�ʶ���ݴ�С(int)+�ͻ��˱�ʶ����
	tHead.length = 2 * sizeof(int)+scan_id.length() + pic_id.length();
	//�������ݿ���
	char data[MAX_DATA] = { 0 };
	int pos = 0;
	memcpy(&data[pos], &tHead, sizeof(req_head_t));
	pos += sizeof(req_head_t);
	int len = scan_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], scan_id.c_str(), len);
	pos += len;
	//pic_id
	len = pic_id.length();
	memcpy(&data[pos], &len, sizeof(int));
	pos += sizeof(int);
	memcpy(&data[pos], pic_id.c_str(), len);
	pos += len;
	//�������δ��Ȩ�򷵻ش�����Ϣ
	if (!sock.Send(data, pos))
	{
		sMsg = "get result msg fail";
		return false;
	}


	char resp[MAX_DATA] = { 0 };
	if (!recv_data(resp, sizeof(resp_head_t)))
	{
		sMsg = "get resp head fail 1";
		return false;
	}
	//����ͷ��
	resp_head_t *resp_head = new resp_head_t();
	memcpy(resp_head, resp, sizeof(resp_head_t));
	if (10000000 < resp_head->length || 0 == resp_head->length)
	{
		sMsg = "get resp head fail 2";
		return false;
	}
	//printf("anth head: %d, %d, %d\n", 
	//	resp_head->length, resp_head->result, resp_head->timestamp);

	//char tmp[26] = {0};
	//sprintf(tmp, "%d-%d-%d", resp_head->result, resp_head->length, resp_head->timestamp);
	//sprintf(tmp, "%d-%d-%d", resp_head->length, data_len, recv);
	//sMsg = tmp;
	//return false;

	char *respData = new char[resp_head->length + 1];
	memset(respData, 0, resp_head->length + 1);

	if (!recv_data(respData, resp_head->length))
	{
		delete[] respData;
		sMsg = "get resp data fail";
		return false;
	}

	//����body
	int data_len;
	memcpy(&data_len, respData, sizeof(int));
	pos = sizeof(int);
	std::string msg1(&respData[pos], data_len);
	pos += data_len;
	info = msg1;

	if (0 != resp_head->result)
	{
		//std::string msg(&resp[pos], data_len);
		sMsg = "result get fail, " + info;
		return false;
	}

	memcpy(&data_len, &respData[pos], sizeof(int));
	pos += sizeof(int);
	std::string msg2(&respData[pos], data_len);
	pos += data_len;

	if ("0" == msg2)
	{
		memcpy(&data_len, &respData[pos], sizeof(int));
		pos += sizeof(int);
		int cnt = data_len / sizeof(unsigned short);
		if (cnt > pic_len)
			cnt = pic_len;
		for (int i = 0; i<cnt; ++i)
		{
			pic[i] = *((unsigned short*)(&respData[pos] + i * sizeof(unsigned short)));
		}
		pos += data_len;

		pic_s = "";
	}
	else
	{
		memcpy(&data_len, &respData[pos], sizeof(int));
		pos += sizeof(int);
		std::string msg3(&respData[pos], data_len);
		pos += data_len;

		pic_s = msg3;
	}

	delete[] respData;
	return true;
}
