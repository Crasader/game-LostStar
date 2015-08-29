
#ifndef SIMPLEPROTOCOLCMD_H
#define SIMPLEPROTOCOLCMD_H

#include "cocos2d.h"
using namespace cocos2d;
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

//#define Current_ProtocolVersion 1 //Lite�� ����
#define Current_ProtocolVersion 2   //PR��  ż��


#define SimpleProtocol_Tag_Server 21335
#define SimpleProtocol_Tag_Client 21336

// ��ЧId
#define INVALID_ID 0
    
//�ͻ��˵�¼
#define ClientAccountMaxLen 32
#define ClientPswMaxLen     16


/* ���ذ���Ϣ������� */
enum MessageErrorCode {
    Success             	= 0,	//�ɹ�
    Error               	= 1,	//����
    Nonsupport              = 2,	//��֧��
};


/* Э��ָ�� */
enum MessageCmd {
    /* �������� */
    ReqCmdStart             = 0,
    
    // ͬ��
    Req_Sync				= ReqCmdStart + 1,
    // �ϴ��ؿ���Ϣ
    Req_UploadTaskInfo      = ReqCmdStart + 2,
    // �ϴ��ⷨ
    Req_UploadSolution      = ReqCmdStart + 3,
    // ���ؽⷨ
    Req_DownloadSolution    = ReqCmdStart + 4,
    // ���޽ⷨ
    Req_UpSolution          = ReqCmdStart + 5,
    // ��ѯ������Ϣ
    Req_QueryFriendsInfo    = ReqCmdStart + 6,

    ReqCmdEnd,


    /* �������� */
    RspCmdStart             = 10000,
    
    // ͬ��
    Rsp_Sync				= RspCmdStart + 1,
    // �ϴ��ؿ���Ϣ
    Rsp_UploadTaskInfo      = RspCmdStart + 2,
    // �ϴ��ⷨ
    Rsp_UploadSolution      = RspCmdStart + 3,
    // ���ؽⷨ
    Rsp_DownloadSolution    = RspCmdStart + 4,
    // ���޽ⷨ
    Rsp_UpSolution          = RspCmdStart + 5,
    // ��ѯ������Ϣ
    Rsp_QueryFriendsInfo    = RspCmdStart + 6,


    // ����������󷵻�����
    Rsp_RequestError        = RspCmdStart + 10000,
    RspCmdEnd,
};


//���յ��ֽڶ���
#pragma pack(1) 


/* ��ͷ */
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


#pragma pack() //����


#endif //SIMPLEPROTOCOLCMD_H

