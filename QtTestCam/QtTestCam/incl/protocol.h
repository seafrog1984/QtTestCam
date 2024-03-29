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

//请求头
//cmd: 1获取授权，2验证授权，3上传图片数据，4上传数据项，5下载数据项，6下载图片，7结果数据
typedef struct _req_head
{
	int length;     //不包含头部的长度
	int version;    //协议版本，目前为10001
	int cmd;        //数据类型;
	int timestamp;  //当前时间，时间戳格式
} req_head_t;

//应答头
typedef struct _resp_head
{
	int length;     //不包含头部的长度
	int result;     //结果
	int timestamp;  //当前时间，时间戳格式
} resp_head_t;

/*
//个人信息数据
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
