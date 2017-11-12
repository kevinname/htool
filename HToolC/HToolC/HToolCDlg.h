
// HToolCDlg.h : header file
//

#pragma once

#include "comm_hal.h"
#include "afxcmn.h"
#include "comm_config.h"
#include "hs_package.h"
#include "hs_tone.h"

#define CFG_DATA_DIR		"dlg.config"
#define CFG_DATA_FILE		"dlg.config/hanToolC.cfg"

#define CFG_DATA_FLAG		0x46435348

#define FOR_EACH_STL(ior, container, container_type)\
    for(container_type::iterator ior=container.begin(); ior != container.end(); ++ior)
#define FOR_EACH_STL_CONST(ior, container, container_type)\
    for(container_type::const_iterator ior=container.begin(); ior != container.end(); ++ior)

#define FOR_EACH_DLG(ior) \
    FOR_EACH_STL(ior, m_lstChildDlg, std::list<CChildDlgInfo>)

struct CChildDlgInfo
{
    DWORD dwID;
    CString strName;
    CDialogEx *pDlg;
    CChildDlgInfo(DWORD _dwID = 0, CString _strName = _T("NoName"), CDialogEx *_pDlg = NULL)
    : dwID(_dwID)
    , strName(_strName)
    , pDlg(_pDlg)
    {}
};

// CHToolCDlg dialog
class CHToolCDlg : public CDialogEx
{
// Construction
public:
	CHToolCDlg(CWnd* pParent = NULL);	// standard constructor
    ~CHToolCDlg(); //qli+

// Dialog Data
	enum { IDD = IDD_HTOOLC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;   

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:	
	std::list<CChildDlgInfo> m_lstChildDlg; //qli+
	int m_page; 

	CTabCtrl hs_mainTab;

	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	void SaveDlgData(FILE *dataFile, CDialogEx *pDlg, CString strName);
	void GetDlgData(FILE *dataFile, CDialogEx *pDlg, CString strName);
	void RestoreDlgData(void);
};
