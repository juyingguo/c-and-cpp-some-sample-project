/*******************************��Ȩ��Ϣ*************************************
 * �ļ����� : AppConfig.h
 * �ļ��汾 : 1.0
 * ������Ա : Yao
 * �������� : 2010.03.04
 * ����ԭ�� : ���¶����ļ���С����������Ϊ64λ
 * �ļ�˵�� : ������Ӧ�ó���Ļ���������Ϣ
 ****************************************************************************/

#pragma once

#include "winsock2.h"
#pragma comment(lib,"wsock32.lib")


/**************************�����ļ�����ض���*******************************/

/**************************�����ļ������������Ŀ***************************/
#define MAX_CONFIG_ITEM												64
/**************************�����ļ�·��*************************************/
#define CONFIG_PATH													_T("\\config.ini")
/**************************��������󳤶�***********************************/
#define MAX_BUF_LEN													1024
/**************************�ļ�·����󳤶�*********************************/
#define MAX_FILE_PATH_LEN											256
/**************************�����ַ�������***********************************/
#define FILTER														_T("=")
/**************************������ȶ���***********************************/
#define CONFIGITEMNUM												_T("��������Ŀ")
/**************************�����������***********************************/
#define INDEX_CONFIG_ITEM_NUM										0
/**************************��Чֵ����***************************************/
#define INVALID_VALUE												-1

/***************************************************************************/
/******************************�����ļ���������*****************************/
/***************************************************************************/
#define INDEX_SERVER_PORT											1	//server & client
#define INDEX_MAX_CONNECT											2	//server
#define INDEX_SERVER_ADDRESS										2	//client

/*************************���ֻ������� Ĭ��Ϊʮ����*************************/
#define RADIX														10
/*************************ʱ�䵥λ����**************************************/
#define TIME_UNIT													60
#define SECOND														1
#define MINUTE														(UNIT*SECOND)
#define HOUR														(UNIT*MINUTE)
/**************************�ļ��鳤�ȶ���***********************************/
#define KB															1024
#define MB															(KB*KB)
const double GB = (double)(KB) * (double)(MB);
const double TB = (double)(KB) * (double)(GB);
#define BLOCK_SIZE													(256*KB)	//256KB
/**************************���ļ�����***************************************/
#define READ_ERROR													-1
/**************************д�ļ�����***************************************/
#define WRITE_ERROR													-2

/**************************�˿ڷ�Χ����*************************************/
#define MIN_PORT													1024
#define MAX_PORT													65535

/**************************��������Χ����***********************************/
#define MIN_CONNECT													1
#define MAX_CONNECT													32


/***************************************************************************/
/*******************************ͨ��ģ����ض���****************************/
/***************************************************************************/
/******************************������ʱ��Ϣ*******************************/
#define MAX_ELAPSE													10
/***********************************״̬��������****************************/
#define STATE_NUM													3
/*********************************״̬����**********************************/
typedef enum
{
	INIT						= 0,								//��ʼ̬
	CONNECTED					= 1,								//������
	TRANSFERRING				= 2,								//������
}State;

/********************************��Ϣ��Χ����*******************************/
#define START_INDEX_MSG												(WM_USER + 1)
#define END_INDEX_MSG												(WM_USER + 9)

/**********************ͨ����Ϣ�ṹ����*************************************/
typedef enum
{
	/********************server�˷��͵���Ϣ���Ͷ���*************************/
	SEND_FILE_INFO_REQ			= (WM_USER + 1),					//�����ļ���Ϣ����
	SEND_FILE_REQ				= (WM_USER + 2),					//�����ļ�����
	/********************client�˷��͵���Ϣ���Ͷ���*************************/
	RECV_FILE_INFO_ACK			= (WM_USER + 3),					//�����ļ���Ϣ�ɹ�
	RECV_FILE_ACK				= (WM_USER + 4),					//�����ļ���ɹ�
	/********************������Ϣ����***************************************/
	INTERRUPT_TRANSFER			= (WM_USER + 5),					//�жϴ���
	CLOSE_CONNECT				= (WM_USER + 6),					//�ر�����
	/********************ģ������ͼͨ����Ϣ����*****************************/
	CONNECT_SUCCESS				= (WM_USER + 7),					//���ӳɹ�
	TRANSFERRING_FILE			= (WM_USER + 8),					//�����ļ�
	TRANSFER_OVER				= (WM_USER + 9),					//������������
}MessageType;

/***********************�Զ�������ݴ�С����********************************/
typedef __int64 Size;

/************************�ļ���Ϣ�ṹ����***********************************/
typedef struct  
{
	char _name[MAX_FILE_PATH_LEN];									//�ļ���
	//long _fileSize;												//�ļ���С
	//DWORD _size;
	Size _fileSize;													//�ļ���С
}FileInfo;

/**************************�������õ����ݽṹ����***************************/
typedef struct  
{
	BYTE _buf[BLOCK_SIZE];											//���͵��ļ�������
	int _len;														//�ֿ�ĳ���
	//long _offset;													//�ļ�����ƫ��ֵ
	//DWORD _offSet;												//�ļ�����ƫ��ֵ
	Size _offset;													//�ļ�����ƫ��ֵ
}TransferData;

/**********************server��client���͵���Ϣ�����ݲ��ֵĶ���*************/
typedef union 
{
	FileInfo _fileInfo;												//�ļ���Ϣ����һ�δ��������͵�����
	TransferData _transferData;										//��������
}DataFromServer;

/**************************server��client���͵���Ϣ�ṹ�Ķ���***************/
typedef struct  
{
	MessageType _messageType;										//��Ϣ����
	DataFromServer _data;											//���ݲ���
}MessageFromServer;

/**************************client��server���͵���Ϣ�ṹ����*****************/
typedef struct  
{
	MessageType _message;											//��Ϣ����
	//long _offset;													//�ļ�ƫ��ֵ��Ϊ�ϵ������ṩ��֤
	//DWORD _offSet;												//�ļ�ƫ��ֵ��Ϊ�ϵ������ṩ��֤
	Size _offset;													//�ļ�ƫ��ֵ��Ϊ�ϵ������ṩ��֤
}MessageFromClient;

/**************************server��clientͨ�ŵ���Ϣ�ṹ�Ķ���***************/
typedef union
{
	MessageFromClient _messageFromClient;
	MessageFromServer _messageFromServer;
}Message;
/**************************�ٽ�������***************************************/
static CRITICAL_SECTION g_csMessage;								//��Ϣͨ���߳����õ��ٽ���
static CRITICAL_SECTION g_csTransfer;								//�ļ������߳����õ��ٽ���


/***************************************************************************/
/******************************��ͼ��ض���*********************************/
/***************************************************************************/

/**************************������������Զ���*******************************/
#define MIN_RANGE													0
#define MAX_RANGE													100
#define STEP														1
/************************�����ĵ���·��*************************************/
#define HELP_PATH													_T("//Help.chm")
/**************************���͸�����������Ϣ*******************************/
#define WM_PROGRESS													(WM_USER+100)
/************************��С����������Ϣ�Ķ���*****************************/
#define WM_SHOWTASK													(WM_USER+101)

/**********************��Ҫ�ı�״̬�Ŀؼ������������************************/
#define MAX_CONTROL_NUM														10

/**********************server����Ҫ�ı�״̬�Ŀؼ���������*******************/
#define SERVER_CTL_NUM												4
/***********************client����Ҫ�ı�״̬�Ŀؼ���Ŀ***********************/
#define CLIENT_CTL_NUM												5
/***********************client��Ĭ�ϵ��ļ�����Ŀ¼***************************/
#define DEFAULT_FILE_PATH											_T("\\Temp")

/************************������Ϣ���¶�ʱ����********************************/
#define TIMER_UPDATE												0
/************************Ĭ�ϵ���ʱ����**************************************/
#define ELAPSE_DEFAULT												1000
/************************�����ٶȵ�ʱ����**********************************/
#define ELAPSE_UPDATE												1000