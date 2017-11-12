#include "StdAfx.h"
#include "comm_protocal.h"


comm_protocal::comm_protocal(void)
{
}


comm_protocal::~comm_protocal(void)
{
}

long comm_protocal::hs_open(const char * pDevice)
{
	return hs_transfer.hs_open(pDevice);
}


long comm_protocal::hs_setup(void * pPara)
{
	return hs_transfer.hs_setup(pPara);
}


long comm_protocal::hs_close(void)
{	
	return hs_transfer.hs_close();
}


void comm_protocal::hs_slelectChannel(hs_comm_type channel)
{
	
}

unsigned int cal_checksum(unsigned char *ptr,unsigned int len)
{
	unsigned int i,u32ChkSum = 0;

	for(i=0; i<len; i++)
		u32ChkSum += ptr[i];

	return u32ChkSum;
}

static void hs_packageData(unsigned char * pSrc, unsigned char * pPackage, unsigned char cnt, int len)
{
	memset(pPackage, 0, PROTOCOL_PACKAGE_LEN);

	pPackage[0] = PROTOCOL_SOH;
	pPackage[1] = cnt;
	pPackage[2] = ~cnt;

	memcpy(&pPackage[3], pSrc, len);

	pPackage[PROTOCOL_PACKAGE_LEN-1] = cal_checksum(&pPackage[3], PROTOCOL_PACKDATA_LEN);
}

ProtocolRes comm_protocal::hs_transferData(unsigned char * pchPtr, unsigned int len, hs_backcall_para *pTransStatus)
{
	unsigned int blkcnt, errCnt = 0;	
	unsigned char dataBuf[PROTOCOL_PACKAGE_LEN];
	long res;
	unsigned int packCnt;
	unsigned char *pBufPtr;
	bool resFlg;
	char recData[10];
	int transLen, leftLen = len;

	memset((void *)pTransStatus, 0, sizeof(hs_backcall_para));

	blkcnt = len/PROTOCOL_PACKDATA_LEN;
	if(len % PROTOCOL_PACKDATA_LEN)
		blkcnt ++;

	if(blkcnt == 0)
		return RES_ERR_PARAM;

	pTransStatus->totalCnt = blkcnt;

	do
	{
		res = hs_transfer.hs_read((void *)recData, 1, 1);
		if(res != 0)
			//return RES_ERR_TRANSMIT;
			continue;

		TRACE("Transmit start? 0x%x\r\n", recData[0]);

		if(recData[0] != (char)PROTOCOL_NAK)
		{
			errCnt ++;
			if(errCnt > PROTOCOL_ERROR_MAXNUMBER)
				return RES_ERR_TRANSMIT;

			continue;
		}

		pBufPtr = pchPtr;
		packCnt = 1;
		errCnt = 0;
		do
		{
			if(leftLen >= PROTOCOL_PACKDATA_LEN)
				transLen = PROTOCOL_PACKDATA_LEN;
			else
				transLen = leftLen;

			hs_packageData(pBufPtr, dataBuf, packCnt, transLen);
			res = hs_transfer.hs_write(dataBuf, PROTOCOL_PACKAGE_LEN);
			if(res != 0)
				return RES_ERR_TRANSMIT;

		getRes:
			resFlg = false;
			res = hs_transfer.hs_read((void *)recData, 1, 1);
			TRACE("Transmit response result:0x%x, data: 0x%x\r\n", res, recData[0]);
			if(res != 0)
				return RES_ERR_TRANSMIT;

			if(recData[0] == (char)PROTOCOL_ACK)
			{
				pBufPtr += PROTOCOL_PACKDATA_LEN;
				packCnt ++;

				pTransStatus->successCnt ++;

				leftLen -= transLen;
				errCnt = 0;
			}
			else if(recData[0] == (char)PROTOCOL_CAN)
			{
				res = hs_transfer.hs_read((void *)recData, 1, 1);
				if(res != 0)
					return RES_ERR_TRANSMIT;

				if(recData[0] == PROTOCOL_CAN)
					return RES_ERR_CANCEL;
			}
			else if(recData[0] == (char)PROTOCOL_NAK)
			{
				errCnt ++;
				pTransStatus->totalErrCnt++;				

				pTransStatus->lastErrIdx = packCnt;
				pTransStatus->lastErrCnt = errCnt;
			}
			else
			{
				/* receive a error response */
				resFlg = true;
			}			

			if(resFlg == true)
				goto getRes;

			//if(pTransStatus->successCnt == pTransStatus->totalCnt)
			if(leftLen <= 0)
			{
				do
				{
					dataBuf[0] = PROTOCOL_EOT;
					res = hs_transfer.hs_write(dataBuf, 1);
					if(res != 0)
						return RES_ERR_TRANSMIT;

					res = hs_transfer.hs_read((void *)recData, 1, 0);
					if(res != 0)
						return RES_ERR_TRANSMIT;

				}while(recData[0] != PROTOCOL_ACK);

				return RES_OK;
			}

			if(pTransStatus->lastErrCnt > PROTOCOL_ERROR_MAXNUMBER)
				return RES_ERR_TRANSMIT;
		}while(1);
	}while(1);	

	return RES_ERR_TRANSMIT;
}
