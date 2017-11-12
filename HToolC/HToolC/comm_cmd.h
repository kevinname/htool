#pragma once
#include "comm_protocal.h"
#include "..\..\..\bootrom\lib\comm\comm_cmd.h"

#define COMM_CMD_DATA_LEN           258
#define COMM_EVENT_DATA_LEN         258

#define COMM_HCI_OPCODE             0xfd60u

#define COMM_CMD_HEADER             1
#define COMM_EVENT_HEADER           4
#define COMM_EVENT_COMPLETE         0xe
#define COMM_EVENT_TOTALHEAD_LEN    9

enum
{
	COMM_CMD_ERR_READOPER			= -11,
	COMM_CMD_ERR_WRITEOPER			= -12,
	COMM_CMD_ERR_EVENTHEADER		= -13,
};

#define __comm_byte2word(buf, word)				\
do                                              \
{                                               \
  word = buf[1];                                \
  word = (word << 8) + buf[0];                  \
}while(0)

#define __comm_byte2dword(buf, word)            \
do                                              \
{                                               \
  word = buf[3];                                \
  word = (word << 8) + buf[2];                  \
  word = (word << 8) + buf[1];                  \
  word = (word << 8) + buf[0];                  \
}while(0)


class comm_cmd :
	public comm_protocal
{
public:
	comm_cmd(void);
	~comm_cmd(void);

public:
	// send general command, and then receive the response
	int SendCmd(unsigned short cmd, unsigned char *  paraBuf, unsigned char paraLen, unsigned char * resBuf, unsigned char resLen);
	int startDownload(unsigned char * dataBuf, unsigned int dataLen, unsigned int * speed, hs_backcall_para *pTransStatus);
	int packetCmd(unsigned char * packetBuf, unsigned short cmd, unsigned char * paraBuf, unsigned char paraLen, unsigned char resLen);
	int getResponse(unsigned char * resBuf, unsigned char resLen);
};

