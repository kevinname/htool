// comm_config.cpp : implementation file
//

#include "stdafx.h"
#include "HToolC.h"
#include "comm_config.h"
#include "afxdialogex.h"


// comm_config dialog

IMPLEMENT_DYNAMIC(comm_config, CDialogEx)

comm_config::comm_config(CWnd* pParent /*=NULL*/)
	: CDialogEx(comm_config::IDD, pParent)
{
	
}

comm_config::~comm_config()
{
	bTransFlg = false;

	TerminateThread(hThread[1], 0);

	hs_comm.hs_close();
}

void comm_config::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UART_DEVICE, hs_combUartDevice);
	DDX_Control(pDX, IDC_EDIT_BAUDRATE, hs_editBaudrate);
	DDX_Control(pDX, IDC_BUTTON_UART_CONNECT, hs_bnUartConnect);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, hs_editFileName);
	DDX_Control(pDX, IDC_BUTTON_GET_FILE, hs_bnSelectFile);
	DDX_Control(pDX, IDC_BUTTON_START_TRANSFER, hs_bnStartTransfer);
	DDX_Control(pDX, IDC_EDIT1, hs_statusFileSize);
	DDX_Control(pDX, IDC_EDIT4, hs_statusTotalCnt);
	DDX_Control(pDX, IDC_EDIT2, hs_statusSuccessCnt);
	DDX_Control(pDX, IDC_EDIT5, hs_statusTotalErrCnt);
	DDX_Control(pDX, IDC_EDIT3, hs_statusLastErrIdx);
	DDX_Control(pDX, IDC_EDIT6, hs_statusLastErrCnt);
	DDX_Control(pDX, IDC_PROGRESS1, hs_statusProcess);
	DDX_Control(pDX, IDC_BUTTON_STOP_TRANSFER, hs_bnStopTransfer);
}


BEGIN_MESSAGE_MAP(comm_config, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UART_CONNECT, &comm_config::OnBnClickedButtonUartConnect)
	ON_BN_CLICKED(IDC_BUTTON_START_TRANSFER, &comm_config::OnBnClickedButtonStartTransfer)
	ON_BN_CLICKED(IDC_BUTTON_GET_FILE, &comm_config::OnBnClickedButtonGetFile)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TRANSFER, &comm_config::OnBnClickedButtonStopTransfer)
END_MESSAGE_MAP()


// comm_config message handlers


BOOL comm_config::OnInitDialog(void)
{
	CDialogEx::OnInitDialog();
	long i, index, res;
	char devName[32];

	hs_comm.hs_slelectChannel(HS_COMMTYPE_UART);
	
	index = 0;
	for(i=0; i<51; i++)
	{		
		sprintf_s(devName,"\\\\.\\com%d", i);
		res = hs_comm.hs_open(devName);
		if(!res)
		{			
			strcpy_s(uartDevList[index], devName);

			sprintf_s(devName,"com%d", i);

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

	this->hs_editFileName.EnableWindow(0);
	this->hs_bnStartTransfer.EnableWindow(0);
	this->hs_bnSelectFile.EnableWindow(0);
	this->hs_bnStopTransfer.EnableWindow(0);

	memset(w_curdir, 0, DIR_PATCH_LENGTH);

	return true;
}


void comm_config::OnBnClickedButtonUartConnect()
{
	long res;
	unsigned int index, dataTemp;
	hs_uart_para uartPara;
	char str[20];

	if(bConnectFlg == false)
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

		this->hs_combUartDevice.EnableWindow(0);
		this->hs_editBaudrate.EnableWindow(0);

		this->hs_bnUartConnect.SetWindowTextA(_T("disconnect"));

		this->hs_editFileName.EnableWindow(1);
		this->hs_bnStartTransfer.EnableWindow(1);
		this->hs_bnSelectFile.EnableWindow(1);

		bConnectFlg = true;
	}
	else
	{
		hs_comm.hs_close();
		hs_comm.hs_slelectChannel(HS_COMMTYPE_NODEV);

		this->hs_combUartDevice.EnableWindow(1);
		this->hs_editBaudrate.EnableWindow(1);

		this->hs_bnUartConnect.SetWindowTextA(_T("connect"));

		this->hs_editFileName.EnableWindow(0);
		this->hs_bnStartTransfer.EnableWindow(0);
		this->hs_bnSelectFile.EnableWindow(0);

		bConnectFlg = false;
	}
}

static int getFileLen(FILE *fp)
{
	int temp,len;

	temp = ftell(fp);
	fseek(fp,0,SEEK_END);	
	len = ftell(fp);
	fseek(fp,temp,0);	
	
	return len;
}

DWORD WINAPI StatusThreadProc(LPVOID lpParam)
{
	comm_config *pThis = (comm_config *)lpParam;
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
	pThis->hs_statusProcess.SetRange(0, pStatus->totalCnt);

	while(1)
	{
		#ifndef _DEBUG
		pThis->UpdateData(false);
		#endif

		sprintf_s(str, "%d", pThis->hs_stTransPara.fileLen);
		pThis->hs_statusFileSize.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->totalCnt);
		pThis->hs_statusTotalCnt.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->successCnt);
		pThis->hs_statusSuccessCnt.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->totalErrCnt);
		pThis->hs_statusTotalErrCnt.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->lastErrIdx);
		pThis->hs_statusLastErrIdx.SetWindowTextA(_T(str));

		sprintf_s(str, "%d", pStatus->lastErrCnt);
		pThis->hs_statusLastErrCnt.SetWindowTextA(_T(str));

		pThis->hs_statusProcess.SetPos(pStatus->successCnt);		

		if(pThis->bTransFlg == false)
			break;

		Sleep(50);
	}

	return 0;
}

DWORD WINAPI TransmitThreadProc(LPVOID lpParam)
{
	comm_config *pThis = (comm_config *)lpParam;
	char str[255];
	FILE *pBinFile;
	errno_t fres;
	unsigned int speed;

	fres = fopen_s(&pBinFile, pThis->hs_file, "rb" );
	if(fres)
	{
		pThis->MessageBox(_T("Open output packed file failed!"),_T("Transger Error"),MB_ICONHAND|MB_OK);
		pThis->bTransFlg = false;
		return 0;
	}

	pThis->hs_stTransPara.fileLen = getFileLen(pBinFile);
	pThis->hs_stTransPara.pBufPtr = (unsigned char *)malloc(pThis->hs_stTransPara.fileLen);

	fread((void *)pThis->hs_stTransPara.pBufPtr, 1, pThis->hs_stTransPara.fileLen, pBinFile);	
	fclose(pBinFile);

	pThis->enTransRes = pThis->hs_comm.startDownload(pThis->hs_stTransPara.pBufPtr, pThis->hs_stTransPara.fileLen, &speed, &pThis->hs_status);

	pThis->hs_bnStartTransfer.EnableWindow(1);
	pThis->hs_bnStopTransfer.EnableWindow(0);
	pThis->hs_bnUartConnect.EnableWindow(1);

	pThis->bTransFlg = false;
	free(pThis->hs_stTransPara.pBufPtr);
	pThis->hs_stTransPara.pBufPtr = NULL;

	if(RES_OK == pThis->enTransRes)
	{
		pThis->MessageBox(_T("File transmit over successfully!"),_T("Transger OK"),MB_ICONEXCLAMATION|MB_OK);
	}
	else
	{
		if(pThis->enTransRes == RES_ERR_CANCEL)
			sprintf_s(str,"Receiver terminated the transmition!");
		else
			sprintf_s(str,"File transmit error: %d", pThis->enTransRes);

		pThis->MessageBox(_T(str),_T("Transger error"),MB_ICONHAND|MB_OK);
	}

	return 0;
}

void comm_config::OnBnClickedButtonStartTransfer()
{
	if(strcmp(hs_file, "") == 0)
	{
		MessageBox(_T("Bin file path invalidate!"), _T("Transger Error"), MB_ICONHAND|MB_OK);
		return ;
	}

	if(bConnectFlg == false)
	{
		MessageBox(_T("The communiction have not connected!"),_T("Transger Error"),MB_ICONHAND|MB_OK);
		return ;
	}	

	memset(&hs_status, 0, sizeof(hs_backcall_para));
	bTransFlg = true;
	hThread[0] = CreateThread(NULL, 0, StatusThreadProc, (LPVOID)this, 0, &dwThreadId[0]);
	hThread[1] = CreateThread(NULL, 0, TransmitThreadProc, (LPVOID)this, 0, &dwThreadId[1]);	
	
	this->hs_bnStopTransfer.EnableWindow(1);
	this->hs_bnStartTransfer.EnableWindow(0);
	this->hs_bnUartConnect.EnableWindow(0);
}


void comm_config::OnBnClickedButtonGetFile()
{
	char str[255];
	int tmp;

	CFileDialog dlg(TRUE, _T("*.*"), NULL, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("bin file (*.pack)|*.pack|All Files (*.*)|*.*|"));
	dlg.m_ofn.lpstrTitle = _T("select the files to be transmitted");
	GetCurrentDirectory(255, str);
	dlg.m_ofn.lpstrInitialDir = str;// _T("e:");
	if(strcmp(w_curdir, "") != 0)
		dlg.m_ofn.lpstrInitialDir = w_curdir;

	if( dlg.DoModal() == IDOK )
	{
		this->hs_file = dlg.GetPathName();
		this->UpdateData(FALSE);
		this->hs_editFileName.SetWindowText(hs_file);

		tmp = dlg.GetPathName().ReverseFind('\\');
		memcpy(w_curdir ,(LPSTR)(LPCTSTR) dlg.GetPathName(), tmp+1);
	}
	else
	{
		this->hs_file = "";
		this->UpdateData(FALSE);
		this->hs_editFileName.SetWindowText(hs_file);
	}
}

void comm_config::OnBnClickedButtonStopTransfer()
{
	this->hs_bnStartTransfer.EnableWindow(1);
	this->hs_bnStopTransfer.EnableWindow(0);
	this->hs_bnUartConnect.EnableWindow(1);

	bTransFlg = false;

	TerminateThread(hThread[1], 0);

	if(hs_stTransPara.pBufPtr != NULL)
	{
		free(hs_stTransPara.pBufPtr);
		hs_stTransPara.pBufPtr = NULL;
	}

	MessageBox(_T("File transmit terminated!"), _T("Transger Error"), MB_ICONHAND|MB_OK);
}

#if 0
char h2b(char v)
{
	if((v>='a') && (v<='f'))
		return (v-'a' + 0xa) ;
	else if((v>='A') && (v<='F'))
		return (v-'A' + 0xa);
	else
		return (v-'0');
}

char hexAscii2bin(char *ptr)
{
	char tmp;
	
	tmp = (h2b(ptr[0]) << 4) &0xf0;
	tmp |= h2b(ptr[1]);

	return tmp;
}

char b2h(char v)
{
	v &= 0xf;

	if(v>=0xa)
		return ((v-0xa)+'A');
	else
		return (v+'0');
}

void bin2hexAscii(char *ptr, char ch)
{
	char tmp;

	tmp = ch>>4;
	ptr[0] = b2h(tmp);
	ptr[1] = b2h(ch);
	ptr[2] = ':';
}
#endif



void comm_config::OnBnClickedButtonEraseConfig()
{
	int res;
	unsigned char buf[256];
	char str[256];

	MessageBox(_T("If rom program can't right run because of the error config data, suggested steps as following:\n\n1. Press down the default button of hs66xx!\n2. Reset hs66xx!\n3. Press OK button of the tools!"), _T("Suggestion"), MB_ICONWARNING|MB_OK);

	res = hs_comm.SendCmd(COMM_CMD_ERASE_ALLCONFIGDATA, buf, 1, buf+10, 4);

	if(res == 0)
		MessageBox(_T("Erase config data successfull!"), _T("Infomation"), MB_ICONINFORMATION|MB_OK);
	else
	{
		sprintf(str, "Erase config data error!\n\nError code : %d", res);
		MessageBox(_T(str), _T("Infomation"), MB_ICONERROR|MB_OK);
	}
}
