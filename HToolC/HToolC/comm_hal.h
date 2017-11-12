#pragma once

#include "../serial/serial.h"

#define UART_CLR_FLAG		PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR
//#define UART_CLR_FLAG		PURGE_TXCLEAR|PURGE_RXCLEAR

typedef enum
{
	HS_COMMTYPE_NODEV          = 0,
	HS_COMMTYPE_UART           ,
	HS_COMMTYPE_USB            ,
	HS_COMMTYPE_BLE            ,
	HS_COMMTYPE_BULT           ,
}hs_comm_type;

typedef struct
{
	CSerial::EBaudrate eBaudrate;
	CSerial::EDataBits eDataBits;
	CSerial::EParity   eParity;
	CSerial::EStopBits eStopBits;
}hs_uart_para;


class comm_hal
{
public:
	comm_hal(void);
	~comm_hal(void);

public:
	CSerial hs_comm_uart;

	long hs_open(const char * pDevice);	
	long hs_setup(void * pstPara);
	long hs_write(const void * pData, unsigned int dataLen);
	long hs_read(void * pData, unsigned int dataLen, int clrFlg);
	long hs_close(void);
	
	long hs_readDirect(void * pData, unsigned int dataLen, int clrFlg);
	void clrReadBuf(int read);
};

