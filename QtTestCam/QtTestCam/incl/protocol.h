#ifndef PROTOCOL__H__
#define PROTOCOL__H__



#define   REQ_CMD_GET_AUTH      1
#define   REQ_CMD_CHECK_AUTH    2
#define   REQ_CMD_SET_SCANID    3
#define   REQ_CMD_SEND_PNG      4
#define   REQ_CMD_SEND_DATA     5
#define   REQ_CMD_GET_PNG       6
#define   REQ_CMD_GET_DATA      7
#define   REQ_CMD_SEND_RESULT   8
#define   REQ_CMD_SET_CARDID    13
#define   REQ_CMD_GET_RESULT_LIST    14
#define   REQ_CMD_GET_RESULT    15
#define   REQ_CMD_GET_SCANID    16

#define   REQ_CONTROL_GET_LIST  9
#define   REQ_CONTROL_NEW_USER  10
#define   REQ_CONTROL_DEL_USER  11
#define   REQ_CONTROL_UPD_USER  12



#pragma pack(1)

//����ͷ
//cmd: 1��ȡ��Ȩ��2��֤��Ȩ��3�ϴ�ͼƬ���ݣ�4�ϴ������5���������6����ͼƬ��7�������
typedef struct _req_head
{
	int length;     //������ͷ���ĳ���
	int version;    //Э��汾��ĿǰΪ10001
	int cmd;        //��������;
	int timestamp;  //��ǰʱ�䣬ʱ�����ʽ
} req_head_t;

//Ӧ��ͷ
typedef struct _resp_head
{
	int length;     //������ͷ���ĳ���
	int result;     //���
	int timestamp;  //��ǰʱ�䣬ʱ�����ʽ
} resp_head_t;

/*
//������Ϣ����
typedef struct _usr_info
{
std::string id;
std::string name;
std::string sex;
std::string age;
std::string identity;
std::string pics;
} usr_info_t;
*/

typedef struct _permissions
{
	bool p1;
	bool p2;
	bool p3;

	_permissions() : p1(false), p2(false), p3(false) {}
} permissions_t;

#pragma pack()


#endif
