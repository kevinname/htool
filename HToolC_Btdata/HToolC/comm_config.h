#pragma once

#include "comm_cmd.h"
#include "afxwin.h"
#include "afxcmn.h"

#define DIR_PATCH_LENGTH			0x100

typedef struct
{
	unsigned char *pBufPtr;
	unsigned int fileLen;
}hs_trans_para;

// comm_config dialog

class comm_config : public CDialogEx
{
	DECLARE_DYNAMIC(comm_config)

public:
	comm_config(CWnd* pParent = NULL);   // standard constructor
	virtual ~comm_config();

// Dialog Data
	enum { IDD = IDD_COMM_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	comm_cmd hs_comm;
	char uartDevList[10][32];
	bool bConnectFlg;
	bool bTransFlg;
	CString hs_file;

	DWORD dwThreadId[2];
	HANDLE hThread[2];

	hs_backcall_para hs_status;
	int enTransRes;
	hs_trans_para hs_stTransPara;
	char    w_curdir[DIR_PATCH_LENGTH];

	CComboBox hs_combUartDevice;	
	CEdit hs_editBaudrate;
	CButton hs_bnUartConnect;

	virtual BOOL OnInitDialog(void);
	afx_msg void OnBnClickedButtonUartConnect();
	
	CEdit hs_editFileName;
	CButton hs_bnSelectFile;
	CButton hs_bnStartTransfer;
	afx_msg void OnBnClickedButtonStartTransfer();
	afx_msg void OnBnClickedButtonGetFile();

	CEdit hs_statusFileSize;
	CEdit hs_statusTotalCnt;
	CEdit hs_statusSuccessCnt;
	CEdit hs_statusTotalErrCnt;
	CEdit hs_statusLastErrIdx;
	CEdit hs_statusLastErrCnt;
	CProgressCtrl hs_statusProcess;
	CButton hs_bnStopTransfer;
	afx_msg void OnBnClickedButtonStopTransfer();
	afx_msg void OnBnClickedButtonEraseConfig();
};
