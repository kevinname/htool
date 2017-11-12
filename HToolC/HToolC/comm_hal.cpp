#include "StdAfx.h"
#include "comm_hal.h"
#include "../serial/serial.h"


comm_hal::comm_hal(void)
{
}


comm_hal::~comm_hal(void)
{
}


long comm_hal::hs_open(const char * pDevice)
{
	return hs_comm_uart.Open(pDevice, 16, 16, true);
}


long comm_hal::hs_setup(void * pstPara)
{
	long res = -1;

	hs_uart_para *pstUart = (hs_uart_para *)pstPara;

	res = hs_comm_uart.Setup(pstUart->eBaudrate, 
		                           pstUart->eDataBits, 
								   pstUart->eParity, 
								   pstUart->eStopBits);
	if(res != ERROR_SUCCESS)
	{
		return hs_comm_uart.GetLastError();
	}

	res = hs_comm_uart.SetMask(//CSerial::EEventBreak |
							   CSerial::EEventError |
							   //CSerial::EEventRing  |
							   //CSerial::EEventRLSD  |
							   CSerial::EEventRecv);
	if(res != ERROR_SUCCESS)
	{
		return hs_comm_uart.GetLastError();
	}

	res = hs_comm_uart.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
	if(res != ERROR_SUCCESS)
	{
		return hs_comm_uart.GetLastError();
	}	

	return res;
}


long comm_hal::hs_write(const void * pData, unsigned int dataLen)
{
	long res;
	DWORD dwWritten;

	res = hs_comm_uart.Write(pData, dataLen, &dwWritten);
	TRACE("hs_comm_uart.Write res: %x\r\n", res);
	if(dataLen != dwWritten)
		return -1;

	return 0;
}


long comm_hal::hs_read(void * pData, unsigned int dataLen, int clrFlg)
{
	long res = -1;
	unsigned long readLen = 0;

	hs_read_restart:
	res = hs_comm_uart.WaitEvent(0, 15000);
	if(res != ERROR_SUCCESS)
	{
		return hs_comm_uart.GetLastError();
	}

	const CSerial::EEvent eEvent = hs_comm_uart.GetEventType();

	if(eEvent == 0)
		goto hs_read_restart;

	if (eEvent & CSerial::EEventError)
	{
		return hs_comm_uart.GetLastError();
	}

	if (eEvent & CSerial::EEventRecv)
	{
		res = this->hs_comm_uart.Read(pData, dataLen, &readLen);
		if(readLen != dataLen)
		{
			res = -1;
			return res;
		}
	}

	if(clrFlg != 0)
		PurgeComm(hs_comm_uart.GetCommHandle(), UART_CLR_FLAG);

	return 0;
}


long comm_hal::hs_close(void)
{
	return hs_comm_uart.Close();	
}


long comm_hal::hs_readDirect(void * pData, unsigned int dataLen, int clrFlg)
{
	long res = -1;
	unsigned long readLen = 0;

	res = this->hs_comm_uart.Read(pData, dataLen, &readLen);
	if(readLen != dataLen)
	{
		res = -1;
		return res;
	}

	if(clrFlg != 0)
		PurgeComm(hs_comm_uart.GetCommHandle(), UART_CLR_FLAG);

	return 0;
}


void comm_hal::clrReadBuf(int read)
{
	if(read)
		PurgeComm(hs_comm_uart.GetCommHandle(), PURGE_RXABORT|PURGE_RXCLEAR);
	else
		PurgeComm(hs_comm_uart.GetCommHandle(), PURGE_TXABORT|PURGE_TXCLEAR);
}
