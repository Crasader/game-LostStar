
#ifndef SIMPLEPROTOCOLCMD_H
#define SIMPLEPROTOCOLCMD_H

#include "cocos2d.h"
using namespace cocos2d;
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

//#define Current_ProtocolVersion 1 //Lite版 奇数
#define Current_ProtocolVersion 2   //PR版  偶数


#define SimpleProtocol_Tag_Server 21335
#define SimpleProtocol_Tag_Client 21336

// 无效Id
#define INVALID_ID 0
    
//客户端登录
#define ClientAccountMaxLen 32
#define ClientPswMaxLen     16


/* 返回包消息错误代码 */
enum MessageErrorCode {
    Success             	= 0,	//成功
    Error               	= 1,	//错误
    Nonsupport              = 2,	//不支持
};


/* 协议指令 */
enum MessageCmd {
    /* 请求命令 */
    ReqCmdStart             = 0,
    
    // 同步
    Req_Sync				= ReqCmdStart + 1,
    // 上传关卡信息
    Req_UploadTaskInfo      = ReqCmdStart + 2,
    // 上传解法
    Req_UploadSolution      = ReqCmdStart + 3,
    // 下载解法
    Req_DownloadSolution    = ReqCmdStart + 4,
    // 点赞解法
    Req_UpSolution          = ReqCmdStart + 5,
    // 查询好友信息
    Req_QueryFriendsInfo    = ReqCmdStart + 6,

    ReqCmdEnd,


    /* 返回命令 */
    RspCmdStart             = 10000,
    
    // 同步
    Rsp_Sync				= RspCmdStart + 1,
    // 上传关卡信息
    Rsp_UploadTaskInfo      = RspCmdStart + 2,
    // 上传解法
    Rsp_UploadSolution      = RspCmdStart + 3,
    // 下载解法
    Rsp_DownloadSolution    = RspCmdStart + 4,
    // 点赞解法
    Rsp_UpSolution          = RspCmdStart + 5,
    // 查询好友信息
    Rsp_QueryFriendsInfo    = RspCmdStart + 6,


    // 请求命令错误返回命令
    Rsp_RequestError        = RspCmdStart + 10000,
    RspCmdEnd,
};


//按照单字节对齐
#pragma pack(1) 


/* 包头 */
struct SPHead
{
    unsigned short protocol_tag;
    unsigned short protocol_ver;
    unsigned short cmd;
    unsigned char not_use1;
    unsigned char not_use2;
    int data_len;
    //char protocol_buf_message[];
    //int check_sum;
};

inline void hton_SPHead(SPHead* head)
{
    head->protocol_tag = htons(head->protocol_tag);
    head->protocol_ver = htons(head->protocol_ver);
    head->cmd = htons(head->cmd);
    head->data_len = htonl(head->data_len);
}

inline void ntoh_SPHead(SPHead* head)
{
    head->protocol_tag = ntohs(head->protocol_tag);
    head->protocol_ver = ntohs(head->protocol_ver);
    head->cmd = ntohs(head->cmd);
    head->data_len = ntohl(head->data_len);
}

inline bool isValiedServerHead(SPHead* head)
{
	unsigned short tag = ntohs(head->protocol_tag);
	unsigned short ver = ntohs(head->protocol_ver);
	unsigned short cmd = ntohs(head->cmd);
	if (tag != SimpleProtocol_Tag_Server) return false;
	if (ver != Current_ProtocolVersion) return false;
	if ((cmd > RspCmdStart && cmd < RspCmdEnd)/* || (cmd > OtherCmdStart && cmd < OtherCmdEnd)*/) return true;
	return false;
}

inline bool isValiedClientHead(SPHead* head)
{
	unsigned short tag = ntohs(head->protocol_tag);
	unsigned short ver = ntohs(head->protocol_ver);
	unsigned short cmd = ntohs(head->cmd);
	if (tag != SimpleProtocol_Tag_Client) return false;
	if (ver != Current_ProtocolVersion) return false;
	if ((cmd > ReqCmdStart && cmd < ReqCmdEnd)/* || (cmd > OtherCmdStart && cmd < OtherCmdEnd)*/) return true;
	return false;
}

inline MessageCmd getRspcmdByReqcmd(MessageCmd reqCmd)
{
	return (MessageCmd)(RspCmdStart + (reqCmd - ReqCmdStart));
}


#pragma pack() //结束


#endif //SIMPLEPROTOCOLCMD_H

