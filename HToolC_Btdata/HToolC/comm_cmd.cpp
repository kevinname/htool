#include "StdAfx.h"
#include "comm_cmd.h"


comm_cmd::comm_cmd(void)
{
}


comm_cmd::~comm_cmd(void)
{
}


// send general command, and then receive the response
int comm_cmd::SendCmd(unsigned short cmd, unsigned char *  paraBuf, unsigned char paraLen, unsigned char * resBuf, unsigned char resLen)
{
	unsigned char packData[COMM_CMD_DATA_LEN];
	short packLen;
	CWaitCursor hWaitCursor;
	
	hs_transfer.clrReadBuf(0);
	hs_transfer.clrReadBuf(1);
	packLen = packetCmd(packData, cmd, paraBuf, paraLen, resLen);
	if(hs_transfer.hs_write(packData, packLen) != 0)
		return COMM_CMD_ERR_WRITEOPER;	

	return getResponse(resBuf, resLen);
}


int comm_cmd::startDownload(unsigned char * dataBuf, unsigned int dataLen, unsigned int * speed, hs_backcall_para *pTransStatus)
{
	unsigned char packData[COMM_CMD_DATA_LEN];
	unsigned char resData[4];
	short packLen;
	ProtocolRes res;
	int eventRes;
	unsigned int tmp;

	packLen = packetCmd(packData, COMM_CMD_ENTER_DOWNLOAD, NULL, 0, 4);
	if(hs_transfer.hs_write(packData, packLen) != 0)
		return COMM_CMD_ERR_WRITEOPER;

	res = hs_transferData(dataBuf, dataLen, pTransStatus);	
	eventRes = getResponse(resData, 4);
	if(eventRes == 0)
	{
		tmp = resData[3];
		tmp = tmp << 8 + resData[2];
		tmp = tmp << 8 + resData[1];
		tmp = tmp << 8 + resData[0];

		*speed = tmp;
	}

	if(res != 0)
		return res;
	else
		return eventRes;

	return 0;
}


int comm_cmd::packetCmd(unsigned char * packetBuf, unsigned short cmd, unsigned char * paraBuf, unsigned char paraLen, unsigned char resLen)
{
	int offset = 0;

	if(packetBuf == NULL)
		return 0;

	packetBuf[offset++] = COMM_CMD_HEADER;
	packetBuf[offset++] = COMM_HCI_OPCODE;
	packetBuf[offset++] = COMM_HCI_OPCODE >> 8;
	packetBuf[offset++] = paraLen + 3;

	packetBuf[offset++] = cmd;
	packetBuf[offset++] = cmd >> 8;
	packetBuf[offset++] = resLen;

	if(paraLen > 0)
	{
		memcpy(&packetBuf[offset], paraBuf, paraLen);
		offset += paraLen;
	}

	return offset;
}


int comm_cmd::getResponse(unsigned char * resBuf, unsigned char resLen)
{
	unsigned char resHeader[COMM_EVENT_TOTALHEAD_LEN];
	unsigned char responseLen;
	char res;
	unsigned short opcode;
	int cnt = 15;

	res = hs_transfer.hs_read(resHeader, 1, 0);

#if 0
	//TRACE("hs_read: %d, data[0]:%x\r\n", res, resHeader[0]);
	while(resHeader[0] != COMM_EVENT_HEADER)
	{		
		if(cnt < 5)
		{
			res = hs_transfer.hs_read(resHeader, 1, 0);
			//TRACE("hs_read: %d, cnt:%d, data[0]:%x\r\n", res, cnt, resHeader[0]);
		}
		else
		{
			res = hs_transfer.hs_readDirect(resHeader, 1, 0);
			//TRACE("hs_readDirect: %d, cnt:%d, data[0]:%x\r\n", res, cnt, resHeader[0]);
		}

		if(!cnt--)
		{
			return COMM_CMD_ERR_READOPER;
		}
	};
#else
	if((resHeader[0] != COMM_EVENT_HEADER) || (res != 0))
	{
		return COMM_CMD_ERR_READOPER;
	}
#endif

	Sleep(10);

	if(0 != hs_transfer.hs_readDirect(resHeader+1, COMM_EVENT_TOTALHEAD_LEN-2, 0))
	{
		TRACE("read hci header error!\r\n");
		return COMM_CMD_ERR_READOPER;
	}

	opcode = resHeader[5];
	opcode = opcode << 8 + resHeader[4];

	if((resHeader[1] != COMM_EVENT_COMPLETE) && (opcode != COMM_HCI_OPCODE))
	{
		hs_transfer.hs_readDirect(resHeader, 1, 1);
		return COMM_CMD_ERR_EVENTHEADER;
	}

	if(resHeader[2] == 4)
	{
		hs_transfer.hs_readDirect(&resHeader[COMM_EVENT_TOTALHEAD_LEN-1], 1, 1);
		return resHeader[COMM_EVENT_TOTALHEAD_LEN-1];
	}

	hs_transfer.hs_readDirect(&resHeader[COMM_EVENT_TOTALHEAD_LEN-1], 1, 0);

	if(resHeader[6] != 0)
		return resHeader[6];

	if(resLen != 0)
	{
		unsigned long r_len;

		do
		{
			hs_transfer.hs_comm_uart.Read(resBuf, resLen, &r_len);

			resLen -= r_len;
			resBuf += r_len;
		}while(resLen > 0);
	}

	return resHeader[COMM_EVENT_TOTALHEAD_LEN-1];
}
