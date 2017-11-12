// hs_package.cpp : implementation file
//

#include "stdafx.h"
#include "HToolC.h"
#include "hs_package.h"
#include "afxdialogex.h"
#include "comm_protocal.h"
#include "HToolCDlg.h"



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
}


BEGIN_MESSAGE_MAP(hs_package, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_PACKAGE, &hs_package::OnBnClickedButtonPackage)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_IMAGE, &hs_package::OnBnClickedButtonInputImage)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_CONFIG, &hs_package::OnBnClickedButtonInputConfig)
	ON_BN_CLICKED(IDC_BUTTON_PACKED_BIN, &hs_package::OnBnClickedButtonPackedBin)
	ON_CBN_SELCHANGE(IDC_COMBO_BTADDR_CHANGE_MODE, &hs_package::OnCbnSelchangeComboBtaddrChangeMode)
	ON_BN_CLICKED(IDC_CHECK_SF_CLOCK_CFG, &hs_package::OnBnClickedCheckSfClockCfg)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE_TYPE, &hs_package::OnCbnSelchangeComboImageType)
END_MESSAGE_MAP()


// hs_package message handlers
BOOL hs_package::OnInitDialog(void)
{
	char str[256];
	int i;

	CDialogEx::OnInitDialog();	

	hs_combImgType.InsertString(0, _T("Upgrade Flash"));
	hs_combImgType.InsertString(1, _T("Loading and Execute"));
	hs_combImgType.SetCurSel(0);

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
	hs_combBTAddrMode.SetCurSel(1);
	
	hs_editBTAddrStart.SetWindowTextA(_T("00 : 00 : 00 : 00 : 00 : 00"));
	hs_editBTAddrEnd.SetWindowTextA(_T("1000000"));
	hs_editBTAddrStart.EnableWindow(0);
	hs_editBTAddrEnd.EnableWindow(0);
	hs_combBTAddrMode.EnableWindow(0);
	hs_editBTName.EnableWindow(0);

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

	return true;
}

int getFileLen(FILE *fp)
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
	FILE *inCodeBin, *outBinFile; 
	CWaitCursor hWaitCursor;
	errno_t fres;
	unsigned int dataTemp;
	char *ptr, str[256], *pCodeBuf;
	CTime cTime;
	CString cStr;
	
	if(strcmp(hs_option.w_imagefile, "") == 0)
	{
		MessageBox(_T("Input bin file path invalidate!"), _T("infoPacking Error"), MB_ICONEXCLAMATION|MB_OK);
		return ;
	}

	fres = fopen_s(&inCodeBin, hs_option.w_imagefile, "rb" );
	if(fres)
	{
		MessageBox(_T("Open bin file failed!"),_T("infoPacking Warning"),MB_ICONEXCLAMATION|MB_OK);
		return ;
	}

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
	hs_packinfo.u32ImageLen = getFileLen(inCodeBin);
	hs_packinfo.u32StatusIO = hs_combChipStatusGpio.GetCurSel();
	hs_packinfo.u32StatusIOLvl = hs_combChipBusyLvl.GetCurSel();
	hs_packinfo.u32ResultIO = hs_combUpgradeResGpio.GetCurSel();
	hs_packinfo.u32ResultIOLvl = hs_combUpgradeOkLvl.GetCurSel();
	hs_packinfo.u32KeyAddr = KEY_ADDR;
	hs_packinfo.u32InfoChkSum = 0;	

	unsigned int u32ImgPackLen;
	u32ImgPackLen = (hs_packinfo.u32ImageLen + CFG_BLOCK_SIZE - 1) & CFG_BLOCK_ALIGN_MASK;
	u32ImgPackLen += INFO_SIZE;

	pCodeBuf = (char *)malloc(u32ImgPackLen);
	memset((void *)pCodeBuf, 0, u32ImgPackLen);

	fread((void *)pCodeBuf, 1, hs_packinfo.u32ImageLen, inCodeBin);	

	if(hs_packinfo.u32CfgDataEn)
	{
		hs_packinfo.u32BTAddrOffset = u32ImgPackLen + getDataOffset(HS_CFG_BT_HC, (unsigned char *)hs_option.pInfoDataBuf);
		hs_packinfo.u32BTAddrChgMode = hs_combBTAddrMode.GetCurSel();
		hs_packinfo.u32ChgModeArgc0 = 0;
		if(hs_packinfo.u32BTAddrChgMode == 2)
		{
			hs_editBTAddrEnd.GetWindowTextA(str, 256);
			sscanf(str, "%d", &hs_packinfo.u32ChgModeArgc1);
		}

		configDataUpdate();
	}
	else
	{
		hs_option.cfgBinLen = 0;
	}

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
	{
		fwrite((void *)&hs_packinfo, 1, INFO_SIZE, outBinFile);
		totalLen = INFO_SIZE;
	}

	fwrite((void *)pCodeBuf, 1, hs_pImgInfo->u32ImageLen, outBinFile);
	totalLen += hs_pImgInfo->u32ImageLen;

	if(hs_packinfo.u32CfgDataEn)
	{
		fwrite((void *)hs_option.pInfoDataBuf, 1, hs_option.cfgBinLen, outBinFile);
		totalLen += hs_option.cfgBinLen;
	}

	memset((void *)pCodeBuf, 0, u32ImgPackLen);
	u32ImgPackLen = hs_packinfo.u32ImageLen + INFO_SIZE - (u32ImgPackLen + hs_option.cfgBinLen);
	fwrite((void *)pCodeBuf, 1, u32ImgPackLen, outBinFile);
	totalLen += u32ImgPackLen;

	fclose(inCodeBin);
	fclose(outBinFile);

	free(pCodeBuf);

	sprintf(str, "Packing OK!\r\n\r\nTotal length: %d KBytes.", (totalLen / 1024));
	MessageBox(_T(str), _T("Packing OK"), MB_ICONINFORMATION|MB_OK);

	saveDlg();
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

	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_imagefile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editImgFile.SetWindowText(this->hs_option.w_imagefile);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_imageCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
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
	char str[255];
	int tmp;

	CFileDialog dlg(TRUE, _T("*.*"), NULL, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("info Files (*.ini)|*.ini|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("Select config file");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;
	if(strcmp(p_option.w_configCurDir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = p_option.w_configCurDir;

	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_configfile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editCfgFile.SetWindowText(this->hs_option.w_configfile);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_configCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);

		compileConfig();
	}
	else
	{
		this->hs_option.w_configfile = "";
		this->UpdateData(FALSE);
		this->hs_editCfgFile.SetWindowText(this->hs_option.w_configfile);

		hs_packinfo.u32CfgDataEn = 0;
		hs_option.cfgBinLen = 0;
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

	if( dlg.DoModal() == IDOK )
	{
		this->hs_option.w_outputfile = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editOutFile.SetWindowText(this->hs_option.w_outputfile);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(p_option.w_packCurDir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
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
	unsigned int btaddr_offset;
	unsigned char *dbptr;
	char gbkname[64], str[256];

	if(hs_packinfo.u32CfgDataEn)
	{
		btaddr_offset = getDataOffset(HS_CFG_BT_DEV, (unsigned char *)hs_option.pInfoDataBuf);
		dbptr = (unsigned char *)(hs_option.pInfoDataBuf + btaddr_offset);
		UTF8ToGBK(dbptr, (unsigned char *)gbkname, 64);
		hs_editBTName.SetWindowTextA(_T(gbkname));

		btaddr_offset = getDataOffset(HS_CFG_BT_HC, (unsigned char *)hs_option.pInfoDataBuf);
		dbptr = (unsigned char *)(hs_option.pInfoDataBuf + btaddr_offset);
		sprintf(str, "%02X : %02X : %02X : %02X : %02X : %02X", dbptr[5], dbptr[4], dbptr[3], dbptr[2], dbptr[1], dbptr[0]);
		hs_editBTAddrStart.SetWindowTextA(_T(str));	

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

			if(hs_packinfo.u32CfgDataEn)
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
	else
	{
		hs_combBTAddrMode.EnableWindow(0);
		hs_editBTName.EnableWindow(0);
		hs_editBTAddrStart.EnableWindow(0);
		hs_editBTAddrEnd.EnableWindow(0);
	}

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
		hs_packinfo.u32CfgDataEn = 0;
		hs_packinfo.u32BTAddrOffset = 0xffffffff;
		return ;
	}

	hs_packinfo.u32CfgDataEn = 1;
	fres = fopen_s(&inCfgFile, hs_option.w_configfile, "rb" );
	if(fres)
	{
		hs_packinfo.u32CfgDataEn = 0;
		hs_packinfo.u32BTAddrOffset = 0xffffffff;
		return ;
	}

	/* begin to parse the config text */
	memset(hs_option.pInfoDataBuf, 0, CONFIG_LEN);
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

void hs_package::configDataUpdate(void)
{
	unsigned int btaddr_offset;
	char start[6], name[16], str[256], *ptr;
	int i, f;

	hs_editBTAddrStart.GetWindowTextA(str, 256);
	parseStrForBtAddr(str, (unsigned char *)start);
	btaddr_offset = getDataOffset(HS_CFG_BT_HC, (unsigned char *)hs_option.pInfoDataBuf);
	ptr = hs_option.pInfoDataBuf + btaddr_offset;
	memcpy(ptr, start, 6);

	hs_editBTName.GetWindowTextA(str, 256);	
	btaddr_offset = getDataOffset(HS_CFG_BT_DEV, (unsigned char *)hs_option.pInfoDataBuf);
	ptr = hs_option.pInfoDataBuf + btaddr_offset;
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
}


int hs_package::restoreDlg(void)
{
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

	if(p_option.imageTypeSel)
		hs_editExeAddr.EnableWindow(1);
	else
		hs_editExeAddr.EnableWindow(0);

	if(p_option.clkCfgChkEn)
	{
		hs_editSFClkCfgCPM.EnableWindow(1);
		hs_editSFClkDiv.EnableWindow(1);
	}
	else
	{
		hs_editSFClkCfgCPM.EnableWindow(0);
		hs_editSFClkDiv.EnableWindow(0);
	}

	return 0;
}
