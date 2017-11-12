// hs_package.cpp : implementation file
//

#include "stdafx.h"
#include "HToolC.h"
#include "hs_package.h"
#include "afxdialogex.h"
#include "comm_protocal.h"
#include "HToolCDlg.h"
#include "math.h"

#define CONFIG_FILE   "\\config\\config_evb_pos.ini"
#define PACK_FILE     "\\pack\\hs6601_pos.pack"

static bool need_pack;
static bool new_pack;
static uint32_t    addr_offset = 0x80;
static char file_boot[256] = { 0 };
// hs_package dialog

IMPLEMENT_DYNAMIC(hs_package, CDialogEx)

hs_package::hs_package(CWnd* pParent /*=NULL*/)
	: CDialogEx(hs_package::IDD, pParent)
{
	
}

hs_package::~hs_package()
{
	if(hs_option.pInfoDataBuf != NULL)
	{
		free(hs_option.pInfoDataBuf);
		hs_option.pInfoDataBuf = NULL;
	}
}

void hs_package::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHIPSTATUS_GPIO, hs_combChipStatusGpio);
	DDX_Control(pDX, IDC_COMBO_UPGRADERESULT_GPIO, hs_combUpgradeResGpio);
	DDX_Control(pDX, IDC_COMBO_CHIPSTATUS_GPIO_LVL, hs_combChipBusyLvl);
	DDX_Control(pDX, IDC_COMBO_UPGRADERESULT_GPIO_LVL, hs_combUpgradeOkLvl);
	DDX_Control(pDX, IDC_EDIT_IMAGE_FILE, hs_editImgFile);
	DDX_Control(pDX, IDC_EDIT_CONFIG_FILE, hs_editCfgFile);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_FILE, hs_editOutFile);
	DDX_Control(pDX, IDC_CHECK_ANDESBURNER, hs_chkAndesBurner);
	DDX_Control(pDX, IDC_COMBO_BTADDR_CHANGE_MODE, hs_combBTAddrMode);
	DDX_Control(pDX, IDC_EDIT_BTADDR_START, hs_editBTAddrStart);
	DDX_Control(pDX, IDC_EDIT_BTADDR_END, hs_editBTAddrEnd);
	DDX_Control(pDX, IDC_EDIT_SF_CPM_REG, hs_editSFClkCfgCPM);
	DDX_Control(pDX, IDC_EDIT_SF_DIV_REG, hs_editSFClkDiv);
	DDX_Control(pDX, IDC_CHECK_SF_CLOCK_CFG, hs_chkClkCfgEn);
	DDX_Control(pDX, IDC_EDIT_BTNAME, hs_editBTName);
	DDX_Control(pDX, IDC_COMBO_FLASHMEM_MODE, hs_combFmemMode);
	DDX_Control(pDX, IDC_COMBO_IMAGE_TYPE, hs_combImgType);
	DDX_Control(pDX, IDC_EDIT_EXEADDR, hs_editExeAddr);
	DDX_Control(pDX, IDC_BUTTON_INPUT_CONFIG, hs_bnSelCfg);
	DDX_Control(pDX, IDC_COMBO_S_PORT, hs_combUartDevice);
	DDX_Control(pDX, IDC_EDIT_S_BOARDRATE, hs_editBaudrate);
	DDX_Control(pDX, IDC_EDIT_S_PACK_FILE, hs_editFileName);
	DDX_Control(pDX, IDC_BUTTON_GET_PACK_FILE, hs_bnSelectFile);

	DDX_Control(pDX, IDC_BUTTON_PACKAGE, hs_bnDownload);
	DDX_Control(pDX, IDC_CHECK_S_PACK, hs_CheckPackNeed);

	DDX_Control(pDX, IDC_PROGRESS_S_LOADING, hs_s_statusProcess);
	DDX_Control(pDX, IDC_EDIT_S_FILE_SIZE, hs_s_statusFileSize);
	DDX_Control(pDX, IDC_EDIT_S_TOTAL_NUM, hs_s_statusTotalCnt);
	DDX_Control(pDX, IDC_EDIT_S_SCES_NUM, hs_s_statusSuccessCnt);
	DDX_Control(pDX, IDC_EDIT_TOTAL_ERR_NUM, hs_s_statusTotalErrCnt);
	DDX_Control(pDX, IDC_EDIT_S_LAST_ERR_NUM, hs_s_statusLastErrCnt);
	DDX_Control(pDX, IDC_EDIT_S_LAST_ERR_INDEX, hs_s_statusLastErrIdx);
	DDX_Control(pDX, IDC_BUTTON_INPUT_IMAGE, hs_getImagefile);
	DDX_Control(pDX, IDC_BUTTON_PACKED_BIN, hs_outPackfile);
	DDX_Control(pDX, IDC_EDIT_GET_ADDR_FILE, hs_editGetAddrFile);
	DDX_Control(pDX, IDC_BUTTON_GET_ADDR_FILE, hs_bnGetAddrFile);
	DDX_Control(pDX, IDC_EDIT_SHOW_ADDR, hs_editShowAddress);
	DDX_Control(pDX, IDC_EDIT_SHOW_ADDR_MOD, hs_editShowAddrMode);
	DDX_Control(pDX, IDC_EDIT_SHOW_NAME, hs_editShowName);
	DDX_Control(pDX, IDC_EDIT_SHOW_ADDR_ORG, hs_editShowAddrOrg);
	DDX_Control(pDX, IDC_EDIT_PACK_VERSION, hs_editShowPackVersion);
	DDX_Control(pDX, IDC_EDIT_CFG_FLASH_ADDR, hs_editCfgFlashAddr);
}


BEGIN_MESSAGE_MAP(hs_package, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_PACKAGE, &hs_package::OnBnClickedButtonPackage)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_IMAGE, &hs_package::OnBnClickedButtonInputImage)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_CONFIG, &hs_package::OnBnClickedButtonInputConfig)
	ON_BN_CLICKED(IDC_BUTTON_PACKED_BIN, &hs_package::OnBnClickedButtonPackedBin)
	ON_CBN_SELCHANGE(IDC_COMBO_BTADDR_CHANGE_MODE, &hs_package::OnCbnSelchangeComboBtaddrChangeMode)
	ON_BN_CLICKED(IDC_CHECK_SF_CLOCK_CFG, &hs_package::OnBnClickedCheckSfClockCfg)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE_TYPE, &hs_package::OnCbnSelchangeComboImageType)
	ON_BN_CLICKED(IDC_BUTTON_GET_PACK_FILE, &hs_package::OnBnClickedButtonGetPackFile)
	ON_BN_CLICKED(IDC_CHECK_S_PACK, &hs_package::OnBnClickedCheckSPack)
	ON_BN_CLICKED(IDC_BUTTON_GET_ADDR_FILE, &hs_package::OnBnClickedButtonGetAddrFile)
	ON_BN_CLICKED(IDC_BUTTON2, &hs_package::OnBnClickedButton2)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////


static void bytesTOuintHL( unsigned char address_byte[]  , uint32_t *pH, uint32_t *pL)
{
	uint32_t   address_H3=0;
	uint32_t   address_L3=0;
	int i;
	for(i=5;i>=0;i--)
	{
		if(i>=3)
		{
			address_H3=address_H3<<8;
			address_H3+=(address_byte[i]);
		}
		else
		{
			
			address_L3=address_L3<<8;
			address_L3+=(address_byte[i]);	
		}			
	}
	*pH=address_H3;
	*pL=address_L3;
}
static void uintHLTObytes( unsigned char address_byte[] ,  uint32_t *pH, uint32_t *pL)
{
	uint32_t address_L3;
	uint32_t address_H3;
	address_L3=*pL;
	address_H3=*pH;
	int i;
	for(i=0;i<6;i++)
	{
		if(i<3)
		{
			(address_byte[i])=(address_L3&0xFF);
			address_L3=address_L3>>8;
		}
		else
		{
			(address_byte[i])=(address_H3&0xFF);
			address_H3=address_H3>>8;
		}
		
	}

}

static uint32_t getAddrIndex(unsigned char address_buf[])
{
	int i;
	uint32_t data_addr=0;
	for(i=4;i<12;i++)
	{
		data_addr*=10;
		data_addr+=(address_buf[i]-'0');
	}
	return data_addr;

}
static void turnAddrIndex(unsigned char address_buf[],uint32_t addr_index)
{
	int i;
	int data_sigle;
	memset(address_buf,'0',12);
	for(i=11;i>=4&&addr_index!=0;i--)
	{
		data_sigle=addr_index%10;
		addr_index=addr_index/10;
		address_buf[i]=(data_sigle)+'0';
	}

}

static void getNewAddr(unsigned char address_buf[],unsigned char address[])
{
	int i,j;
	unsigned char key_buf[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x7,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	int key_buf_index;
	unsigned char address_sigle=0x00;
	for(i=0,j=5;i<12;i++)
	{
		address_sigle=address_sigle<<4;
		key_buf_index=address_buf[i]-'0';
		address_sigle|=key_buf[key_buf_index];
		if(i%2!=0)
		{
			address[j]=address_sigle;
			j--;
		}	
	}
}

/*************************** For The Pos ********************************/
static int CopyPosImage(__in LPCSTR lpExistingFileName, __in LPCSTR lpNewFileName)
{
	FILE *inFile, *outFile;
	char buf[1024], convertBuf[3];
	int i;
	unsigned char tmp;

	if (fopen_s(&inFile, lpExistingFileName, "rb"))
	{
		return -1;
	}

    DeleteFile(lpNewFileName);

	if (fopen_s(&outFile, lpNewFileName, "wb+"))
	{
		fclose(inFile);
		return -1;
	}

	memset(buf, '/', sizeof(buf));
	while (fgets(buf, sizeof(buf), inFile) != NULL){
		for (i = 0; i < sizeof(buf); i++){
			if (buf[i] == '/' || buf[i] == 0 || buf[i] == '\r'){
				break;
			}

			if (buf[i] == ',' || buf[i] == ' '){
				continue;
			}

			convertBuf[0] = buf[i];
			convertBuf[1] = buf[i + 1];
			convertBuf[2] = 0;
			tmp = strtol(convertBuf, 0, 16);
			fwrite(&tmp, 1, sizeof(tmp), outFile);
			i++;
		}
		memset(buf, '/', sizeof(buf));
	}

	fclose(inFile);
	fclose(outFile);

	return 0;
}

/************************************************************************/

// hs_package message handlers
BOOL hs_package::OnInitDialog(void)
{
	
	need_pack=false;
	new_pack=false;
	new_config_file=false;
	char str[256];
	int i;
	
    GetCurrentDirectory(256, file_boot);

	CDialogEx::OnInitDialog();	
	hs_combImgType.InsertString(0, _T("Upgrade Flash"));
	hs_combImgType.InsertString(1, _T("Loading and Execute"));
	hs_combImgType.SetCurSel(0);

	hs_editCfgFlashAddr.SetWindowTextA(_T("516096"));

	hs_editExeAddr.SetWindowTextA(_T("30000000"));
	hs_editExeAddr.EnableWindow(0);


	for(i=0; i<CFG_GPIO_NUM; i++)
	{
		sprintf_s(str,"%d", i);
		hs_combChipStatusGpio.InsertString(i, str);
		hs_combUpgradeResGpio.InsertString(i, str);
	}

	hs_combChipBusyLvl.InsertString(0, _T("0"));
	hs_combUpgradeOkLvl.InsertString(0, _T("0"));
	hs_combChipBusyLvl.InsertString(1, _T("1"));
	hs_combUpgradeOkLvl.InsertString(1, _T("1"));

	hs_combChipStatusGpio.SetCurSel(0);
	hs_combUpgradeResGpio.SetCurSel(0);
	hs_combChipBusyLvl.SetCurSel(0);
	hs_combUpgradeOkLvl.SetCurSel(0);

	hs_combBTAddrMode.InsertString(0, _T("Fixed"));
	hs_combBTAddrMode.InsertString(1, _T("Increased"));
	hs_combBTAddrMode.InsertString(2, _T("Increased and Loop"));
	hs_combBTAddrMode.InsertString(3, _T("File List"));
	hs_combBTAddrMode.SetCurSel(0);
	
	hs_editBTAddrStart.SetWindowTextA(_T("00 : 00 : 00 : 00 : 00 : 00"));
	hs_editBTAddrEnd.SetWindowTextA(_T("1000000"));

 	hs_editBTAddrEnd.EnableWindow(0);

	hs_combFmemMode.InsertString(0, _T("Single"));
	hs_combFmemMode.InsertString(1, _T("Dual"));
	hs_combFmemMode.InsertString(2, _T("Quad"));
	hs_combFmemMode.SetCurSel(2);

	hs_editSFClkCfgCPM.SetWindowTextA(_T("21d"));
	hs_editSFClkDiv.SetWindowTextA(_T("4"));

	hs_editSFClkCfgCPM.EnableWindow(0);
	hs_editSFClkDiv.EnableWindow(0);
	hs_chkClkCfgEn.SetCheck(0);

	

	hs_option.pInfoDataBuf = (char *)malloc(CONFIG_LEN);
	hs_option.infoDataAreaPtr = 0;
	hs_option.infoDataBackPtr = 0;
	hs_option.infoDataIndex = 0;
	hs_option.infoIndexData = 0;
	hs_option.infoManageFlg =0;
	hs_option.infoMaxIndex = 0;
	hs_option.manageDataPtr = 0;

	hs_option.cfgBinLen = 0;

	memset(&hs_packinfo, 0 , sizeof(hs_packinfo_t));
	
	only_addr_mode_change=true;

//////////////////////////////////////////////////////////////////////////
	long j, index, res;
	char devName[32];

	hs_comm.hs_slelectChannel(HS_COMMTYPE_UART);

	index = 0;
	for(j=0;j<51; j++)
	{		
		sprintf_s(devName,"\\\\.\\com%d", j);
		res = hs_comm.hs_open(devName);
		if(!res)
		{			
			strcpy_s(uartDevList[index], devName);

			sprintf_s(devName,"com%d", j);

			this->hs_combUartDevice.InsertString(index, _T(devName));		
			index++;
		}

		hs_comm.hs_close();
	}

	this->hs_combUartDevice.SetCurSel(0);
	this->hs_editBaudrate.SetWindowTextA(_T("115200"));

	hs_comm.hs_slelectChannel(HS_COMMTYPE_NODEV);
	bConnectFlg = false;
	bTransFlg = false;
	memset(p_option.w_packFile, 0, DIR_PATCH_LENGTH);

	//////////////////////////////////////////////////////////////////////////
	hs_editExeAddr.ShowWindow(false);
	hs_chkAndesBurner.ShowWindow(false);
	hs_chkClkCfgEn.ShowWindow(false);
	hs_combChipBusyLvl.ShowWindow(false);
	hs_combChipStatusGpio.ShowWindow(false);
	hs_combUpgradeOkLvl.ShowWindow(false);
	hs_combUpgradeResGpio.ShowWindow(false);
	hs_combImgType.ShowWindow(false);
	hs_combFmemMode.ShowWindow(false);
	hs_editSFClkDiv.ShowWindow(false);
	hs_editSFClkCfgCPM.ShowWindow(false);
	hs_editCfgFile.ShowWindow(false);
	hs_bnSelCfg.ShowWindow(false);
	hs_editOutFile.ShowWindow(false);
	hs_outPackfile.ShowWindow(false);
	hs_editFileName.ShowWindow(false);
	hs_bnSelectFile.ShowWindow(false);

	hs_editGetAddrFile.EnableWindow(0);
	hs_bnGetAddrFile.EnableWindow(0);

	hs_CheckPackNeed.SetCheck(FALSE);
	OnBnClickedCheckSPack();
	return true;
}

//////////////////////////////////////////////////////////////////////////


int hs_package::getBtConfigFlashAddr(void)
{
	char str[30] = { 0 };
	hs_editCfgFlashAddr.GetWindowTextA(str, 20);
	return atoi(str);
}


static int getfilelen_s(FILE *fp)
{
	int temp,len;

	temp = ftell(fp);
	fseek(fp,0,SEEK_END);	
	len = ftell(fp);
	fseek(fp,temp,0);	

	return len;
}

void hs_package::UartConnect(bool connect_or_not)
{
	long res;
	unsigned int index, dataTemp;
	hs_uart_para uartPara;
	char str[20];
	
	if(connect_or_not)
	{
		hs_comm.hs_close();
		hs_comm.hs_slelectChannel(HS_COMMTYPE_UART);

		index = this->hs_combUartDevice.GetCurSel();

		res = hs_comm.hs_open(uartDevList[index]);
		if(res != 0)
		{
			MessageBox(_T("uart open error!"), _T("Connect Error"), MB_ICONEXCLAMATION|MB_OK);
			return ;
		}

		this->hs_editBaudrate.GetWindowTextA(str, 20);
		sscanf_s(str,"%d",&dataTemp);

		uartPara.eBaudrate = (CSerial::EBaudrate)dataTemp;
		uartPara.eDataBits = CSerial::EData8;  //(CSerial::EDataBits)(this->hs_combDataWidth.GetCurSel() + 5);
		uartPara.eParity   = CSerial::EParNone; //(CSerial::EParity)this->hs_combParity.GetCurSel();
		uartPara.eStopBits = CSerial::EStop1; //(CSerial::EStopBits)this->hs_StopBits.GetCurSel();

		res = hs_comm.hs_setup(&uartPara);
		if(res != 0)
		{
			MessageBox(_T("uart setup error!"), _T("Connect Error"), MB_ICONEXCLAMATION|MB_OK);
			return ;
		}

		bConnectFlg = true;	
	}
	else
	{
		hs_comm.hs_close();
		hs_comm.hs_slelectChannel(HS_COMMTYPE_NODEV);
		bConnectFlg = false;
	}
	saveDlg();
}
//////////////////////////////////////////////////////////////////////////

int getFileLen_s(FILE *fp)
{
	int temp,len;

	temp = ftell(fp);
	fseek(fp,0,SEEK_END);	
	len = ftell(fp);
	fseek(fp,temp,0);	
	
	return len;
}

static unsigned int cal_checksum(unsigned char *ptr,unsigned int len)
{
	unsigned int i,u32ChkSum = 0;

	for(i=0; i<len; i++)
		u32ChkSum += ptr[i];

	return u32ChkSum;
}

CString hs_package::GetValidString(LPCSTR chInput)
{
	CString str=chInput;
	str.TrimLeft();
	str.TrimRight();
	CString strReturn=str;
	if(!str.IsEmpty())
	{
		int start=str.Find('/');
		if(start!=-1)
		{
			strReturn=str.Mid(0,start);
			strReturn.TrimRight();
		}
	}
	return strReturn;
}

void hs_package::getPara(char ** str)
{
	int i;

	for(i=0; i<hs_option.defineIndex; i++)
	{
		if(strcmp(*str, hs_option.define[i]) ==0 )
		{
			* str = hs_option.defineVal[i];
			return ;
		}
	}

	return ;
}

void hs_package::ParseStr(char * str, int *argc, char *argv[])
{
	int i = 0;
	int flag = 1;
	int len = strlen(str);

	(*argc) = 0;
	argv[0] = str;

	while(i < len)
	{
		switch(str[i])
		{
			case ' ':
			case 0x09:
			case ',':
				flag = 1;
				str[i] = 0;

				if((*argc) != 0)
				{
					getPara(&argv[(*argc) - 1]);
				}
				break;
			default:
				if(flag)
				{
					argv[*argc] = str+i;
					flag = 0;
					(*argc)++;

					if ((strcmp(argv[0], "string") == 0) && ((*argc) > 2))
					{
						return ;
					}
				}
				
				break;
		}
		i++;
	}
}

void hs_package::PromptError(char *ptr)
{
	char str[1024];

	sprintf_s(str,"some line conetent:\n\n[ %s ] \n\nExpression Error!",ptr);
	MessageBox(str,"infoPacking Warning",MB_ICONEXCLAMATION|MB_OK);

	return ;
}
int GetStrLen(char *str)
{
	int i;

	for(i=0;;i++)
		if(str[i] == 0)
			break;

	return i;

}

void index_to_byte(int val, char *pBuf)
{
    pBuf[0] = val & 0xff;
	pBuf[1] = (val >> 8) & 0xff;
	pBuf[2] = (val >> 16) & 0xff;
}

int GBKToUTF8(unsigned char * lpGBKStr,unsigned char * lpUTF8Str,int nUTF8StrLen)
{
    wchar_t * lpUnicodeStr = NULL;
    int nRetLen = 0;

    if(!lpGBKStr)  //如果GBK字符串为NULL则出错退出
        return 0;

    nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,NULL,NULL);  //获取转换到Unicode编码后所需要的字符空间长度
    lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
    nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
    if(!nRetLen)  //转换失败则出错退出
        return 0;

    nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,NULL,0,NULL,NULL);  //获取转换到UTF8编码后所需要的字符空间长度
    
    if(!lpUTF8Str)  //输出缓冲区为空则返回转换后需要的空间大小
    {
        if(lpUnicodeStr)
			delete []lpUnicodeStr;

        return nRetLen;
    }

    nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,(char *)lpUTF8Str,nUTF8StrLen,NULL,NULL);  //转换到UTF8编码    
    if(lpUnicodeStr)
        delete []lpUnicodeStr;
        
    return nRetLen;
}

int UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;
 
	if(!lpUTF8Str)  //如果UTF8字符串为NULL则出错退出
		return 0;
 
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,NULL,NULL);  //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
	if(!nRetLen)  //转换失败则出错退出
		return 0;
 
	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,NULL,NULL,NULL);  //获取转换到GBK编码后所需要的字符空间长度
 
	if(!lpGBKStr)  //输出缓冲区为空则返回转换后需要的空间大小
	{
		if(lpUnicodeStr)
		delete []lpUnicodeStr;
		return nRetLen;
	}
 
	if(nGBKStrLen < nRetLen)  //如果输出缓冲区长度不够则退出
	{
		if(lpUnicodeStr)
		delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL);  //转换到GBK编码
 
	if(lpUnicodeStr)
		delete []lpUnicodeStr;
 
	return nRetLen;
}

void hs_package::DoTone(void)
{
	CDialog *pParentDlg;
	CRect rect;
	CHToolCDlg *pMainDlg;
	std::list<CChildDlgInfo>::iterator child;
	hs_tone *pTone;
	bool findDlg = false;
	int i, size, temp;
	char *Dptr, str[255];

	pParentDlg = (CDialog *)GetParent()->GetParent();
	pMainDlg = (CHToolCDlg *)pParentDlg;
	for(child = pMainDlg->m_lstChildDlg.begin(); child != pMainDlg->m_lstChildDlg.end(); child++)
	{
		if(strcmp(child->strName, "Tone") == 0)
		{
			findDlg = true;
			break;
		}
	}

	if(findDlg == false)
	{
		return ;
	}

	pTone = (hs_tone *)child->pDlg;
	if( ! pTone->hs_chkToneEn.GetCheck())
	{
		return ;
	}

	//Dptr = (char *)malloc(CFG_TONE_MAX_SIZE * 64 * 3);
	pTone->doToneData();

	hs_option.infoDataAreaPtr = (hs_option.infoDataAreaPtr +0x4) & 0xfffffc;
	temp = hs_option.infoDataAreaPtr;
	temp = (temp << 2) & 0xffffff0;
	index_to_byte(temp, hs_option.indexTable + CFG_TONE_INDEX * CFG_INDEX_LEN);

	memcpy(&hs_option.pInfoDataBuf[hs_option.infoDataAreaPtr], pTone->p_option.ptr, pTone->p_option.data_ptr);
	hs_option.infoDataAreaPtr += pTone->p_option.data_ptr;
}

char hs_package::CompileSentence(CString strLine)
{
	char *ptr,*cmd;
	int argc = 0;
	char *argv[MAX_PARA_COUNT];
	char str[1024];
	int len = strLine.GetLength();
	int temp, attr, index, bk_len;
	unsigned int val, str_len;
	unsigned int addrW, i;
	FILE *patchFile;

	ptr = strLine.GetBuffer(len);
	strcpy_s(str,ptr);

	ParseStr(str,&argc,argv);

	cmd = argv[0];

	if(strcmp(cmd,"$index$")==0)
	{
		if((argc < 2) || (argc > 3))
		{
			PromptError(ptr);
			return 1;
		}

		if((GetStrLen(argv[1]) >= 3) || (!IsHex(argv[1][0])))
		{
			PromptError(ptr);
			return 1;
		}

		sscanf_s(argv[1],"%x",&index);
		if( (index > CFG_INDEX_MAX) && (index != 0xff))
		{
			PromptError(ptr);
			return 1;
		}		

		if(argc == 3)
		{
			sscanf_s(argv[2],"%x",&attr);
		}

		/* save last index data */
		ptr = hs_option.indexTable + hs_option.infoDataIndex * 3;
		if(hs_option.infoDataAreaPtr == hs_option.infoDataBackPtr)
		{
			index_to_byte(1, ptr);
		}
		else
		{
			temp = hs_option.infoIndexData & 0xfffffffe;
			index_to_byte(temp, ptr);			
		}

		/* save this index data info */
		if(index == 0)
		{
			hs_option.infoManageFlg = 1;
			hs_option.manageDataPtr = 0;
			hs_option.infoMaxIndex = 0;
		}		
		else if(index == 0xff)
		{
			DoTone();

			/* write manage info */
			hs_option.infoManageData.u32Date = hs_packinfo.u32InfoDate;
			hs_option.infoManageData.u32IndexCnt = hs_option.infoMaxIndex + 1;			
			hs_option.infoManageData.u32BodyAddr = CFG_BLOCK_SIZE;
			hs_option.infoManageData.u32Chksum = cal_checksum((unsigned char *)&hs_option.infoManageData, sizeof(hs_cfg_info_t));

			hs_option.infoDataAreaPtr = (hs_option.infoDataAreaPtr +0x4) &0xfffffffc;
			temp = hs_option.infoDataAreaPtr + hs_option.infoManageData.u32BodyAddr;
			temp = (temp << 2) & 0xffffff0;
			index_to_byte(temp, hs_option.indexTable);
			memcpy(&hs_option.pInfoDataBuf[hs_option.infoDataAreaPtr], &hs_option.infoManageData, sizeof(hs_cfg_info_t));
			hs_option.infoDataAreaPtr += sizeof(hs_cfg_info_t);

			/* write backup data area */
			hs_option.infoDataAreaPtr = (hs_option.infoDataAreaPtr +0x4) &0xfffffffc;
			bk_len = hs_option.infoManageData.u32BackupLen;
			temp = hs_option.infoDataAreaPtr + hs_option.infoManageData.u32BodyAddr;
			if(temp % CFG_BLOCK_SIZE)
			{
				hs_option.infoDataAreaPtr = ((temp + CFG_BLOCK_SIZE) & CFG_BLOCK_ALIGN_MASK) - hs_option.infoManageData.u32BodyAddr;
			}
				
			if(hs_option.infoManageData.u32BackupLen % CFG_BLOCK_SIZE)
			{
				bk_len = (hs_option.infoManageData.u32BackupLen + CFG_BLOCK_SIZE) & CFG_BLOCK_ALIGN_MASK;
			}

			temp = hs_option.infoDataAreaPtr + hs_option.infoManageData.u32BodyAddr;
			temp = (temp << 2) & 0xffffff0;
			index_to_byte(temp, hs_option.indexTable + CFG_INDEX_LEN);

			hs_option.infoDataAreaPtr += bk_len;
		}
		else
		{
			hs_option.infoManageFlg = 0;

			if(index > hs_option.infoMaxIndex)
				hs_option.infoMaxIndex = index;

			hs_option.infoDataIndex = index;

			if(hs_option.infoDataAreaPtr % 4)
				hs_option.infoDataAreaPtr = (hs_option.infoDataAreaPtr +0x4) &0xfffffffc;

			hs_option.infoDataBackPtr = hs_option.infoDataAreaPtr;
			hs_option.infoIndexData = (hs_option.infoDataAreaPtr << 2) & 0xffffff0;
			if(attr == 0)
				hs_option.infoIndexData &= ~2;
			else
				hs_option.infoIndexData |= 2;
		}
	}
	else if(strcmp(cmd,"define")==0)
	{
		strcpy(hs_option.define[hs_option.defineIndex], argv[1]);
		strcpy(hs_option.defineVal[hs_option.defineIndex], argv[2]);
		hs_option.defineIndex ++;
	}
	else
	{
		if(IsHex(cmd[0]))
		{
			for(temp=0;temp<argc; temp++)
			{
				addrW = (GetStrLen(argv[temp])+1)/2;
				sscanf_s(argv[temp],"%x",&val);

				if(hs_option.infoManageFlg == 1)
					ptr = (char *)&hs_option.infoManageData;
	
				i = 0;
				while(addrW)
				{
					if(hs_option.infoManageFlg == 1)
					{						
						ptr[hs_option.manageDataPtr++] = (val >> (8 * i)) & 0xff ;
					}
					else
					{
						hs_option.pInfoDataBuf[hs_option.infoDataAreaPtr++] = (val >> (8 * i)) & 0xff ;
					}
					addrW--;
					i ++;
				};
			}
		}
		else if(strcmp(cmd,"string")==0)
		{
			if(argc != 3)
			{
				PromptError(ptr);
				return 1;
			}

			sscanf_s(argv[1],"%x",&str_len);
			GBKToUTF8((unsigned char *)argv[2], (unsigned char *)&hs_option.pInfoDataBuf[hs_option.infoDataAreaPtr], str_len);			
			hs_option.infoDataAreaPtr += str_len;
		}
		else if(strcmp(cmd,"space")==0)
		{
			if(argc != 2)
			{
				PromptError(ptr);
				return 1;
			}

			sscanf_s(argv[1],"%x",&str_len);					
			hs_option.infoDataAreaPtr += str_len;
		}
		else
		{
			PromptError(ptr);
			return 1;
		}
	}	

	return 0;
}

unsigned int _halByteToWord(unsigned char *pu8Buf, unsigned int len)
{
  unsigned int i, u32Tmp, u32Val = 0;

  if(len > 4)
    return 0;

  for(i=0; i<len; i++)
  {
    u32Tmp = pu8Buf[i];
    u32Val |= u32Tmp << (i * 8);
  }

	return u32Val;
}

unsigned int getDataOffset(unsigned short index, unsigned char *dptr)
{
	unsigned int tmp, bodyAddr;

	tmp = index * 3;
	bodyAddr = _halByteToWord(dptr+tmp, 3);
	bodyAddr >>= 2;
	bodyAddr += CFG_BLOCK_SIZE;

	return bodyAddr;
}

void hs_package::OnBnClickedButtonPackage()
{
	
	if(bTransFlg==false)
	{
		if(need_pack==true)
		{	
			

			addr_offset = 0x80;
			FILE *inCodeBin, *outBinFile, *packInfo; 
			CWaitCursor hWaitCursor;
			errno_t fres;
			unsigned int dataTemp;
			char *ptr, str[256], *pCodeBuf, *pStep;
			CTime cTime;
			CString cStr;
			//////////////////////////////////////////////////////////////////////////
			hs_option.w_imagefile.Format("%s",p_option.w_imageFile);

			char file_boot_pack[256] = { 0 };
			strcpy(file_boot_pack, file_boot);
			strcat(file_boot_pack, PACK_FILE);
			hs_option.w_outputfile = file_boot_pack;
			
#if 0
			if(new_config_file==false)
			{
				hs_option.w_configfile.Format("%s",p_option.w_configFile);
				if(p_option.have_config_file==true)
				{
					hs_option.w_configfile.Format("%s",p_option.w_configFile);
					compileConfig();
				}
				else
					OnCbnSelchangeComboBtaddrChangeMode();
			}

#endif			
			//////////////////////////////////////////////////////////////////////////

			if(strcmp(hs_option.w_imagefile, "") == 0)
			{
				MessageBox(_T("Image Bin(TO PACK) file path invalidate!"), _T("infoPacking Error"), MB_ICONEXCLAMATION|MB_OK);
				return ;
			}

			fres = fopen_s(&inCodeBin, hs_option.w_imagefile, "rb" );
			if(fres)
			{
				MessageBox(_T("Open bin file failed!"),_T("infoPacking Warning"),MB_ICONEXCLAMATION|MB_OK);
				return ;
			}

			DeleteFile(hs_option.w_outputfile);
			fres = fopen_s(&outBinFile, hs_option.w_outputfile, "wb+" );
			if(fres)
			{
				MessageBox(_T("Open output packed file failed!"),_T("infoPacking Warning"), MB_ICONEXCLAMATION | MB_OK);
				return ;
			}

			cTime = CTime::GetCurrentTime();
			cStr = cTime.Format("%m%d%H%M");
			ptr = cStr.GetBuffer(cStr.GetLength());
			sscanf_s(ptr,"%x", &dataTemp);
/*  */		hs_pack_load_info_init.pack_version = dataTemp;
		
			hs_packinfo.u32InfoFlg = (0 == hs_combImgType.GetCurSel()) ? INFO_WR_FLAG : INFO_LD_FLAG;
			if(0 == hs_combImgType.GetCurSel())
			{
				hs_packinfo.u32InfoFlg = INFO_WR_FLAG;
			}
			else
			{
				hs_packinfo.u32InfoFlg = INFO_LD_FLAG;
				hs_packinfo.u32CfgDataEn = 0;

				hs_editExeAddr.GetWindowTextA(str, 256);
				sscanf(str, "%x", &hs_packinfo.u32ChgModeArgc0);
			}

			hs_packinfo.u32InfoDate = dataTemp;
			hs_packinfo.u32CipherTxtFlg = IMG_PLAIN;
			hs_packinfo.u32ImageLen = getfilelen_s(inCodeBin); // bin_length
			hs_packinfo.u32StatusIO = hs_combChipStatusGpio.GetCurSel();
			hs_packinfo.u32StatusIOLvl = hs_combChipBusyLvl.GetCurSel();
			hs_packinfo.u32ResultIO = hs_combUpgradeResGpio.GetCurSel();
			hs_packinfo.u32ResultIOLvl = hs_combUpgradeOkLvl.GetCurSel();
			hs_packinfo.u32KeyAddr = KEY_ADDR;    // the address where the pack_info locate
			hs_packinfo.u32InfoChkSum = 0;	

			unsigned int u32ImgPackLen;  // bin_length  --- 4K align
			u32ImgPackLen = (hs_packinfo.u32ImageLen + CFG_BLOCK_SIZE - 1) & CFG_BLOCK_ALIGN_MASK;
			u32ImgPackLen += INFO_SIZE;  //  bin_length  info_size
			pCodeBuf = (char *)malloc(u32ImgPackLen);
			memset((void *)pCodeBuf, 0, u32ImgPackLen);

			/* 将.bin 写.入 pcode_buf*/
			fread((void *)pCodeBuf, 1, hs_packinfo.u32ImageLen, inCodeBin);	


			if(getBtConfigFlashAddr() < u32ImgPackLen)
			{
				MessageBox(_T("The CfgFlashAddr Is Too Small!"),_T("Error"),MB_ICONHAND|MB_OK);
				fclose(inCodeBin);
				fclose(outBinFile);
				return;
			}
			addr_offset += getBtConfigFlashAddr() - u32ImgPackLen;

			// read new config.ini
			if(hs_packinfo.u32CfgDataEn)
			{

				//  have config.ini file
            	//hs_pack_load_info_init.name_offset = u32ImgPackLen + getDataOffset(HS_CFG_BT_DEV, (unsigned char *)hs_option.pInfoDataBuf);
// 				hs_packinfo.u32BTAddrOffset = u32ImgPackLen + getDataOffset(HS_CFG_BT_HC, (unsigned char *)hs_option.pInfoDataBuf);
//     			hs_pack_load_info_init.addr_offset=hs_packinfo.u32BTAddrOffset;
// 				hs_pack_load_info_init.name_offset=hs_pack_load_info_init.addr_offset+6;

				compileConfigPos(addr_offset);
			}
			else
			{   
				hs_option.cfgBinLen = 0;
			}

			hs_packinfo.u32BTAddrOffset = getBtConfigFlashAddr();
			hs_pack_load_info_init.addr_offset=hs_packinfo.u32BTAddrOffset+0x80;
			hs_pack_load_info_init.name_offset=hs_pack_load_info_init.addr_offset+6;
			hs_packinfo.u32BTAddrChgMode = hs_combBTAddrMode.GetCurSel();
			hs_packinfo.u32ChgModeArgc0 = 0;
			if(hs_packinfo.u32BTAddrChgMode == 2)
			{
				hs_editBTAddrEnd.GetWindowTextA(str, 256);
				sscanf(str, "%d", &hs_packinfo.u32ChgModeArgc1); // 循.环增加模式的LOOP大小
			}
				
			//  需要Pack时.写地址和name到pack
			configDataUpdate(addr_offset);


			hs_editBTName.GetWindowTextA(p_option.bt_name,256);
			p_option.bt_address_change_mode = hs_combBTAddrMode.GetCurSel();
			hs_editBTAddrEnd.GetWindowTextA(p_option.bt_loop_change_num ,256);
			hs_editBTAddrStart.GetWindowTextA(p_option.bt_address ,256);

			hs_packinfo.u32ImageLen = (u32ImgPackLen + hs_option.cfgBinLen + ALGIN_SIZE) & (~(ALGIN_SIZE-1));
			hs_packinfo.u32ImageLen -= INFO_SIZE;
			hs_packinfo.u32InfoChkSum = cal_checksum((unsigned char *)&hs_packinfo, INFO_SIZE);

			hs_pImgInfo = (hs_imginfo_t  *)(pCodeBuf + IMG_INFO_ADDR);
			hs_pImgInfo->u32ImgFlg = IMG_INFO_FLAG;
			hs_pImgInfo->u32ImageLen = u32ImgPackLen - INFO_SIZE;
			hs_pImgInfo->u32Version = hs_packinfo.u32InfoDate;
			hs_pImgInfo->u32ClkUsed = hs_chkClkCfgEn.GetCheck();
			if(hs_pImgInfo->u32ClkUsed)
			{
				hs_editSFClkCfgCPM.GetWindowTextA(str, 256);
				sscanf(str, "%x", &hs_pImgInfo->u32CpmRegSf);

				hs_editSFClkDiv.GetWindowTextA(str, 256);
				sscanf(str, "%x", &hs_pImgInfo->u32SFRegCfg);
			}
			else
			{
				hs_pImgInfo->u32CpmRegSf = 0;
				hs_pImgInfo->u32SFRegCfg = 0;
			}

			int fmemMode = hs_combFmemMode.GetCurSel();
			if(fmemMode == 0)
				hs_pImgInfo->u32FmemMode = FLASHMEM_MODE_SINGLE;
			else if(fmemMode == 1)
				hs_pImgInfo->u32FmemMode = FLASHMEM_MODE_DUAL;
			else
				hs_pImgInfo->u32FmemMode = FLASHMEM_MODE_QUAD;

			hs_pImgInfo->u32InfoChkSum = 0;
			hs_pImgInfo->u32InfoChkSum = cal_checksum((unsigned char *)pCodeBuf, hs_pImgInfo->u32ImageLen);

			int totalLen = 0;
			if(!hs_chkAndesBurner.GetCheck())
			{  //  1 写.入 pack_info(mode and so on)
				fwrite((void *)&hs_packinfo, 1, INFO_SIZE, outBinFile);
				totalLen = INFO_SIZE;
			}
			   //  2 写.入 image_bin  4K对.齐.的
			fwrite((void *)pCodeBuf, 1, hs_pImgInfo->u32ImageLen, outBinFile);
			totalLen += hs_pImgInfo->u32ImageLen;

			if(hs_packinfo.u32CfgDataEn)
			{
				// 3 写.入 config_ini(address  name)
				fwrite((void *)hs_option.pInfoDataBuf, 1, hs_option.cfgBinLen, outBinFile);
				totalLen = hs_packinfo.u32BTAddrOffset + hs_option.cfgBinLen;
			}
			memset((void *)pCodeBuf, 0, u32ImgPackLen);
			u32ImgPackLen = hs_packinfo.u32ImageLen + INFO_SIZE - (u32ImgPackLen + hs_option.cfgBinLen);
			//  4 写.入 empty image_bin_backup
			fwrite((void *)pCodeBuf, 1, u32ImgPackLen, outBinFile);
			totalLen += u32ImgPackLen;
 
			fclose(inCodeBin);

			fclose(outBinFile);
			free(pCodeBuf);
			saveDlg();
			if(new_config_file)
				new_config_file=false;

			CString info_dat=hs_option.w_outputfile+(CString)"_info.dat";
			DeleteFile(info_dat);
			fres = fopen_s(&packInfo,info_dat, "wb+" );
			// open address error
			if(fres)
			{
				MessageBox(_T("Open Pack Info Failed!"),_T("Open PACKINFO Error"),MB_ICONHAND|MB_OK);
				return;
			}
			
			hs_pack_load_info_init.pack_name_change_mod=0;
			hs_pack_load_info_init.pack_name_loop_num=0;
			hs_pack_load_info_init.pack_name_loop_position=0;
			hs_editBTName.GetWindowTextA(hs_pack_load_info_init.pack_bt_name_org, 256);
			hs_editBTAddrStart.GetWindowTextA(hs_pack_load_info_init.pack_bt_address_org, 256);
			hs_pack_load_info_init.pack_addr_change_mod=p_option.bt_address_change_mode;
			hs_pack_load_info_init.pack_addr_loop_num=hs_packinfo.u32ChgModeArgc1;
			hs_pack_load_info_init.pack_addr_loop_position=0;	
			fwrite((void*)&hs_pack_load_info_init,1,sizeof(pack_load_record_t),packInfo);
			fclose(packInfo);
		    saveDlg();
			
        }
		
		// do not need Repack
		UartConnect(TRUE);
		Download_Start();	
		return;
	}
	else
	{
		Download_Stop();
		UartConnect(false);
		hs_CheckPackNeed.EnableWindow(1);
		return;
	}

	
	
}

void hs_package::OnBnClickedButtonInputImage()
{
	char str[255];
	int tmp;

	CFileDialog dlg(TRUE, _T("*.*"), NULL, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("info Files (*.bin)|*.bin|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("Select image file");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;
	if(strcmp(p_option.w_imageCurDir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = p_option.w_imageCurDir;

	memset(p_option.w_imageFile, 0, DIR_PATCH_LENGTH);
	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_imagefile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editImgFile.SetWindowText(this->hs_option.w_imagefile);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_imageCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
		memcpy(p_option.w_imageFile ,(LPSTR)(LPCTSTR) dlg.GetPathName(),hs_option.w_imagefile.GetLength());
	}
	else
	{
		this->hs_option.w_imagefile = "";
		this->UpdateData(FALSE);
		this->hs_editImgFile.SetWindowText(this->hs_option.w_imagefile);
	}

	saveDlg();
}


void hs_package::OnBnClickedButtonInputConfig()
{
	new_config_file=true;
	char str[255];
	int tmp;

	CFileDialog dlg(TRUE, _T("*.*"), NULL, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("info Files (*.ini)|*.ini|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("Select config file");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;
	if(strcmp(p_option.w_configCurDir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = p_option.w_configCurDir;
	memset(p_option.w_configFile, 0, DIR_PATCH_LENGTH);

	if( dlg.DoModal() == IDOK )
	{
		// have choose config.ini file
		this->hs_option.w_configfile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editCfgFile.SetWindowText(this->hs_option.w_configfile);
		p_option.have_config_file=true;

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_configCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
		memcpy(p_option.w_configFile ,this->hs_option.w_configfile,hs_option.w_configfile.GetLength());
		// handle the config.ini file
		only_addr_mode_change=false;
		compileConfig();
	}
	else
	{
		// without choose the config.ini file
		this->hs_option.w_configfile = "";
		this->UpdateData(FALSE);
		this->hs_editCfgFile.SetWindowText(this->hs_option.w_configfile);
		p_option.have_config_file=false;
		hs_packinfo.u32CfgDataEn = 0;
		hs_option.cfgBinLen = 0;
		only_addr_mode_change=true;
		// and get this key_info (address  name and so on)
		OnCbnSelchangeComboBtaddrChangeMode();
	}

	saveDlg();
}


void hs_package::OnBnClickedButtonPackedBin()
{
	char str[255];
	int tmp;

	CFileDialog dlg(FALSE, _T("*.*"), NULL, 0, _T("info Files (*.pack)|*.pack|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("Save output file");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;//_T("e:");
	if(strcmp(p_option.w_packCurDir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = p_option.w_packCurDir;
	
	memset(p_option.w_outpackFile, 0, DIR_PATCH_LENGTH);

	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_outputfile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editOutFile.SetWindowText(this->hs_option.w_outputfile);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_packCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
		memcpy(p_option.w_outpackFile ,(LPSTR)(LPCTSTR) dlg.GetPathName(),hs_option.w_outputfile.GetLength());
	}
	else
	{
		this->hs_option.w_outputfile = "";
		this->UpdateData(FALSE);
		this->hs_editOutFile.SetWindowText(this->hs_option.w_outputfile);
	}

	saveDlg();
}


void hs_package::OnCbnSelchangeComboBtaddrChangeMode()
{
	/*
	get the INFO about address and name 
	*/
	unsigned int btaddr_offset;
	unsigned char *dbptr;
	char gbkname[64], str[256];
#if 0
	if(p_option.have_config_file==false)
	{
		hs_combBTAddrMode.EnableWindow(0);
		hs_editBTName.EnableWindow(0);
		hs_editBTAddrStart.EnableWindow(0);
		hs_editBTAddrEnd.EnableWindow(0);
	}
	else
#endif

	{
		/*
		if(hs_packinfo.u32CfgDataEn&&only_addr_mode_change==false)
		{
// 			btaddr_offset = getDataOffset(HS_CFG_BT_DEV, (unsigned char *)hs_option.pInfoDataBuf);
// 			dbptr = (unsigned char *)(hs_option.pInfoDataBuf + btaddr_offset);
// 			UTF8ToGBK(dbptr, (unsigned char *)gbkname, 64);
// 			hs_editBTName.SetWindowTextA(_T(gbkname));  //  show name


// 			btaddr_offset = getDataOffset(HS_CFG_BT_HC, (unsigned char *)hs_option.pInfoDataBuf);
			dbptr =  (unsigned char *)hs_option.pInfoDataBuf;
			sprintf(str, "%02X : %02X : %02X : %02X : %02X : %02X", dbptr[5], dbptr[4], dbptr[3], dbptr[2], dbptr[1], dbptr[0]);
			hs_editBTAddrStart.SetWindowTextA(_T(str));	 //  show address

			UTF8ToGBK(dbptr+6, (unsigned char *)gbkname, 64);
			hs_editBTName.SetWindowTextA(_T(gbkname));  //  show name

		}
		*/
			hs_combBTAddrMode.EnableWindow(1);
			hs_editBTName.EnableWindow(1);

			if((hs_combBTAddrMode.GetCurSel() == 0) || (hs_combBTAddrMode.GetCurSel() == 1))
			{
				hs_editBTAddrStart.EnableWindow(1);
				hs_editBTAddrEnd.EnableWindow(0);
			}

			if(hs_combBTAddrMode.GetCurSel() == 2)
			{
				hs_editBTAddrStart.EnableWindow(1);
				hs_editBTAddrEnd.EnableWindow(1);

				if(hs_packinfo.u32CfgDataEn&&only_addr_mode_change==false)
				{
					hs_editBTAddrEnd.SetWindowTextA(_T("1000000"));
				}
			}

			if(hs_combBTAddrMode.GetCurSel() == 3)
			{
				hs_editBTAddrStart.EnableWindow(0);
				hs_editBTAddrEnd.EnableWindow(0);
			}
		}
	//if(only_addr_mode_change==false)
	//	only_addr_mode_change=true;
	saveDlg();
}


void hs_package::compileConfig(void)
{
	FILE *inCfgFile; 
	CWaitCursor hWaitCursor;
	errno_t fres;
	unsigned int dataTemp;
	unsigned int u32CfgBinLen = 0;
	char *ptr, str[256], *pCfgBinBuf;
	CTime cTime;
	CString cStr;
	int m_lineNum;
	int m_lineCnt;
	char line[1024], res;
	CString strLineContent;

	if(0 == strcmp(hs_option.w_configfile, ""))
	{
	    // the config.ini file is empty
		hs_packinfo.u32CfgDataEn = 0;
		hs_packinfo.u32BTAddrOffset = 0xffffffff;
		return ;
	}

	//  start to read config.ini file
	hs_packinfo.u32CfgDataEn = 1;
	fres = fopen_s(&inCfgFile, hs_option.w_configfile, "rb" );
	if(fres)
	{
		//  open the config.ini file error
		hs_packinfo.u32CfgDataEn = 0;
		hs_packinfo.u32BTAddrOffset = 0xffffffff;
		return ;
	}

	/* begin to parse the config text */
	memset(hs_option.pInfoDataBuf, 0xff, CONFIG_LEN);
	memset(hs_option.indexTable, 0xff, CFG_BLOCK_SIZE);
	memset(&hs_option.infoManageData, 0, sizeof(hs_cfg_info_t));
	hs_option.infoDataAreaPtr = 0;
	hs_option.infoDataBackPtr = 0;
	hs_option.infoDataIndex = 0;
	hs_option.infoIndexData = 0;
	hs_option.infoManageFlg =0;
	hs_option.infoMaxIndex = 0;
	hs_option.manageDataPtr = 0;
	hs_option.defineIndex = 0;

	while( !feof(inCfgFile) )
	{
		if( fgets( line, 1024, inCfgFile )) 
		{
			CString strValid = GetValidString(line);
			if(!strValid.IsEmpty())
			{
				m_BatchData.Add(strValid);
			}
		}
	}

	CString strEnd = "$index$ ff 0";
	m_BatchData.Add(strEnd);
	fclose( inCfgFile );

	m_lineCnt = 0;
	m_lineNum = (int)m_BatchData.GetSize();

	while(m_lineCnt < m_lineNum)
	{
		strLineContent = m_BatchData.GetAt(m_lineCnt);
		res = CompileSentence(strLineContent);
		if(res == 1)
		{
			m_BatchData.RemoveAt(0,m_lineNum);
			fclose(inCfgFile);
			return ;
		}
		m_lineCnt ++;
	}
	m_BatchData.RemoveAt(0,m_lineNum);

	/* add a info header */
	u32CfgBinLen = hs_option.infoDataAreaPtr + hs_option.infoManageData.u32BodyAddr;
	if(u32CfgBinLen % PROTOCOL_PACKDATA_LEN)
		u32CfgBinLen = (u32CfgBinLen + PROTOCOL_PACKDATA_LEN) & ~(PROTOCOL_PACKDATA_LEN - 1);

	pCfgBinBuf = (char *)malloc(u32CfgBinLen);
	memset((void *) pCfgBinBuf, 0, u32CfgBinLen);
	memcpy((void *) pCfgBinBuf, hs_option.indexTable, hs_option.infoManageData.u32BodyAddr);
	memcpy((void *)(pCfgBinBuf + hs_option.infoManageData.u32BodyAddr), hs_option.pInfoDataBuf, hs_option.infoDataAreaPtr);

	hs_option.cfgBinLen = u32CfgBinLen;
	memcpy((void *)hs_option.pInfoDataBuf, (void *)pCfgBinBuf, hs_option.cfgBinLen);	
	free(pCfgBinBuf);
	OnCbnSelchangeComboBtaddrChangeMode();
}


void hs_package::OnBnClickedCheckSfClockCfg()
{
	if(hs_chkClkCfgEn.GetCheck())
	{
		hs_editSFClkCfgCPM.EnableWindow(1);
		hs_editSFClkDiv.EnableWindow(1);
	}
	else
	{
		hs_editSFClkCfgCPM.EnableWindow(0);
		hs_editSFClkDiv.EnableWindow(0);
	}

	saveDlg();
}

unsigned int parseStrForBtAddr(char *str, unsigned char addr[])
{
	int i, tmp, flag = 0, index = 5;
	char *ptr;

	strcat(str, " ");

	ptr = str;
	for(i=0; str[i] != '\0'; i++)
	{
		switch(str[i])
		{
			case ' ':			
			case ':':
				str[i] = 0;					
				if((flag == 0) && (ptr[0] != 0))
				{
					sscanf_s(ptr, "%x", &tmp);
					addr[index--] = tmp;

					if(index < 0)
						return 0;
				}

				flag = 1;
				break;
			default:
				if(flag)
				{
					ptr = &str[i];
					flag = 0;
				}
				break;
		}
	}

	return 0;
}

void hs_package::configDataUpdate(uint32_t cfg_addr)
{
	unsigned int btaddr_offset;
	char start[6], name[16], str[256], *ptr;
	int i, f;

	hs_editBTAddrStart.GetWindowTextA(str, 256);
	parseStrForBtAddr(str, (unsigned char *)start);
// 	btaddr_offset = getDataOffset(HS_CFG_BT_HC, (unsigned char *)hs_option.pInfoDataBuf);
// 	ptr = hs_option.pInfoDataBuf + btaddr_offset;
	ptr = hs_option.pInfoDataBuf+cfg_addr;
	memcpy(ptr, start, 6);

	hs_editBTName.GetWindowTextA(str, 256);	
	/*btaddr_offset+=6;*/
	/*btaddr_offset = getDataOffset(HS_CFG_BT_DEV, (unsigned char *)hs_option.pInfoDataBuf);*/
	/*ptr = hs_option.pInfoDataBuf + btaddr_offset;*/
	ptr = hs_option.pInfoDataBuf + cfg_addr + 6;
	GBKToUTF8((unsigned char *)str, (unsigned char *)ptr, 16);
}


void hs_package::OnCbnSelchangeComboImageType()
{
	if(0 == hs_combImgType.GetCurSel())
	{
		hs_editExeAddr.EnableWindow(0);
		hs_bnSelCfg.EnableWindow(1);
	}
	else
	{
		hs_editExeAddr.EnableWindow(1);
		hs_bnSelCfg.EnableWindow(0);
	}

	saveDlg();
}


void hs_package::saveDlg(void)
{
	p_option.imageTypeSel     = hs_combImgType.GetCurSel();

	p_option.chipStatusSel    = hs_combChipStatusGpio.GetCurSel();
	p_option.chipStatusLvlSel = hs_combChipBusyLvl.GetCurSel();
	p_option.chipResSel       = hs_combUpgradeResGpio.GetCurSel();
	p_option.chipResLvlSel    = hs_combUpgradeOkLvl.GetCurSel();

	p_option.flashModeSel     = hs_combFmemMode.GetCurSel();	

	p_option.clkCfgChkEn      = hs_chkClkCfgEn.GetCheck();
	p_option.ideBurnerChkEn   = hs_chkAndesBurner.GetCheck();

	hs_editExeAddr.GetWindowTextA(p_option.exeAddr, 10);
	hs_editSFClkCfgCPM.GetWindowTextA(p_option.cpmVal, 10);
	hs_editSFClkDiv.GetWindowTextA(p_option.sfVal, 10);
	//////////////////////////////////////////////////////////////////////////
	p_option.serialport        = hs_combUartDevice.GetCurSel();
	hs_editBaudrate.GetWindowTextA(p_option.baudrate ,10);
	p_option.packChkEn         = hs_CheckPackNeed.GetCheck();

	//////////////////////////////////////////////////////////////////////////
}


int hs_package::restoreDlg(void)
{
	hs_CheckPackNeed.SetCheck(p_option.packChkEn);

	hs_combImgType.SetCurSel(p_option.imageTypeSel);

	hs_combChipStatusGpio.SetCurSel(p_option.chipStatusSel);
	hs_combChipBusyLvl.SetCurSel(p_option.chipStatusLvlSel);
	hs_combUpgradeResGpio.SetCurSel(p_option.chipResSel);
	hs_combUpgradeOkLvl.SetCurSel(p_option.chipResLvlSel);

	hs_combFmemMode.SetCurSel(p_option.flashModeSel);

	hs_chkClkCfgEn.SetCheck(p_option.clkCfgChkEn);
	hs_chkAndesBurner.SetCheck(p_option.ideBurnerChkEn);

	hs_editExeAddr.SetWindowTextA(p_option.exeAddr);
	hs_editSFClkCfgCPM.SetWindowTextA(p_option.cpmVal);
	hs_editSFClkDiv.SetWindowTextA(p_option.sfVal);

	//////////////////////////////////////////////////////////////////////////
	hs_combUartDevice.SetCurSel(p_option.serialport);
	hs_editBaudrate.SetWindowTextA(p_option.baudrate);
	hs_editFileName.SetWindowTextA(p_option.w_packFile);
	hs_editCfgFile.SetWindowTextA(p_option.w_configFile);
	hs_editImgFile.SetWindowTextA(p_option.w_imageFile);
	hs_editOutFile.SetWindowTextA(p_option.w_outpackFile);
	
	OnBnClickedCheckSPack();
	

	hs_combBTAddrMode.SetCurSel(p_option.bt_address_change_mode);
	if(hs_combBTAddrMode.GetCurSel()==2&&strcmp(p_option.w_configFile,"")!=0)
		hs_editBTAddrEnd.EnableWindow(1);

#if 0
	if(strcmp(p_option.w_configFile,"")==0)
	{
		hs_editBTAddrEnd.EnableWindow(0);
		hs_editBTAddrStart.EnableWindow(0);
		hs_editBTName.EnableWindow(0);
		hs_combBTAddrMode.EnableWindow(0);
	}
#endif	

	hs_editBTAddrEnd.SetWindowTextA(p_option.bt_loop_change_num);
 	hs_editBTAddrStart.SetWindowTextA(p_option.bt_address);
	hs_editBTName.SetWindowTextA(p_option.bt_name);
	
	//////////////////////////////////////////////////////////////////////////

	return 0;
}

//////////////////////////////////////////////////////////////////////////
void hs_package::OnBnClickedButtonGetPackFile()
{
	// TODO: 在此添加控件通知处理程序代码

	
	char str[255];
	int tmp;

	CFileDialog dlg(TRUE, _T("*.*"), NULL, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("bin file (*.pack)|*.pack|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("select the files to be transmitted");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;// _T("e:");
	if(strcmp(p_option.w_packFileDir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = p_option.w_packFileDir;

	memset(p_option.w_packFile, 0, DIR_PATCH_LENGTH);

	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_packfile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editFileName.SetWindowText(hs_option.w_packfile);
		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_packFileDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
		memcpy(p_option.w_packFile ,hs_option.w_packfile,hs_option.w_packfile.GetLength());
		
	}
	else
	{
		this->hs_option.w_packfile = "";
		this->UpdateData(FALSE);
		this->hs_editFileName.SetWindowText(hs_option.w_packfile);
	}
	saveDlg();
}


DWORD WINAPI StatusThreadProc_s(LPVOID lpParam)
{
	hs_package *pThis = (hs_package *)lpParam;
	hs_backcall_para *pStatus = &pThis->hs_status;
	char str[255];
	unsigned int wtime = 0x7f00000;

	while(wtime--)
	{
		if(pStatus->totalCnt > 0)
			break;
	}

#ifndef _DEBUG
	pThis->UpdateData(false);
#endif
	pThis->hs_s_statusProcess.SetRange(0, pStatus->totalCnt);

	while(1)
	{
#ifndef _DEBUG
		pThis->UpdateData(false);
#endif

		sprintf_s(str, "%d", pThis->hs_stTransPara.fileLen);
		pThis->hs_s_statusFileSize.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->totalCnt);
		pThis->hs_s_statusTotalCnt.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->successCnt);
		pThis->hs_s_statusSuccessCnt.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->totalErrCnt);
		pThis->hs_s_statusTotalErrCnt.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->lastErrIdx);
		pThis->hs_s_statusLastErrIdx.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->lastErrCnt);
		pThis->hs_s_statusLastErrCnt.SetWindowTextA(_T(str));

		pThis->hs_s_statusProcess.SetPos(pStatus->successCnt);		

		if(pThis->bTransFlg == false)
			break;

		Sleep(50);
	}

	return 0;
}

DWORD WINAPI TransmitThreadProc_s(LPVOID lpParam)
{
	bool prepare_download=true;
	hs_package *pThis = (hs_package *)lpParam;
	char str[255];
	FILE *pBinFile, *packinfo;
	errno_t fres, fres_addr_file;
	unsigned int speed;
	uint32_t pack_version;
	unsigned char address_org_show[6];
	unsigned char address_show[6];
	unsigned char name_show[16];
	char str_show[256], str_name[64];
	FILE *pAddressFile;
	uint32_t wait_index;
	uint32_t wait_index_max;
	char addr_new_show[256];
	unsigned char addr_file_buf[12], address_new[6];
	uint32_t address_change_mode_rw;
	uint32_t name_change_mode_rw;
	unsigned char address_rw[6];
	unsigned char address_org_rw[6];
	unsigned char name_rw[16];
	uint32_t address_H3,address_L3, *pH,*pL;
	uint32_t address_increase_loop_count;
	/*  Try to open packed file  */
	char file_boot_download[256]={0};
	strcpy(file_boot_download,file_boot);
	strcat(file_boot_download, PACK_FILE);
	pThis->hs_option.w_packfile = file_boot_download;
	fres = fopen_s(&pBinFile, pThis->hs_option.w_packfile, "rb" );
	// open packed file error
	if(fres)
	{
		pThis->MessageBox(_T("Transmit Thread Open packed(To Download)file failed!"),_T("Transger Error"),MB_ICONHAND|MB_OK);
		pThis->bTransFlg = false;
		return 0;
	}
	//  Have Open it
	if(pThis->bTransFlg)
	{
		pThis->hs_bnDownload.SetWindowTextA("Stop");
		pThis->hs_CheckPackNeed.EnableWindow(0);
	}
	 // Get the Pack to Tranf_Buffer
	pThis->hs_stTransPara.fileLen = getfilelen_s(pBinFile);
	pThis->hs_stTransPara.pBufPtr = (unsigned char *)malloc(pThis->hs_stTransPara.fileLen);
	fread((void *)pThis->hs_stTransPara.pBufPtr, 1, pThis->hs_stTransPara.fileLen, pBinFile);
	fclose(pBinFile);
	
	/************************************************************************************/
	//  read the pack_load_info
	fres = fopen_s(&packinfo, pThis->hs_option.w_packfile+(CString)("_info.dat"), "rb" );
	if(fres)
	{
		pThis->MessageBox(_T("Open packInfo (Read) file failed!"),_T("Open packInfo Error"),MB_ICONHAND|MB_OK);
		return 0;
	}
	
	fread((void*)&pThis->p_option.pack_load_record,1,sizeof(pack_load_record_t),packinfo);
	fclose(packinfo);



	/* Show Pack Information */

	// 0 Show pack version

	char string[25];
	itoa(pThis->p_option.pack_load_record.pack_version,string,10);

	pThis->hs_editShowPackVersion.SetWindowTextA(string);

	// 1 show address ORG
	pThis->hs_editShowAddrOrg.SetWindowTextA(pThis->p_option.pack_load_record.pack_bt_address_org); 

	// 2  show address for This time
	if(pThis->p_option.pack_load_record.pack_addr_change_mod!=3)
	{
		memcpy(address_show,(void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset), 6);
		sprintf(str_show, "%02X : %02X : %02X : %02X : %02X : %02X", address_show[5],address_show[4], address_show[3], address_show[2],address_show[1], address_show[0]);
		pThis->hs_editShowAddress.SetWindowTextA(_T(str_show));	
	}

	// 3 Show the address change mode
	switch(pThis->p_option.pack_load_record.pack_addr_change_mod)
	{
	case 0: pThis->hs_editShowAddrMode.SetWindowTextA(_T("Fixed"));
		pThis->hs_editGetAddrFile.EnableWindow(0);
		pThis->hs_bnGetAddrFile.EnableWindow(0);
		break;
	case 1: pThis->hs_editShowAddrMode.SetWindowTextA(_T("Increased"));
		pThis->hs_editGetAddrFile.EnableWindow(0);
		pThis->hs_bnGetAddrFile.EnableWindow(0);
		break;
	case 2: pThis->hs_editShowAddrMode.SetWindowTextA(_T("Increased and Loop"));
		pThis->hs_editGetAddrFile.EnableWindow(0);
		pThis->hs_bnGetAddrFile.EnableWindow(0);
		break;
	case 3: pThis->hs_editShowAddrMode.SetWindowTextA(_T("File List"));
		pThis->hs_editGetAddrFile.EnableWindow(1);
		pThis->hs_bnGetAddrFile.EnableWindow(1);
		if(strcmp(pThis->hs_option.w_addrfile,"")==0)
		{
			pThis->MessageBox("Need You To Input Address File","Note");
			prepare_download=false;
			break;
		}
		fres_addr_file = fopen_s(&pAddressFile, pThis->hs_option.w_addrfile, "rb" );
		// open address error
		if(fres_addr_file)
		{
			pThis->MessageBox(_T("Open Address(To Download)file Failed!"),_T("Open ADDRFILE Error"),MB_ICONHAND|MB_OK);
			prepare_download=false;
			break;
		}
		//  Have open Address File
		if(new_pack==true)
		{
			new_pack=false;
		}
		// . get the address index
		fread((void *)addr_file_buf, sizeof(char), 12, pAddressFile);
		wait_index=getAddrIndex(addr_file_buf);
		fseek(pAddressFile,2,SEEK_CUR);
		fread((void *)addr_file_buf, sizeof(char), 12, pAddressFile);
		wait_index_max=getAddrIndex(addr_file_buf);
		// .. read the address buf
		rewind(pAddressFile);
		fseek(pAddressFile,14*(wait_index+1),SEEK_CUR);
		fread((void *)addr_file_buf, sizeof(char), 12, pAddressFile);

		// ... turn address and write it to transf_data_buf
		getNewAddr(addr_file_buf,address_new);
		memcpy((void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset), address_new,6); 
		sprintf(addr_new_show, "%02X : %02X : %02X : %02X : %02X : %02X", address_new[5],address_new[4], address_new[3], address_new[2],address_new[1], address_new[0]);
		pThis->hs_editShowAddress.SetWindowTextA(_T(addr_new_show));	 // show address for This time From File
		fclose(pAddressFile);	
		break;
	default:break;
	}

	// 4 Show the original name
	pThis->hs_editShowName.SetWindowTextA(pThis->p_option.pack_load_record.pack_bt_name_org);
	if(prepare_download==false)
		goto END;
	

	//*************　Start To Download
	pThis->enTransRes = pThis->hs_comm.startDownload(pThis->hs_stTransPara.pBufPtr, pThis->hs_stTransPara.fileLen, &speed, &pThis->hs_status);
	pThis->bTransFlg=false;
	if(RES_OK == pThis->enTransRes)
	{
		// 下.载成功


		//IF  The result of Download is OK
		//-------------------------------------- CHANGE OPREATION  -----------------------------------
		

		/*
		*  read pack and rewrite back to pack based the address_change_mode and name_change_mode
		*/
		
		
		/************************************************************************/
		/* The next will change address and the name if needed                      */
		/************************************************************************/
		 address_change_mode_rw=pThis->p_option.pack_load_record.pack_addr_change_mod;
		 switch(address_change_mode_rw)
		 {

		 case 0:/*Fixed*/
			 break;
		 case 1:/*Increase*/
			 pH=&address_H3;
			 pL=&address_L3;
			 memcpy(address_rw,(void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset), 6); 
			 bytesTOuintHL(address_rw,pH,pL);
			 if(address_L3==16777215)
			 {
				 address_H3+=1;
				 address_L3=0;
			 }
			 else if(address_H3==16777215&&address_L3==16777215)
			 {
				 address_L3=0;
				 address_H3=0;
			 }
			 else
			 {
				 address_L3+=1;
			 }
			 uintHLTObytes(address_rw,pH,pL);
			 memcpy((void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset),address_rw,6); 
			 fres = fopen_s(&pBinFile, pThis->hs_option.w_packfile, "rb+" );
			 // open packed file error
			 if(fres)
			 {
				 pThis->MessageBox(_T("Open packed file For Write Back  Failed!"),_T("Transger Error"),MB_ICONHAND|MB_OK);
				 return 0;
			 }
			 fwrite((void *)pThis->hs_stTransPara.pBufPtr, 1, pThis->hs_stTransPara.fileLen, pBinFile);
			 fclose(pBinFile);
			 break;

		 case 2:/*Loop Increase*/
			 pH=&address_H3;
			 pL=&address_L3;
			 memcpy(address_rw,(void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset), 6); 
			 bytesTOuintHL(address_rw,pH,pL);
			 if(pThis->p_option.pack_load_record.pack_addr_loop_position<pThis->p_option.pack_load_record.pack_addr_loop_num-1)
			 {
				pThis->p_option.pack_load_record.pack_addr_loop_position+=1;
				if(address_L3==16777215)
				{
					address_H3+=1;
					address_L3=0;
				}
				else if(address_H3==16777215&&address_L3==16777215)
				{
					address_L3=0;
					address_H3=0;
				}
				else
				{
					address_L3+=1;
				}
				uintHLTObytes(address_rw,pH,pL);
				memcpy((void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset),address_rw,6); 
			 }
			 else
			 {
 				 pThis->p_option.pack_load_record.pack_addr_loop_position=0;
				 char start_org[6], address_original[256];
				 memcpy(address_original,pThis->p_option.pack_load_record.pack_bt_address_org, 256);
				 parseStrForBtAddr(address_original, (unsigned char *)start_org);
				 memcpy((void*)(pThis->hs_stTransPara.pBufPtr+pThis->p_option.pack_load_record.addr_offset),start_org,6); 

			 }

			 fres = fopen_s(&pBinFile, pThis->hs_option.w_packfile, "rb+" );
			 // open packed file error
			 if(fres)
			 {
				 pThis->MessageBox(_T("Open packed file For Write Back  Failed!"),_T("Transger Error"),MB_ICONHAND|MB_OK);
				 return 0;
			 }
			 fwrite((void *)pThis->hs_stTransPara.pBufPtr, 1, pThis->hs_stTransPara.fileLen, pBinFile);
			 fclose(pBinFile);
			 break;

		 case 3:/* Base on file*/
		
		     fres_addr_file = fopen_s(&pAddressFile, pThis->hs_option.w_addrfile, "rb+" );
			 // open address file error
			 if(fres_addr_file)
			 {
				 pThis->MessageBox(_T("Open Address(To Download)file Failed!"),_T("Open ADDRFILE Error"),MB_ICONHAND|MB_OK);
				 return 0;
			 }
			 //  Have open Address File

			 // 4 address_index ++
			 wait_index+=1;
			 if(wait_index>wait_index_max)
			 {
				 wait_index=1;
			 }
			 turnAddrIndex(addr_file_buf,wait_index);
			 rewind(pAddressFile);
			 fwrite((void *)addr_file_buf, sizeof(char), 12, pAddressFile);		 
			 fclose(pAddressFile);
			 break;
		 default:break;
		 }

		 //  write back the load info
		fres = fopen_s(&packinfo, pThis->hs_option.w_packfile+(CString)("_info.dat"), "wb+" );
		if(fres)
		{
		 	pThis->MessageBox(_T("Open packInfo (Write) file failed!"),_T("Open packInfo Error"),MB_ICONHAND|MB_OK);
		return 0;
		}
		fwrite((void*)&pThis->p_option.pack_load_record,1,sizeof(pack_load_record_t),packinfo);
		fclose(packinfo);
	    //------------------------------------------------------------------

		pThis->MessageBox(_T("Transmite Successfully"),_T("Transmite Okey"),MB_ICONEXCLAMATION|MB_OK);
	}

	else
	{   
		//   下.载失败
		if(pThis->enTransRes == RES_ERR_CANCEL)
			sprintf_s(str,"Receiver terminated the transfer!");
		else
			sprintf_s(str,"File transmit error: %d", pThis->enTransRes);

		pThis->MessageBox(_T(str),_T("Transger error"),MB_ICONHAND|MB_OK);
	}
END:
    pThis->saveDlg();
    pThis->UartConnect(FALSE);
    free(pThis->hs_stTransPara.pBufPtr);
	pThis->hs_stTransPara.pBufPtr = NULL;
	pThis->hs_CheckPackNeed.EnableWindow(1);
	pThis->hs_bnDownload.SetWindowTextA(_T("DownLoad"));
	if(prepare_download==false)
	{
		pThis->bTransFlg=false;
		pThis->UartConnect(false);
	}
	return 0;
}

void hs_package::Download_Start()
{
	if(need_pack)
	{
		hs_option.w_packfile=hs_option.w_outputfile;

	}
	else
	{
		char file_boot_download[256]={0};
		strcpy(file_boot_download,file_boot);
		strcat(file_boot_download, PACK_FILE);
		hs_option.w_packfile = file_boot_download;
	}
	if(strcmp(hs_option.w_packfile, "") == 0)
	{
		MessageBox(_T("Packed Bin file(To Download) path invalidate!"), _T("Transger Error"), MB_ICONHAND|MB_OK);
		return ;
	}

	if(bConnectFlg == false)
	{
		MessageBox(_T("The communiction have not connected!"),_T("Transger Error"),MB_ICONHAND|MB_OK);
		return ;
	}	

	memset(&hs_status, 0, sizeof(hs_backcall_para));
	hThread[0] = CreateThread(NULL, 0, StatusThreadProc_s, (LPVOID)this, 0, &dwThreadId[0]);
	hThread[1] = CreateThread(NULL, 0, TransmitThreadProc_s, (LPVOID)this, 0, &dwThreadId[1]);	
	bTransFlg=true;

}

void hs_package::Download_Stop()
{
	if(bTransFlg==false)
		return;
	TerminateThread(hThread[1], 0);

	if(hs_stTransPara.pBufPtr != NULL)
	{
		free(hs_stTransPara.pBufPtr);
		hs_stTransPara.pBufPtr = NULL;
	}

	MessageBox(_T("File transmit terminated!"), _T("Transger Error"), MB_ICONHAND|MB_OK);
	bTransFlg=false;
	if(bTransFlg==false)
		hs_bnDownload.SetWindowTextA(_T("DownLoad"));
}

//////////////////////////////////////////////////////////////////////////

void hs_package::OnBnClickedCheckSPack()
{
	// TODO: 在此添加控件通知处理程序代码
	if(hs_CheckPackNeed.GetCheck())
	{
		need_pack=true;
		hs_bnSelectFile.EnableWindow(0);
		hs_editFileName.EnableWindow(0);

		hs_combImgType.EnableWindow(0);
		hs_combChipStatusGpio.EnableWindow(0);
		hs_combChipBusyLvl.EnableWindow(0);
		hs_combUpgradeResGpio.EnableWindow(0);
		hs_combUpgradeOkLvl.EnableWindow(0);

		hs_combFmemMode.EnableWindow(0);

		hs_editImgFile.EnableWindow(1);
		hs_editCfgFile.EnableWindow(1);
		hs_editOutFile.EnableWindow(1);
		hs_getImagefile.EnableWindow(1);
		hs_outPackfile.EnableWindow(1);
		hs_bnSelCfg.EnableWindow(1);

		hs_chkClkCfgEn.EnableWindow(0);
		hs_chkAndesBurner.EnableWindow(0);

		hs_editCfgFlashAddr.EnableWindow(1);
		hs_combBTAddrMode.EnableWindow(1);
		hs_editBTAddrStart.EnableWindow(1);
		hs_editBTName.EnableWindow(1);

		if(hs_combBTAddrMode.GetCurSel()==2)
			hs_editBTAddrEnd.EnableWindow(1);
		
		//  将config.ini读出并写入临时文件
		//  并使能 config 已读的标志位
		//  显示config 的相关地址 名字信息
		readConfig();
	}
	else
	{
		need_pack=false;
		hs_editFileName.EnableWindow(1);
		hs_bnSelectFile.EnableWindow(1);
		hs_combImgType.EnableWindow(0);
		hs_combChipStatusGpio.EnableWindow(0);
		hs_combChipBusyLvl.EnableWindow(0);
		hs_combUpgradeResGpio.EnableWindow(0);
		hs_combUpgradeOkLvl.EnableWindow(0);
		hs_editBTName.EnableWindow(0);
		hs_editBTAddrStart.EnableWindow(0);
		hs_combFmemMode.EnableWindow(0);
		hs_editImgFile.EnableWindow(0);
		hs_editCfgFile.EnableWindow(0);
		hs_editOutFile.EnableWindow(0);
		hs_getImagefile.EnableWindow(0);
		hs_outPackfile.EnableWindow(0);
		hs_bnSelCfg.EnableWindow(0);
		hs_chkClkCfgEn.EnableWindow(0);
		hs_chkAndesBurner.EnableWindow(0);
		hs_editBTAddrStart.EnableWindow(0);
		hs_combBTAddrMode.EnableWindow(0);
		hs_editBTName.EnableWindow(0);
		hs_editBTAddrEnd.EnableWindow(0);
		hs_editCfgFlashAddr.EnableWindow(0);
	}
	saveDlg();
}



void hs_package::OnBnClickedButtonGetAddrFile()
{
	// TODO: 在此添加控件通知处理程序代码
	char str[255];
	int tmp;

	CFileDialog dlg(FALSE, _T("*.*"), NULL, 0, _T("info Files (*.txt)|*.txt|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("Get Address file");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;//_T("e:");
	if(strcmp(p_option.w_addrCurDir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = p_option.w_addrCurDir;

	memset(p_option.w_addrFile, 0, DIR_PATCH_LENGTH);

	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_addrfile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editGetAddrFile.SetWindowText(this->hs_option.w_addrfile);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_addrCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
		memcpy(p_option.w_addrFile ,(LPSTR)(LPCTSTR) dlg.GetPathName(),hs_option.w_addrfile.GetLength());
	}
	else
	{
		this->hs_option.w_addrfile = "";
		this->UpdateData(FALSE);
		this->hs_editGetAddrFile.SetWindowText(this->hs_option.w_addrfile);
	}

	saveDlg();
}


void hs_package::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MessageBox(_T("To Clear The Dlg Option?"),_T("Clear DlgOption"),MB_ICONEXCLAMATION|MB_YESNO)==IDYES)
	{
		memset((void*)&p_option,0,sizeof(dlg_option));
		saveDlg();
	}
	else
		return;

	
}


/***********************************************************************/
void hs_package::compileConfigPos(uint32_t cfg_addr)
{
	//将 临时文件放入缓存队列中
	FILE * fconfig;
	errno_t fres;
	fres = fopen_s(&fconfig, hs_option.w_configfile+(CString)".pack", "rb");
	if (fres)
	{
		//MessageBox(_T("Open config bin file failed!"), _T("infoPacking Warning"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	memset(hs_option.pInfoDataBuf, 0xff, CONFIG_LEN);
	rewind(fconfig);
	hs_option.cfgBinLen = getfilelen_s(fconfig);
	fread((void *)(hs_option.pInfoDataBuf+cfg_addr), 1, hs_option.cfgBinLen, fconfig);	
	hs_option.cfgBinLen += cfg_addr;
	hs_option.cfgBinLen = (hs_option.cfgBinLen + CFG_BLOCK_SIZE - 1) & CFG_BLOCK_ALIGN_MASK;
	fclose(fconfig);
	DeleteFile( hs_option.w_configfile+(CString)".pack");	
}


void hs_package::readConfig(void)
{
	/*
	read the .ini to config.pack(临.时文件)
	*/
	FILE * fconfig;
	errno_t fres;
	unsigned char readBuf[100];
	char str[256];
	char file_boot_config[256] = { 0 };
	strcpy(file_boot_config, file_boot);
	strcat(file_boot_config, CONFIG_FILE);
	hs_option.w_configfile = file_boot_config;
	if (CopyPosImage(hs_option.w_configfile, hs_option.w_configfile+(CString)".pack") < 0)
    {
		//MessageBox(_T("copy packed file failed!"), _T("infoPacking Warning"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	fres = fopen_s(&fconfig, hs_option.w_configfile+(CString)".pack", "rb");
	if (fres)
	{
		//MessageBox(_T("Open config bin file failed!"), _T("infoPacking Warning"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	hs_packinfo.u32CfgDataEn=1;
	//show addr
	fread_s(readBuf, sizeof(readBuf), 1, 50, fconfig);
	sprintf_s(str, "%02X : %02X : %02X : %02X : %02X : %02X", readBuf[5], readBuf[4], readBuf[3], readBuf[2], readBuf[1], readBuf[0]);
	hs_editBTAddrStart.SetWindowTextA(str);

	UTF8ToGBK(readBuf+6, (unsigned char *)str, 64);
	hs_editBTName.SetWindowTextA(_T(str));  //  show name

	fclose(fconfig);

}