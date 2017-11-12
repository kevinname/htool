#pragma once

#include "comm_hal.h"

#define PROTOCOL_SOH            0x01    
#define PROTOCOL_EOT            0x04
#define PROTOCOL_ACK            0x06
#define PROTOCOL_NAK            0x15
#define PROTOCOL_CAN            0x18

#define PROTOCOL_PACKDATA_LEN   1024
#define PROTOCOL_PACKAGE_LEN    (PROTOCOL_PACKDATA_LEN + 4)

#define PROTOCOL_ERROR_MAXNUMBER   0x200000

typedef enum
{
	RES_ERR_TRANSMIT           = -3,
	RES_ERR_CANCEL             = -2,
	RES_ERR_PARAM              = -1,

	RES_OK                     = 0,
}ProtocolRes;

typedef struct
{
	unsigned int totalCnt;
	unsigned int successCnt;

	unsigned int totalErrCnt;

	unsigned int lastErrCnt;
	unsigned int lastErrIdx;
}hs_backcall_para;

typedef void (*FN_BackCall)(hs_backcall_para *pPara);

class comm_protocal
{
public:
	comm_protocal(void);
	~comm_protocal(void);

public:
	comm_hal hs_transfer;

	long hs_open(const char * pDevice);	
	long hs_setup(void * pstPara);
	long hs_close(void);
	void hs_slelectChannel(hs_comm_type channel);
	ProtocolRes hs_transferData(unsigned char * pchPtr, unsigned int len, hs_backcall_para *pTransStatus);
};

